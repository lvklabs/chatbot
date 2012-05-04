/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "coreapp.h"
#include "rule.h"
#include "nlpengine.h"
#include "nlprule.h"
#include "exactmatchengine.h"
#include "random.h"
#include "fbchatbot.h"
#include "gtalkchatbot.h"
#include "defaultvirtualuser.h"

#include <QFile>
#include <QDataStream>

#include <iostream>

#define CRF_MAGIC_NUMBER            (('c'<<0) | ('r'<<8) | ('f'<<16) | ('\0'<<24))
#define CRF_FILE_FORMAT_VERSION     2

#define CEF_MAGIC_NUMBER            (('c'<<0) | ('e'<<8) | ('f'<<16) | ('\0'<<24))
#define CEF_FILE_FORMAT_VERSION     2

// Backward compatibility: Magic number for file format version 1
#define CRF_MAGIC_NUMBER_V1         (('l'<<0) | ('v'<<8) | ('k'<<16) | ('\0'<<24))



//--------------------------------------------------------------------------------------------------
// Constructors & destructor
//--------------------------------------------------------------------------------------------------

Lvk::BE::CoreApp::CoreApp(QObject *parent /*= 0*/)
    : QObject(parent),
      m_rootRule(new Rule()),
      m_evasivesRule(0),
      m_nlpEngine(new Lvk::Nlp::ExactMatchEngine()),
      m_nextRuleId(0),
      m_chatbot(0),
      m_isFirstTime(true)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::CoreApp::CoreApp(Nlp::Engine *nlpEngine, QObject *parent /*= 0*/)
    : QObject(parent),
      m_rootRule(new Rule()),
      m_evasivesRule(0),
      m_nlpEngine(nlpEngine),
      m_nextRuleId(0),
      m_chatbot(0),
      m_isFirstTime(true)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::CoreApp::~CoreApp()
{
    delete m_chatbot;
    delete m_nlpEngine;
}

//--------------------------------------------------------------------------------------------------
// Load, save, save as, close file
//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::load(const QString &filename)
{
    if (!m_filename.isEmpty()) {
        close();
    }

    m_filename = filename;

    bool success = false;

    QFile file(m_filename);

    if (file.exists() && file.open(QFile::ReadOnly)) {
        success = read(file);
    } else if (m_isFirstTime) {
        success = loadDefaultFirstTimeRules();
    } else {
        success = loadDefaultRules();
    }

    if (success) {
        markAsSaved();
    }

    refreshNlpEngine();

    return success;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::save()
{
    bool success = true;

    QFile file(m_filename);

    if (file.open(QFile::WriteOnly)) {
        success = write(file);
    } else {
        success = false;
    }

    if (success) {
        markAsSaved();
    }

    return success;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::saveAs(const QString &filename)
{
    m_filename = filename;

    save();

    return false;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::hasUnsavedChanges() const
{
    for (Rule::/* FIXME const_*/iterator it = m_rootRule->begin(); it != m_rootRule->end(); ++it) {
        if ((*it)->status() == Rule::Unsaved) {
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::markAsSaved()
{
    for (Rule::iterator it = m_rootRule->begin(); it != m_rootRule->end(); ++it) {
        (*it)->setStatus(Rule::Saved);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::close()
{
    if (m_nlpEngine) {
        m_nlpEngine->setRules(Nlp::RuleList());
    }

    if (m_chatbot) {
        m_chatbot->disconnectFromServer();
        delete m_chatbot;
        m_chatbot = 0;
    }

    m_evasivesRule = 0;

    m_rootRule = std::auto_ptr<Rule>(new Rule());

    loadDefaultRules();

    m_filename = "";

    m_nextRuleId = 0;

    m_rulesHash.clear();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::read(QFile &file)
{
    QDataStream istream(&file);

    quint32 magicNumber;
    istream >> magicNumber;

    quint32 version;
    istream >> version;

    if (version == 1 && magicNumber != CRF_MAGIC_NUMBER_V1) {
        return false;
    }
    if (version != 1 && magicNumber != CRF_MAGIC_NUMBER) {
        return false;
    }

    if (version > CRF_FILE_FORMAT_VERSION) {
        return false;
    }

    m_rootRule = std::auto_ptr<Rule>(new Rule());

    istream >> *m_rootRule;

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::write(QFile &file)
{
    QDataStream ostream(&file);

    ostream.setVersion(QDataStream::Qt_4_0);

    ostream << (quint32)CRF_MAGIC_NUMBER;
    ostream << (quint32)CRF_FILE_FORMAT_VERSION;
    ostream << *m_rootRule;

    return true;
}

//--------------------------------------------------------------------------------------------------
// Import/Export
//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::importRules(const QString &inputFile)
{
    BE::Rule container;

    return importRules(&container, inputFile) && mergeRules(&container);
}

bool Lvk::BE::CoreApp::importRules(BE::Rule *container, const QString &inputFile)
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

bool Lvk::BE::CoreApp::exportRules(const BE::Rule *container, const QString &outputFile)
{
    QFile file(outputFile);

    if (!file.open(QFile::WriteOnly)) {
        return false;
    }

    QDataStream ostream(&file);

    ostream.setVersion(QDataStream::Qt_4_0);

    ostream << (quint32)CEF_MAGIC_NUMBER;
    ostream << (quint32)CEF_FILE_FORMAT_VERSION;
    ostream << *container;

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::mergeRules(BE::Rule *container)
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

Lvk::BE::Rule * Lvk::BE::CoreApp::rootRule()
{
    return m_rootRule.get();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::CoreApp::evasivesRule()
{
    return m_evasivesRule;
}

//--------------------------------------------------------------------------------------------------

QStringList Lvk::BE::CoreApp::getEvasives() const
{
    return m_evasivesRule ? static_cast<const BE::Rule*>(m_evasivesRule)->output() : QStringList();
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::CoreApp::getResponse(const QString &input, MatchList &matches) const
{
    matches.clear();

    if (!m_nlpEngine) {
        return "ERROR: NLP Engine Not implemented";
    }

    Nlp::Engine::MatchList nlpRulesMatched;
    QString response = m_nlpEngine->getResponse(input, nlpRulesMatched);


    // TODO Refactor return

    if (!nlpRulesMatched.isEmpty()) {
        Nlp::RuleId ruleId = nlpRulesMatched.first().first;
        int inputNumber = nlpRulesMatched.first().second;
        matches.append(qMakePair(m_rulesHash[ruleId], inputNumber));

        return response;
    } else {
        QStringList evasives = getEvasives();
        if (!evasives.isEmpty()) {
            return evasives[Common::Random::getInt(0, evasives.size() - 1)];
        } else {
            return "";
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::refreshNlpEngine()
{
    m_evasivesRule = 0;
    m_nextRuleId = 0;
    m_rulesHash.clear();

    if (m_nlpEngine) {
        Nlp::RuleList nlpRules;
        buildNlpRulesOf(m_rootRule.get(), nlpRules);
        m_nlpEngine->setRules(nlpRules);
        refreshEvasivesToChatbot();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::buildNlpRulesOf(const BE::Rule *parentRule, Nlp::RuleList &nlpRules)
{
    if (!parentRule) {
        return;
    }

    for (int i = 0; i < parentRule->childCount(); ++i) {
        const BE::Rule *child = parentRule->child(i);
        if (child->type() == Rule::OrdinaryRule) {
            m_rulesHash[m_nextRuleId] = child;
            Nlp::Rule nlpRule(m_nextRuleId++, child->input(), child->output());
            nlpRules.append(nlpRule);
        } else if (child->type() == Rule::EvasiveRule) {
            m_evasivesRule = (BE::Rule *)child; // FIXME fix cast
        } else if (child->type() == BE::Rule::ContainerRule) {
            buildNlpRulesOf(child, nlpRules);
        }
    }
}

//--------------------------------------------------------------------------------------------------
// Chat methods
//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::connectToChat(Lvk::BE::CoreApp::ChatType type, const QString &user,
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

void Lvk::BE::CoreApp::disconnectFromChat()
{
    if (m_chatbot) {
        m_chatbot->disconnectFromServer();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::createChatbot(ChatType type)
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

    DefaultVirtualUser *virtualUser = new DefaultVirtualUser(m_nlpEngine);
    virtualUser->setEvasives(getEvasives());

    m_chatbot->setVirtualUser(virtualUser);

    connectChatClientSignals();
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::deleteCurrentChatbot()
{
    if (!m_chatbot) {
        return;
    }

    delete m_chatbot;   // TODO consider using deleteLater()
    m_chatbot = 0;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::refreshEvasivesToChatbot()
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

void Lvk::BE::CoreApp::connectChatClientSignals()
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

const Lvk::BE::Conversation & Lvk::BE::CoreApp::conversationHistory()
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

bool Lvk::BE::CoreApp::loadDefaultFirstTimeRules()
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

    QStringList evasivesOutputList;
    evasivesOutputList << QString("Perdon, no entiendo")
                       << QString("No entiendo, puedes explicarlo de otra manera?");

    evasives->setOutput(evasivesOutputList);

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::loadDefaultRules()
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

    QStringList evasivesOutputList;

    evasives->setOutput(evasivesOutputList);

    return true;
}





