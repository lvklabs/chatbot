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

#include "appfacade.h"
#include "rule.h"
#include "nlpengine.h"
#include "nlprule.h"
#include "exactmatchengine.h"
#include "simpleaimlengine.h"
#include "defaultsanitizer.h"
#include "random.h"
#include "fbchatbot.h"
#include "gtalkchatbot.h"
#include "defaultvirtualuser.h"

namespace Lvk
{

namespace BE
{

typedef Lvk::Nlp::SimpleAimlEngine DefaultEngine;
typedef Lvk::Nlp::DefaultSanitizer DefaultSanitizer;

} // namespace BE

} // namespace Lvk


//--------------------------------------------------------------------------------------------------
// Non-members Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// Make Nlp::Rule from BE::Rule

inline Lvk::Nlp::Rule makeNlpRule(const Lvk::BE::Rule *rule)
{
    Lvk::Nlp::Rule nlpRule(rule->id(), rule->input(), rule->output());

    QStringList targets;
    foreach (const Lvk::BE::Target &t, rule->target()) {
        targets.append(t.username);
    }

    nlpRule.setTarget(targets);

    return nlpRule;
}

//--------------------------------------------------------------------------------------------------

inline Lvk::CA::Chatbot *newChatbot(Lvk::BE::AppFacade::ChatType type)
{
    switch (type) {
    case Lvk::BE::AppFacade::FbChat:
        return new Lvk::CA::FbChatbot();
    case Lvk::BE::AppFacade::GTalkChat:
        return new Lvk::CA::GTalkChatbot();
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
      m_nlpEngine(new Lvk::BE::DefaultEngine(new Lvk::BE::DefaultSanitizer())),
      m_chatbot(0),
      m_tmpChatbot(0)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::AppFacade::AppFacade(Nlp::Engine *nlpEngine, QObject *parent /*= 0*/)
    : QObject(parent),
      m_evasivesRule(0),
      m_nlpEngine(nlpEngine),
      m_chatbot(0),
      m_tmpChatbot(0)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::AppFacade::~AppFacade()
{
    delete m_chatbot;
    delete m_tmpChatbot;
    delete m_nlpEngine;
}

//--------------------------------------------------------------------------------------------------
// Rules files
//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::load(const QString &filename)
{
    close();

    bool loaded = filename.isEmpty() ? setDefaultRules() : m_rulesFile.load(filename);

    if (loaded) {
        refreshNlpEngine();
    } else {
        close();
    }

    return loaded;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::setDefaultRules()
{
    BE::Rule *rootRule = m_rulesFile.rootRule();

    if (!rootRule) {
        return false;
    }

    BE::Rule *catGreetings = new BE::Rule(tr("Greetings"));

    catGreetings->setType(BE::Rule::ContainerRule);

    QStringList rule1InputList;
    QStringList rule1OutputList;
    rule1InputList  << QString(tr("Hello"));
    rule1OutputList << QString(tr("Hello!"));

    BE::Rule * rule1 = new BE::Rule("", rule1InputList, rule1OutputList);

    catGreetings->appendChild(rule1);

    BE::Rule *evasives  = new BE::Rule(tr("Evasives"));
    evasives->setType(BE::Rule::EvasiveRule);

    m_evasivesRule = evasives;

    rootRule->appendChild(catGreetings);
    rootRule->appendChild(evasives);

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::save()
{
    return m_rulesFile.save();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::saveAs(const QString &filename)
{
    deleteCurrentChatbot();

    return m_rulesFile.saveAs(filename);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::hasUnsavedChanges() const
{
    return m_rulesFile.hasUnsavedChanges();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::close()
{
    if (m_nlpEngine) {
        m_nlpEngine->setRules(Nlp::RuleList());
    }

    if (m_chatbot) {
        m_chatbot->disconnectFromServer();
        deleteCurrentChatbot();
    }

    m_rulesHash.clear();
    m_targets.clear();
    m_rulesFile.close();
    m_evasivesRule = 0;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setMetadata(const QString &key, const QVariant &value)
{
    m_rulesFile.setMetadata(key, value);
}

//--------------------------------------------------------------------------------------------------

const QVariant & Lvk::BE::AppFacade::metadata(const QString &key) const
{
    return m_rulesFile.metadata(key);
}

bool Lvk::BE::AppFacade::importRules(const QString &inputFile)
{
    return m_rulesFile.importRules(inputFile);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::importRules(BE::Rule *container, const QString &inputFile)
{
    return m_rulesFile.importRules(container, inputFile);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::exportRules(const BE::Rule *container, const QString &outputFile)
{
    return m_rulesFile.exportRules(container, outputFile);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::mergeRules(BE::Rule *container)
{
    bool merged = m_rulesFile.mergeRules(container);

    refreshNlpEngine();

    return merged;
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::AppFacade::rootRule()
{
    return m_rulesFile.rootRule();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::AppFacade::evasivesRule()
{
    return m_evasivesRule;
}

//--------------------------------------------------------------------------------------------------
// Nlp Engine methods
//--------------------------------------------------------------------------------------------------

inline Lvk::BE::DefaultVirtualUser * Lvk::BE::AppFacade::virtualUser()
{
    if (!m_chatbot) {
        setupChatbot(FbChat); // FIXME FbChat, harmless so far but very error-prone
    }

    return dynamic_cast<DefaultVirtualUser *>(m_chatbot->virtualUser());
}

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
            matches.append(qMakePair(m_rulesHash[ruleId], inputNumber));
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
    m_rulesHash.clear();
    m_targets.clear();

    if (m_nlpEngine) {
        Nlp::RuleList nlpRules;
        buildNlpRulesOf(m_rulesFile.rootRule(), nlpRules);
        m_nlpEngine->setRules(nlpRules);
        refreshEvasivesToChatbot();
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
            const_cast<BE::Rule *>(child)->setId(m_rulesFile.nextRuleId());
        }

        if (child->type() == Rule::OrdinaryRule) {
            storeTargets(child->target());
            m_rulesHash[child->id()] = child;
            nlpRules.append(makeNlpRule(child));
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
// Chat methods
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

    m_tmpChatbot = newChatbot(type);

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

void Lvk::BE::AppFacade::connectToChat(Lvk::BE::AppFacade::ChatType type, const QString &user,
                                       const QString &passwd)
{
    if (m_chatbot && m_currentChatbotType != type) {
        deleteCurrentChatbot();
    }

    if (!m_chatbot) {
        setupChatbot(type);
    }

    if (m_chatbot) {
        m_chatbot->connectToServer(user, passwd);
    } else {
        emit connectionError(UnknownServerTypeError);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::disconnectFromChat()
{
    if (m_chatbot) {
        m_chatbot->disconnectFromServer();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setupChatbot(ChatType type)
{
    if (type != FbChat && type != GTalkChat) {
        return;
    }

    m_chatbot = newChatbot(type);
    m_currentChatbotType = type;

    DefaultVirtualUser *virtualUser = new DefaultVirtualUser(m_rulesFile.chatbotId(), m_nlpEngine);
    virtualUser->setEvasives(getEvasives());

    m_chatbot->setVirtualUser(virtualUser);

    connectChatClientSignals();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::deleteCurrentChatbot()
{
    delete m_chatbot;
    m_chatbot = 0;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::refreshEvasivesToChatbot()
{
    if (m_chatbot && m_chatbot->virtualUser()) {
        DefaultVirtualUser *virtualUser =
                dynamic_cast<DefaultVirtualUser *>(m_chatbot->virtualUser());

        if (virtualUser) {
            virtualUser->setEvasives(getEvasives());
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::connectChatClientSignals()
{
    connect(m_chatbot, SIGNAL(connected()),    SIGNAL(connected()));
    connect(m_chatbot, SIGNAL(disconnected()), SIGNAL(disconnected()));
    // FIXME add method to remap Chatbot error codes to AppFacade error codes
    connect(m_chatbot, SIGNAL(error(int)),     SIGNAL(connectionError(int)));

    // Hmmm this doesn't look nice. TODO consider some refactoring to avoid cast
    connect(dynamic_cast<DefaultVirtualUser *>(m_chatbot->virtualUser()),
            SIGNAL(newConversationEntry(BE::Conversation::Entry)),
            SIGNAL(newConversationEntry(BE::Conversation::Entry)));
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Roster Lvk::BE::AppFacade::roster()
{
    Roster roster;

    if (m_chatbot) {
        roster = toBERoster(m_chatbot->roster());
    }

    return roster;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setBlackListRoster(const Roster &roster)
{
    if (m_chatbot) {
        m_chatbot->setBlackListRoster(toCAContactInfoList(roster));
    }
}

//--------------------------------------------------------------------------------------------------
// Chat history
//--------------------------------------------------------------------------------------------------

const Lvk::BE::Conversation & Lvk::BE::AppFacade::chatHistory()
{
    if (virtualUser()) {
        return virtualUser()->chatHistory();
    } else {
        static Conversation nullConversation;
        return nullConversation;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::clearChatHistory()
{
    if (virtualUser()) {
        virtualUser()->setChatHistory(Conversation());
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::clearChatHistory(const QDate &date, const QString &user)
{
    if (virtualUser()) {
        Conversation conv;
        foreach (const Conversation::Entry &entry, virtualUser()->chatHistory().entries()) {
            if (entry.from != user || entry.dateTime.date() != date) {
                conv.append(entry);
            }
        }
        virtualUser()->setChatHistory(conv);
    }
}


