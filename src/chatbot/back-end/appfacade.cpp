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
#include "nlp-engine/rule.h"
#include "nlp-engine/enginefactory.h"
#include "nlp-engine/sanitizerfactory.h"
#include "nlp-engine/lemmatizerfactory.h"
#include "nlp-engine/nlpproperties.h"
#include "common/random.h"
#include "common/globalstrings.h"
#include "chat-adapter/fbchatbot.h"
#include "chat-adapter/gtalkchatbot.h"
#include "stats/statsmanager.h"

#include <QDateTime>

#define FILE_METADATA_CHAT_TYPE           "chat_type"
#define FILE_METADATA_USERNAME            "username"
#define FILE_METADATA_NLP_OPTIONS         "nlp_options"

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

inline Lvk::CA::Chatbot *createChatbot(const QString &id, Lvk::BE::AppFacade::ChatType type)
{
    switch (type) {
    case Lvk::BE::AppFacade::FbChat:
        return new Lvk::CA::FbChatbot(id);
    case Lvk::BE::AppFacade::GTalkChat:
        return new Lvk::CA::GTalkChatbot(id);
    default:
        qCritical() << "newChatbot() Invalid chat type" << type;
        return 0;
    }
}

//--------------------------------------------------------------------------------------------------
// convert CA::ContactInfoList to BE::Roster

inline Lvk::BE::Roster toBERoster(const Lvk::CA::ContactInfoList &infoList)
{
    Lvk::BE::Roster roster;

    foreach (const Lvk::CA::ContactInfo &info, infoList) {
        roster.append(Lvk::BE::RosterItem(info.username, info.fullname));
    }

    return roster;
}

//--------------------------------------------------------------------------------------------------
// convert BE::Roster to CA::ContactInfoList

inline Lvk::CA::ContactInfoList toCAContactInfoList(const Lvk::BE::Roster &roster)
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
      m_tmpChatbot(0),
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
      m_tmpChatbot(0),
      m_nlpOptions(0) // FIXME value?
{
    init();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::init()
{
    connect(Stats::StatsManager::manager(),
            SIGNAL(scoreRemainingTime(int)),
            SIGNAL(scoreRemainingTime(int)));

    setupChatbot();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::AppFacade::~AppFacade()
{
    close();

    delete m_tmpChatbot;
    delete m_chatbot;
    delete m_nlpEngine;
}

//--------------------------------------------------------------------------------------------------
// Rules files
//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::load(const QString &filename)
{
    close();

    bool loaded = false;

    if (filename.isEmpty()) {
        loaded = setDefaultRules();

        m_rules.setMetadata(FILE_METADATA_NLP_OPTIONS, getDefaultNlpOptions());
        m_rules.setAsSaved();
    } else {
        loaded = m_rules.load(filename);
    }

    if (loaded) {
        Stats::StatsManager::manager()->setChatbotId(m_rules.chatbotId());
        m_rlogh.setChatbotId(m_rules.chatbotId());
        m_rlogh.setUsername(m_rules.metadata(FILE_METADATA_USERNAME).toString());
        setNlpEngineOptions(m_rules.metadata(FILE_METADATA_NLP_OPTIONS).toUInt());
        setupChatbot();
        refreshNlpEngine();
    } else {
        close();
        setupChatbot();
    }

    return loaded;
}

//--------------------------------------------------------------------------------------------------

unsigned Lvk::BE::AppFacade::getDefaultNlpOptions()
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

bool Lvk::BE::AppFacade::save()
{
    return m_rules.save();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::saveAs(const QString &filename)
{
    bool saved = m_rules.saveAs(filename);

    if (saved) {
        // New chat chatbot id, we need to reset the chatbot and RLogHelper
        deleteCurrentChatbot();
        setupChatbot();
        m_rlogh.setChatbotId(m_rules.chatbotId());
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
    }

    if (m_nlpEngine) {
        // CHECK Do not reset NLP options?
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

    Stats::StatsManager::manager()->setChatbotId("");
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

Lvk::BE::AppFacade::ChatType Lvk::BE::AppFacade::chatType() const
{
    return static_cast<BE::AppFacade::ChatType>
            (m_rules.metadata(FILE_METADATA_CHAT_TYPE).toInt());
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setChatType(Lvk::BE::AppFacade::ChatType type)
{
    m_rules.setMetadata(FILE_METADATA_CHAT_TYPE, type);
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

void Lvk::BE::AppFacade::verifyAccount(Lvk::BE::AppFacade::ChatType type, const QString &user,
                                       const QString &passwd)
{
    // FIXME add mutex

    // CHECK
    // If there is already a verification in progress, we abort it by deleting the object
    if (m_tmpChatbot) {
        delete m_tmpChatbot;
    }

    m_tmpChatbot = createChatbot("", type);

    if (m_tmpChatbot) {
        connect(m_tmpChatbot, SIGNAL(connected()), SLOT(onAccountOk()));
        connect(m_tmpChatbot, SIGNAL(error(int)), SLOT(onAccountError(int)));
        m_tmpChatbot->connectToServer(user, passwd);
    } else {
        emit accountError(UnknownServerTypeError);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::cancelVerifyAccount()
{
    delete m_tmpChatbot;
    m_tmpChatbot = 0;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::onAccountOk()
{
    m_rlogh.logAccountVerified(m_tmpChatbot->username(), m_tmpChatbot->domain());

    Roster roster = toBERoster(m_tmpChatbot->roster());

    m_tmpChatbot->deleteLater();
    m_tmpChatbot = 0;

    emit accountOk(roster);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::onAccountError(int err)
{
    m_tmpChatbot->deleteLater();
    m_tmpChatbot = 0;

    emit accountError(err);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::connectToChat(const QString &passwd)
{
    connectToChat(chatType(), username(), passwd);
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::connectToChat(Lvk::BE::AppFacade::ChatType type, const QString &user,
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
        m_chatbot = createChatbot(m_rules.chatbotId(), type);
        m_chatbot->setAI(new AIAdapter(m_rules.chatbotId(), m_nlpEngine));

        refreshEvasives();
        connectChatbotSignals();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::deleteCurrentChatbot()
{
    delete m_chatbot; // TODO consider using std::auto_ptr
    m_chatbot = 0;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::connectChatbotSignals()
{
    connect(m_chatbot, SIGNAL(connected()),    SIGNAL(connected()));
    connect(m_chatbot, SIGNAL(disconnected()), SIGNAL(disconnected()));

    // FIXME add method to remap Chatbot error codes to AppFacade error codes
    connect(m_chatbot, SIGNAL(error(int)),     SIGNAL(connectionError(int)));

    connect(m_chatbot, SIGNAL(newConversationEntry(Cmn::Conversation::Entry)),
            SIGNAL(newConversationEntry(Cmn::Conversation::Entry)));

    connect(m_chatbot, SIGNAL(connected()),
            Stats::StatsManager::manager(), SLOT(startTicking()));
    connect(m_chatbot, SIGNAL(disconnected()),
            Stats::StatsManager::manager(), SLOT(stopTicking()));
    connect(m_chatbot, SIGNAL(newConversationEntry(Cmn::Conversation::Entry)),
            Stats::StatsManager::manager(), SLOT(updateScoreWith(Cmn::Conversation::Entry)));
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Roster Lvk::BE::AppFacade::roster() const
{
    return toBERoster(m_chatbot->roster());
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setBlackListRoster(const Roster &roster)
{
    m_chatbot->setBlackListRoster(toCAContactInfoList(roster));
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

