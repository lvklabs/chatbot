/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Chatbot.
 *
 * LVK Chatbot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Chatbot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Chatbot.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "back-end/appfacade.h"
#include "back-end/rule.h"
#include "back-end/aiadapter.h"
#include "back-end/rloghelper.h"
#include "back-end/chatbotfactory.h"
#include "nlp-engine/rule.h"
#include "nlp-engine/enginefactory.h"
#include "nlp-engine/sanitizerfactory.h"
#include "nlp-engine/lemmatizerfactory.h"
#include "nlp-engine/nlpproperties.h"
#include "common/random.h"
#include "common/globalstrings.h"
#include "stats/statsmanager.h"

#include <QDateTime>
#include <QMetaType>
#include <QtDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#define FILE_METADATA_CHAT_TYPE           "chat_type"
#define FILE_METADATA_USERNAME            "username"
#define FILE_METADATA_CHAT_USERNAME       "chat_username"
#define FILE_METADATA_NLP_OPTIONS         "nlp_options"
#define FILE_METADATA_ROSTER              "roster"
#define FILE_METADATA_BLACK_ROSTER        "black_roster"

//--------------------------------------------------------------------------------------------------
// Non-members Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// Make Nlp::Rule from BE::Rule
inline Lvk::Nlp::Rule toNlpRule(const Lvk::BE::Rule *rule)
{
    Lvk::Nlp::Rule nlpRule(rule->id(), rule->input(), rule->output());

    if (rule->parent()) {
        nlpRule.setTopic(rule->parent()->name());
    }

    QStringList targets;
    foreach (const Lvk::BE::Target &t, rule->target()) {
        targets.append(t.username);
    }

    nlpRule.setTarget(targets);

    return nlpRule;
}

//--------------------------------------------------------------------------------------------------

inline Lvk::CA::ContactInfoList toChatbotRoster(const Lvk::BE::Roster &roster)
{
    Lvk::CA::ContactInfoList infoList;
    foreach (const Lvk::BE::RosterItem &item, roster) {
        infoList.append(Lvk::CA::ContactInfo(item.username, item.username));
    }
    return infoList;
}

} // namespace


//--------------------------------------------------------------------------------------------------
// Constructors & destructor
//--------------------------------------------------------------------------------------------------

Lvk::BE::AppFacade::AppFacade(QObject *parent /*= 0*/)
    : QObject(parent),
      m_evasivesRule(0),
      m_nlpEngine(Nlp::EngineFactory().createEngine()),
      m_chatbot(0),
      m_nlpOptions(0)
{
    init();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::AppFacade::AppFacade(Nlp::Engine *nlpEngine, QObject *parent /*= 0*/)
    : QObject(parent),
      m_evasivesRule(0),
      m_nlpEngine(nlpEngine),
      m_chatbot(0),
      m_nlpOptions(0) // FIXME value?
{
    init();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::init()
{
    m_rlogh.logAppLaunched();

    qRegisterMetaTypeStreamOperators<BE::RosterItem>("Lvk::BEk::RosterItem");
    qRegisterMetaTypeStreamOperators<BE::Roster>("Lvk::BEk::Roster");

    connect(Stats::StatsManager::manager(),
            SIGNAL(scoreRemainingTime(int)),
            SIGNAL(scoreRemainingTime(int)));

    connect(&m_account,
            SIGNAL(accountOk(AccountVerifier::AccountInfo)),
            SLOT(onAccountOk(AccountVerifier::AccountInfo)));

    connect(&m_account,
            SIGNAL(accountError(int,QString)),
            SLOT(onAccountError(int,QString)));

    setNlpEngineOptions(defaultNlpOptions());

    setupChatbot();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::AppFacade::~AppFacade()
{
    m_rlogh.logAppClosed();

    close();

    delete m_chatbot;
    delete m_nlpEngine;
}

//--------------------------------------------------------------------------------------------------
// History and Stats files
//--------------------------------------------------------------------------------------------------

QString Lvk::BE::AppFacade::getExtrasPath()
{
    QFileInfo info(m_rules.filename());
    QString extrasPath = info.canonicalPath() + QDir::separator() + info.baseName() + "_extras";

    QDir().mkpath(extrasPath);

    return extrasPath + QDir::separator();
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::AppFacade::getStatsFilename()
{
    return getExtrasPath() + m_rules.chatbotId() + ".stat";
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::AppFacade::getHistoryFilename()
{
    return getExtrasPath() + m_rules.chatbotId() + ".hist";
}

//--------------------------------------------------------------------------------------------------
// Rules files
//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::newFile(const QString &filename)
{
    bool created = false;

    close();

    if (setDefaultRules() && setDefaultNlpOptions()) {
        created = m_rules.saveAs(filename);
    }

    if (created) {
        generalSetup();
    } else {
        close();
        setupChatbot();
    }

    return created;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::load(const QString &filename)
{
    bool loaded = false;

    close();

    if (!filename.isEmpty()) {
        loaded = m_rules.load(filename);
    }

    if (loaded) {
        generalSetup();
        m_rlogh.logAutoScore(bestScore());
        m_rlogh.logDefaultMetrics();
    } else {
        close();
        setupChatbot();
    }

    return loaded;
}

//--------------------------------------------------------------------------------------------------

unsigned Lvk::BE::AppFacade::defaultNlpOptions()
{
    unsigned options = RemoveDupChars | SanitizePostLemma;

#ifdef FREELING_SUPPORT
    options |= BE::AppFacade::LemmatizeSentence;
#endif

    return options;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::setDefaultRules()
{
    Rule *rootRule = m_rules.rootRule();

    if (!rootRule) {
        return false;
    }

    Rule *catGreetings = new Rule(tr("Greetings"));

    catGreetings->setType(Rule::ContainerRule);

    QStringList rule1InputList;
    QStringList rule1OutputList;
    rule1InputList  << QString(tr("Hello"));
    rule1OutputList << QString(tr("Hello!"));

    Rule * rule1 = new Rule("", rule1InputList, rule1OutputList);

    catGreetings->appendChild(rule1);

    Rule *evasives  = new Rule(tr("Evasives"));
    evasives->setType(Rule::EvasiveRule);

    m_evasivesRule = evasives;

    rootRule->appendChild(catGreetings);
    rootRule->appendChild(evasives);

    for (Rule::iterator it = rootRule->begin(); it != rootRule->end(); ++it) {
        (*it)->setStatus(Rule::Saved);
    }

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::setDefaultNlpOptions()
{
    m_rules.setMetadata(FILE_METADATA_NLP_OPTIONS, defaultNlpOptions());

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::generalSetup()
{
    Stats::StatsManager::manager()->setFilename(getStatsFilename());

    m_rlogh.setChatbotId(m_rules.chatbotId());
    m_rlogh.setUsername(m_rules.metadata(FILE_METADATA_USERNAME).toString());

    // Warning order is importante here:
    setNlpEngineOptions(m_rules.metadata(FILE_METADATA_NLP_OPTIONS).toUInt());
    setupChatbot();
    refreshNlpEngine();

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::save()
{
    return m_rules.save();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::saveAs(const QString &filename)
{
    bool saved = m_rules.saveAs(filename);

    if (saved) {
        // New chat chatbot id, we need to reset everything
        deleteCurrentChatbot();
        generalSetup();
    }

    return saved;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::hasUnsavedChanges() const
{
    return m_rules.hasUnsavedChanges();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::close()
{
    // If chatbot never saved
    if (m_rules.filename().isEmpty()) {
        Stats::StatsManager::manager()->clear();

        if (m_chatbot) {
            m_chatbot->clearHistory();
        }
    } else {
        m_rlogh.logAutoScore(bestScore());
        m_rlogh.logDefaultMetrics();
    }

    if (m_nlpEngine) {
        m_nlpEngine->setRules(Nlp::RuleList());
    }

    if (m_chatbot) {
        m_chatbot->disconnectFromServer();
        deleteCurrentChatbot();
    }

    Stats::StatsManager::manager()->stopTicking();

    m_rlogh.clear();
    m_targets.clear();
    m_rules.close();
    m_evasivesRule = 0;

    Stats::StatsManager::manager()->setFilename("");
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::importRules(const QString &inputFile)
{
    return m_rules.importRules(inputFile);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::importRules(BE::Rule *container, const QString &inputFile)
{
    return m_rules.importRules(container, inputFile);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::exportRules(const BE::Rule *container, const QString &outputFile)
{
    return m_rules.exportRules(container, outputFile);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::mergeRules(BE::Rule *container)
{
    bool merged = m_rules.mergeRules(container);

    refreshNlpEngine();

    return merged;
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::AppFacade::rootRule()
{
    return m_rules.rootRule();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::AppFacade::evasivesRule()
{
    return m_evasivesRule;
}

//--------------------------------------------------------------------------------------------------
// Nlp Engine methods
//--------------------------------------------------------------------------------------------------

QStringList Lvk::BE::AppFacade::getEvasives() const
{
    return m_evasivesRule ? const_cast<const BE::Rule*>(m_evasivesRule)->output() : QStringList();
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::AppFacade::getResponse(const QString &input, MatchList &matches) const
{
    QString response;

    // Try with all targets

    foreach (const QString &target, m_targets) {
        response = getResponse(input, target, matches);

        if (!matches.isEmpty()) {
            break;
        }
    }

    if (matches.isEmpty()) {
        response = getResponse(input, "", matches);
    }

    return response;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::AppFacade::getResponse(const QString &input, const QString &target,
                                      MatchList &matches) const
{
    matches.clear();
    QString response;

    if (m_nlpEngine) {
        Nlp::Engine::MatchList nlpRulesMatched;
        response = m_nlpEngine->getResponse(input, target, nlpRulesMatched);

        if (!nlpRulesMatched.isEmpty()) {
            Nlp::RuleId ruleId = nlpRulesMatched.first().first;
            int inputNumber = nlpRulesMatched.first().second;
            matches.append(qMakePair(ruleId, inputNumber));
        } else {
            QStringList evasives = getEvasives();
            response = !evasives.isEmpty() ?
                        evasives[Cmn::Random::getInt(0, evasives.size() - 1)] : "";
        }
    } else {
        qCritical("NLP engine not set");
    }

    return response;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::refreshNlpEngine()
{
    m_evasivesRule = 0;
    m_targets.clear();

    if (m_nlpEngine) {
        Nlp::RuleList nlpRules;
        buildNlpRulesOf(m_rules.rootRule(), nlpRules);
        m_nlpEngine->setRules(nlpRules);
        refreshEvasives();
    } else {
        qCritical("NLP engine not set");
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::buildNlpRulesOf(const BE::Rule *parentRule, Nlp::RuleList &nlpRules)
{
    if (!parentRule) {
        return;
    }

    for (int i = 0; i < parentRule->childCount(); ++i) {
        const BE::Rule *child = parentRule->child(i);

        if (!child->id()) {
            const_cast<BE::Rule *>(child)->setId(m_rules.nextRuleId());
        }

        if (child->type() == Rule::OrdinaryRule) {
            storeTargets(child->target());
            nlpRules.append(toNlpRule(child));
        } else if (child->type() == Rule::EvasiveRule) {
            m_evasivesRule = const_cast<BE::Rule *>(child);
        } else if (child->type() == BE::Rule::ContainerRule) {
            buildNlpRulesOf(child, nlpRules);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::storeTargets(const TargetList &targets)
{
    foreach (const Target &t, targets) {
        m_targets.insert(t.username);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::refreshEvasives()
{
    dynamic_cast<AIAdapter *>(m_chatbot->AI())->setEvasives(getEvasives());
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setNlpEngineOptions(unsigned options)
{
    if (m_nlpOptions == options) {
        return;
    }

    if ((options & RemoveDupChars) && !(m_nlpOptions & RemoveDupChars)) {
        m_nlpEngine->setPreSanitizer(Nlp::SanitizerFactory().createPreSanitizer());
    }
    if (!(options & RemoveDupChars) && (m_nlpOptions & RemoveDupChars)) {
        m_nlpEngine->setPreSanitizer(0);
    }

    if ((options & LemmatizeSentence) && !(m_nlpOptions & LemmatizeSentence)) {
        m_nlpEngine->setLemmatizer(Nlp::LemmatizerFactory().createLemmatizer());
    }
    if (!(options & LemmatizeSentence) && (m_nlpOptions & LemmatizeSentence)) {
        m_nlpEngine->setLemmatizer(0);
    }

    if ((options & SanitizePostLemma) && !(m_nlpOptions & SanitizePostLemma)) {
        m_nlpEngine->setPostSanitizer(Nlp::SanitizerFactory().createPostSanitizer());
    }
    if (!(options & SanitizePostLemma) && (m_nlpOptions & SanitizePostLemma)) {
        m_nlpEngine->setPostSanitizer(0);
    }

    if ((options & ExactMatchSupport) && !(m_nlpOptions & ExactMatchSupport)) {
        m_nlpEngine->setProperty(NLP_PROP_EXACT_MATCH, true);
    }
    if (!(options & ExactMatchSupport) && (m_nlpOptions & ExactMatchSupport)) {
        m_nlpEngine->setProperty(NLP_PROP_EXACT_MATCH, false);
    }

    m_nlpOptions = options;

    if (m_rules.metadata(FILE_METADATA_NLP_OPTIONS).toUInt() != options) {
        m_rules.setMetadata(FILE_METADATA_NLP_OPTIONS, options);
    }
}

//--------------------------------------------------------------------------------------------------

unsigned Lvk::BE::AppFacade::nlpEngineOptions() const
{
    return m_nlpOptions;
}

//--------------------------------------------------------------------------------------------------
// Chat methods
//--------------------------------------------------------------------------------------------------

Lvk::BE::ChatType Lvk::BE::AppFacade::chatType() const
{
    return static_cast<BE::ChatType>
            (m_rules.metadata(FILE_METADATA_CHAT_TYPE).toInt());
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setChatType(Lvk::BE::ChatType type)
{
    m_rules.setMetadata(FILE_METADATA_CHAT_TYPE, type);
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::AppFacade::chatUsername() const
{
    return m_rules.metadata(FILE_METADATA_CHAT_USERNAME).toString();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setChatUsername(const QString &username)
{
    m_rules.setMetadata(FILE_METADATA_CHAT_USERNAME, username);
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::AppFacade::username() const
{
    return m_rules.metadata(FILE_METADATA_USERNAME).toString();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setUsername(const QString &username)
{
    m_rules.setMetadata(FILE_METADATA_USERNAME, username);

    m_rlogh.setUsername(username);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::verifyAccount(Lvk::BE::ChatType type, const QString &user,
                                       const QString &passwd)
{
    m_account.verify(type, user, passwd);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::cancelVerifyAccount()
{
    m_account.abort();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::onAccountOk(const AccountVerifier::AccountInfo &info)
{
    m_rlogh.logAccountVerified(info.username, info.type == BE::FbChat ? "facebook" : "gtalk");

    setChatType(info.type);
    setUsername(info.username);
    setChatUsername(info.chatUsername);
    setRoster(info.roster);
    setBlackRoster(info.roster); // Initially all contacts are in the black list

    emit accountOk();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::onAccountError(int err, const QString &msg)
{
    emit accountError(err, msg);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::connectToChat(const QString &passwd)
{
    if (!chatUsername().isEmpty()) {
        connectToChat(chatType(), chatUsername(), passwd);
    } else {
        connectToChat(chatType(), username(), passwd); // Backward compatibility
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::connectToChat(Lvk::BE::ChatType type, const QString &user,
                                       const QString &passwd)
{
    setupChatbot(type);

    m_chatbot->connectToServer(user, passwd);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::disconnectFromChat()
{
    m_chatbot->disconnectFromServer();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::isConnected() const
{
    return m_chatbot && m_chatbot->isConnected();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setupChatbot()
{
    setupChatbot(chatType());
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setupChatbot(ChatType type)
{
    if (type != FbChat && type != GTalkChat) {
        qWarning() << "Invalid chat type, defaulting to FbChat!";
        type = FbChat;
    }

    if (m_chatbot && m_currentChatbotType != type) {
        deleteCurrentChatbot();
    }

    if (!m_chatbot) {
        m_currentChatbotType = type;
        m_chatbot = BE::ChatbotFactory().createChatbot(m_rules.chatbotId(), type);
        m_chatbot->setAI(new AIAdapter(m_rules.chatbotId(), m_nlpEngine));
        m_chatbot->setBlackListRoster(toChatbotRoster(blackRoster()));
        m_chatbot->setHistoryFilename(getHistoryFilename());

        refreshEvasives();

        // FIXME add method to remap Chatbot error codes to AppFacade error codes
        connect(m_chatbot, SIGNAL(error(int)),     SIGNAL(connectionError(int)));
        connect(m_chatbot, SIGNAL(connected()),    SLOT(onConnected()));
        connect(m_chatbot, SIGNAL(disconnected()), SLOT(onDisconnected()));
        connect(m_chatbot, SIGNAL(newConversationEntry(Cmn::Conversation::Entry)),
                SLOT(onConversationEntry(Cmn::Conversation::Entry)));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::deleteCurrentChatbot()
{
    delete m_chatbot; // TODO consider using std::auto_ptr
    m_chatbot = 0;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::onConnected()
{
    emit connected();
    Stats::StatsManager::manager()->startTicking();
    m_rlogh.logChatbotConnected(true);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::onDisconnected()
{
    emit disconnected();
    Stats::StatsManager::manager()->stopTicking();
    m_rlogh.logChatbotConnected(false);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::onConversationEntry(const Cmn::Conversation::Entry &entry)
{
    emit newConversationEntry(entry);
    Stats::StatsManager::manager()->updateScoreWith(entry);
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Roster Lvk::BE::AppFacade::roster()
{
    BE::Roster persistedRoster = m_rules.metadata(FILE_METADATA_ROSTER).value<BE::Roster>();
    BE::Roster roster;

    if (m_chatbot->isConnected()) {
        foreach (const CA::ContactInfo &info, m_chatbot->roster()) {
            roster.append(BE::RosterItem(info.username, info.fullname));
        }
        if (persistedRoster != roster) {
            m_rules.setMetadata(FILE_METADATA_ROSTER, QVariant::fromValue(roster));
        }
    } else {
        roster = persistedRoster;
    }

    return roster;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setRoster(const BE::Roster &roster)
{
    Stats::StatsManager::manager()->setMetric(Stats::RosterSize, roster.size());

    m_rules.setMetadata(FILE_METADATA_ROSTER, QVariant::fromValue(roster));
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setBlackRoster(const BE::Roster &roster)
{
    Stats::StatsManager::manager()->setMetric(Stats::BlackRosterSize, roster.size());

    m_chatbot->setBlackListRoster(toChatbotRoster(roster));

    m_rules.setMetadata(FILE_METADATA_BLACK_ROSTER, QVariant::fromValue(roster));
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Roster Lvk::BE::AppFacade::blackRoster() const
{
    return m_rules.metadata(FILE_METADATA_BLACK_ROSTER).value<BE::Roster>();
}

//--------------------------------------------------------------------------------------------------
// Chat history
//--------------------------------------------------------------------------------------------------

const Lvk::Cmn::Conversation & Lvk::BE::AppFacade::chatHistory()
{
    return m_chatbot->chatHistory();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::clearChatHistory()
{
    m_chatbot->clearHistory();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::clearChatHistory(const QDate &date, const QString &user)
{
    Cmn::Conversation conv;
    foreach (const Cmn::Conversation::Entry &entry, m_chatbot->chatHistory().entries()) {
        if (entry.from != user || entry.dateTime.date() != date) {
            conv.append(entry);
        }
    }

    m_chatbot->setChatHistory(conv);
}

//--------------------------------------------------------------------------------------------------
// Score
//--------------------------------------------------------------------------------------------------

Lvk::Stats::Score Lvk::BE::AppFacade::currentScore()
{
    // TODO improve performance, do not update all the time
    Stats::StatsManager::manager()->updateScoreWith(rootRule());

    return Stats::StatsManager::manager()->currentScore();
}

//--------------------------------------------------------------------------------------------------

Lvk::Stats::Score Lvk::BE::AppFacade::bestScore()
{
    return Stats::StatsManager::manager()->bestScore();
}

//--------------------------------------------------------------------------------------------------

int Lvk::BE::AppFacade::scoreRemainingTime() const
{
    return Stats::StatsManager::manager()->scoreRemainingTime();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::uploadBestScore()
{
    if (!username().isEmpty()) {
        return m_rlogh.logManualScore(bestScore());
    } else {
        qCritical() << "Cannot upload score without username";
        return false;
    }
}

