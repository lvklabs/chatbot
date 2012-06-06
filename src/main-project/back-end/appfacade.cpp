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

#include <QFile>
#include <QUuid>
#include <QDataStream>

#include <iostream>

#define CRF_MAGIC_NUMBER            (('c'<<0) | ('r'<<8) | ('f'<<16) | ('\0'<<24))
#define CRF_FILE_FORMAT_VERSION     2

#define CEF_MAGIC_NUMBER            (('c'<<0) | ('e'<<8) | ('f'<<16) | ('\0'<<24))
#define CEF_FILE_FORMAT_VERSION     2

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

Lvk::Nlp::Rule makeNlpRule(const Lvk::BE::Rule *rule, int id)
{
    Lvk::Nlp::Rule nlpRule(id, rule->input(), rule->output());

    QStringList targets;
    foreach (const Lvk::BE::Target &t, rule->target()) {
        targets.append(t.username);
    }

    nlpRule.setTarget(targets);

    return nlpRule;
}

//--------------------------------------------------------------------------------------------------

QString nullChatbotId()
{
    static QString nullId = "00000000-0000-0000-0000-000000000000";

    return nullId;
}

//--------------------------------------------------------------------------------------------------

QString newChatbotId()
{
    QString id = QUuid::createUuid().toString().mid(1, 36);

    return id;
}

} // namespace


//--------------------------------------------------------------------------------------------------
// Constructors & destructor
//--------------------------------------------------------------------------------------------------

Lvk::BE::AppFacade::AppFacade(QObject *parent /*= 0*/)
    : QObject(parent),
      m_rootRule(new Rule()),
      m_evasivesRule(0),
      m_nlpEngine(new Lvk::BE::DefaultEngine(new Lvk::BE::DefaultSanitizer())),
      m_nextRuleId(0),
      m_chatbot(0),
      m_chatbotId(nullChatbotId()),
      m_isFirstTime(true)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::AppFacade::AppFacade(Nlp::Engine *nlpEngine, QObject *parent /*= 0*/)
    : QObject(parent),
      m_rootRule(new Rule()),
      m_evasivesRule(0),
      m_nlpEngine(nlpEngine),
      m_nextRuleId(0),
      m_chatbot(0),
      m_chatbotId(nullChatbotId()),
      m_isFirstTime(true)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::AppFacade::~AppFacade()
{
    delete m_chatbot;
    delete m_nlpEngine;
}

//--------------------------------------------------------------------------------------------------
// Load, save, save as, close file
//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::load(const QString &filename, bool create /*= true*/)
{
    if (!m_filename.isEmpty()) {
        close();
    }

    m_filename = filename;

    bool success = false;

    QFile file(m_filename);

    if (file.exists()) {
        if (file.open(QFile::ReadOnly)) {
            success = read(file);
        }
    } else if (create) {
        if (m_isFirstTime) {
            success = loadDefaultFirstTimeRules();
        } else {
            success = loadDefaultRules();
        }
    }

    if (success) {
        markAsSaved();
    } else {
        close();
    }

    refreshNlpEngine();

    return success;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::save()
{
    bool success = false;

    QFile file(m_filename);

    if (file.open(QFile::WriteOnly)) {
        success = write(file);
    }

    if (success) {
        markAsSaved();
    }

    return success;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::saveAs(const QString &filename)
{
    QString filenameBak = m_filename;
    QString chatbotIdBak = m_chatbotId;

    deleteCurrentChatbot();

    m_filename = filename;
    m_chatbotId = newChatbotId();

    bool success = save();

    if (!success) {
        m_filename = filenameBak;
        m_chatbotId = chatbotIdBak;
    }

    return success;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::hasUnsavedChanges() const
{
    for (Rule::iterator it = m_rootRule->begin(); it != m_rootRule->end(); ++it) {
        if ((*it)->status() == Rule::Unsaved) {
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::markAsSaved()
{
    for (Rule::iterator it = m_rootRule->begin(); it != m_rootRule->end(); ++it) {
        (*it)->setStatus(Rule::Saved);
    }
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

    m_chatbotId = nullChatbotId();

    m_evasivesRule = 0;

    m_rootRule = std::auto_ptr<Rule>(new Rule());

    loadDefaultRules();

    m_filename = "";

    m_nextRuleId = 0;

    m_rulesHash.clear();

    m_targets.clear();

    markAsSaved();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::read(QFile &file)
{
    QDataStream istream(&file);

    quint32 magicNumber;
    istream >> magicNumber;

    quint32 version;
    istream >> version;

    if (magicNumber != CRF_MAGIC_NUMBER) {
        return false;
    }

    if (version > CRF_FILE_FORMAT_VERSION) {
        return false;
    }

    m_rootRule = std::auto_ptr<Rule>(new Rule());

    istream >> m_chatbotId;
    istream >> *m_rootRule;

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::write(QFile &file)
{
    QDataStream ostream(&file);

    ostream.setVersion(QDataStream::Qt_4_7);

    ostream << (quint32)CRF_MAGIC_NUMBER;
    ostream << (quint32)CRF_FILE_FORMAT_VERSION;
    ostream << m_chatbotId;
    ostream << *m_rootRule;

    return true;
}

//--------------------------------------------------------------------------------------------------
// Import/Export
//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::importRules(const QString &inputFile)
{
    BE::Rule container;

    return importRules(&container, inputFile) && mergeRules(&container);
}

bool Lvk::BE::AppFacade::importRules(BE::Rule *container, const QString &inputFile)
{
    QFile file(inputFile);

    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QDataStream istream(&file);

    quint32 magicNumber;
    istream >> magicNumber;

    quint32 version;
    istream >> version;

    if (magicNumber != CEF_MAGIC_NUMBER) {
        return false;
    }

    if (version > CEF_FILE_FORMAT_VERSION) {
        return false;
    }

    istream >> *container;

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::exportRules(const BE::Rule *container, const QString &outputFile)
{
    QFile file(outputFile);

    if (!file.open(QFile::WriteOnly)) {
        return false;
    }

    QDataStream ostream(&file);

    ostream.setVersion(QDataStream::Qt_4_7);

    ostream << (quint32)CEF_MAGIC_NUMBER;
    ostream << (quint32)CEF_FILE_FORMAT_VERSION;
    ostream << *container;

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::mergeRules(BE::Rule *container)
{
    foreach (Lvk::BE::Rule *rule, container->children()) {
        switch (rule->type()) {
        case Rule::ContainerRule:
            m_rootRule->appendChild(new BE::Rule(*rule, true));
            break;
        case Rule::EvasiveRule:
            if (m_evasivesRule) {
                m_evasivesRule->output().append(rule->output());
            } else {
                m_evasivesRule = new BE::Rule(*rule, true);
                m_rootRule->appendChild(m_evasivesRule);
            }
            break;
        case Rule::OrdinaryRule:
            // not supported
            // TODO log warning
            break;
        }
    }

    refreshNlpEngine();

    return true;
}

//--------------------------------------------------------------------------------------------------
// Nlp Engine methods
//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::AppFacade::rootRule()
{
    return m_rootRule.get();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::AppFacade::evasivesRule()
{
    return m_evasivesRule;
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::BE::AppFacade::getEvasives() const
{
    return m_evasivesRule ? const_cast<const BE::Rule*>(m_evasivesRule)->output() : QStringList();
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::AppFacade::getTestUserResponse(const QString &input, MatchList &matches) const
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

    if (!m_nlpEngine) {
        return "ERROR: NLP Engine Not implemented";
    }

    Nlp::Engine::MatchList nlpRulesMatched;
    QString response = m_nlpEngine->getResponse(input, target, nlpRulesMatched);

    if (!nlpRulesMatched.isEmpty()) {
        Nlp::RuleId ruleId = nlpRulesMatched.first().first;
        int inputNumber = nlpRulesMatched.first().second;
        matches.append(qMakePair(m_rulesHash[ruleId], inputNumber));
    } else {
        QStringList evasives = getEvasives();
        response = !evasives.isEmpty() ?
                    evasives[Cmn::Random::getInt(0, evasives.size() - 1)] : "";
    }

    return response;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::refreshNlpEngine()
{
    m_evasivesRule = 0;
    m_nextRuleId = 0;
    m_rulesHash.clear();
    m_targets.clear();

    if (m_nlpEngine) {
        Nlp::RuleList nlpRules;
        buildNlpRulesOf(m_rootRule.get(), nlpRules);
        m_nlpEngine->setRules(nlpRules);
        refreshEvasivesToChatbot();
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
        if (child->type() == Rule::OrdinaryRule) {
            storeTargets(child->target());
            m_rulesHash[m_nextRuleId] = child;
            nlpRules.append(makeNlpRule(child, m_nextRuleId++));
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

void Lvk::BE::AppFacade::connectToChat(Lvk::BE::AppFacade::ChatType type, const QString &user,
                                     const QString &passwd)
{
    if (m_chatbot && m_currentChatbotType != type) {
        m_chatbot->disconnectFromServer();

        deleteCurrentChatbot();
    }

    if (!m_chatbot) {
        createChatbot(type);
    }

    if (type == FbChat) {
        dynamic_cast<CA::FbChatbot *>(m_chatbot)->connectToServer(user, passwd);
    } else if (type == GTalkChat) {
        dynamic_cast<CA::GTalkChatbot *>(m_chatbot)->connectToServer(user, passwd);
    } else {
        // TODO handle error
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

void Lvk::BE::AppFacade::createChatbot(ChatType type)
{
    if (type != FbChat && type != GTalkChat) {
        return;
    }

    if (type == FbChat) {
        m_chatbot = new CA::FbChatbot();
    } else if (type == GTalkChat) {
        m_chatbot = new CA::GTalkChatbot();
    }

    m_currentChatbotType = type;

    DefaultVirtualUser *virtualUser = new DefaultVirtualUser(m_chatbotId, m_nlpEngine);
    virtualUser->setEvasives(getEvasives());

    m_chatbot->setVirtualUser(virtualUser);

    connectChatClientSignals();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::deleteCurrentChatbot()
{
    delete m_chatbot;   // TODO consider using deleteLater()
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
        CA::ContactInfoList infoList = m_chatbot->roster();
        foreach (const CA::ContactInfo &info, infoList) {
            roster.append(RosterItem(info.username, info.fullname));
        }
    }

    return roster;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::AppFacade::setBlackListRoster(const Roster &roster)
{
    if (m_chatbot) {
        CA::ContactInfoList infoList;
        foreach (const RosterItem &item, roster) {
            infoList.append(CA::ContactInfo(item.username, item.username));
        }
        m_chatbot->setBlackListRoster(infoList);
    }
}

//--------------------------------------------------------------------------------------------------

const Lvk::BE::Conversation & Lvk::BE::AppFacade::conversationHistory()
{
    if (!m_chatbot) {
        createChatbot(FbChat);
    }

    DefaultVirtualUser *virtualUser =
            dynamic_cast<DefaultVirtualUser *>(m_chatbot->virtualUser());

    if (virtualUser) {
        return virtualUser->getConversationHistory();
    } else {
        static Conversation nullConversation;
        return nullConversation;
    }
}

//--------------------------------------------------------------------------------------------------
// Misc
//--------------------------------------------------------------------------------------------------
// TODO localize

bool Lvk::BE::AppFacade::loadDefaultFirstTimeRules()
{
    m_isFirstTime = false;

    m_rootRule = std::auto_ptr<Rule>(new BE::Rule(tr("Rules")));

    BE::Rule *catGreetings    = new BE::Rule("Saludos");

    catGreetings->setType(BE::Rule::ContainerRule);

    m_rootRule->appendChild(catGreetings);

    QStringList rule1InputList;
    QStringList rule1OutputList;
    rule1InputList  << QString("Hola") << QString("Holaa") << QString("Holaaa")
                    << QString("Hola *");
    rule1OutputList << QString("Hola!") << QString("Que haces che!");

    QStringList rule2InputList;
    QStringList rule2OutputList;
    rule2InputList  << QString("Buenas") << QString("Buen dia") << QString("Buenas tardes")
                    << QString("Que haces *");
    rule2OutputList << QString("Buenas, Como estas?");

    BE::Rule * rule1 = new BE::Rule("", rule1InputList, rule1OutputList);
    BE::Rule * rule2 = new BE::Rule("", rule2InputList, rule2OutputList);

    catGreetings->appendChild(rule1);
    catGreetings->appendChild(rule2);

    // evasives

    BE::Rule *evasives    = new BE::Rule("Si no entiende");
    evasives->setType(BE::Rule::EvasiveRule);

    m_rootRule->appendChild(evasives);

    m_evasivesRule = evasives;

    QStringList evasivesOutputList;
    evasivesOutputList << QString("Perdon, no entiendo")
                       << QString("No entiendo, puedes explicarlo de otra manera?");

    evasives->setOutput(evasivesOutputList);

    m_chatbotId = newChatbotId();

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::AppFacade::loadDefaultRules()
{
    m_rootRule = std::auto_ptr<Rule>(new BE::Rule(tr("Rules")));

    // initial category

    BE::Rule *cat1  = new BE::Rule("Categoria");

    cat1->setType(BE::Rule::ContainerRule);

    m_rootRule->appendChild(cat1);

    BE::Rule * rule1 = new BE::Rule("");

    cat1->appendChild(rule1);

    // evasives

    BE::Rule *evasives    = new BE::Rule("Si no entiende");
    evasives->setType(BE::Rule::EvasiveRule);

    m_rootRule->appendChild(evasives);

    m_evasivesRule = evasives;

    QStringList evasivesOutputList;

    evasives->setOutput(evasivesOutputList);

    m_chatbotId = newChatbotId();

    return true;
}






