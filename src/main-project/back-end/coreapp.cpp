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

#define MAGIC_NUMBER            (('l'<<0) | ('v'<<8) | ('k'<<16) | ('\0'<<24))
#define FILE_FORMAT_VERSION     1

//--------------------------------------------------------------------------------------------------
// Constructors & destructor
//--------------------------------------------------------------------------------------------------

Lvk::BE::CoreApp::CoreApp(QObject *parent /*= 0*/)
    : QObject(parent),
      m_rootRule(0),
      m_nlpEngine(new Lvk::Nlp::ExactMatchEngine()),
      m_nextRuleId(0),
      m_chatbot(0)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::CoreApp::CoreApp(Nlp::Engine *nlpEngine, QObject *parent /*= 0*/)
    : QObject(parent),
      m_rootRule(0),
      m_nlpEngine(nlpEngine),
      m_nextRuleId(0),
      m_chatbot(0)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::CoreApp::~CoreApp()
{
    delete m_chatbot;
    delete m_rootRule;
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

    bool success = true;

    QFile file(m_filename);

    if (file.exists() && file.open(QFile::ReadOnly)) {
        success = read(file);
    } else {
        loadDefaultRules();
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

    delete m_rootRule;
    m_rootRule = 0;

    m_filename = "";

    m_nextRuleId = 0;

    m_rulesHash.clear();

    m_evasives.clear();
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::read(QFile &file)
{
    QDataStream istream(&file);

    quint32 magicNumber;
    istream >> magicNumber;

    if (magicNumber != MAGIC_NUMBER) {
        return false;
    }

    qint32 version;
    istream >> version;

    if (version < FILE_FORMAT_VERSION) {
        return false;
    }

    m_rootRule = new Rule();

    istream >> *m_rootRule;

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::CoreApp::write(QFile &file)
{
    QDataStream ostream(&file);

    ostream << (quint32)MAGIC_NUMBER;
    ostream << (qint32)FILE_FORMAT_VERSION;

    ostream.setVersion(QDataStream::Qt_4_0);

    if (m_rootRule) {
        ostream << *m_rootRule;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
// Nlp Engine methods
//--------------------------------------------------------------------------------------------------


Lvk::BE::Rule * Lvk::BE::CoreApp::rootRule()
{
    return m_rootRule;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::CoreApp::getResponse(const QString &input, MatchList  &matches)
{
    matches.clear();

    if (!m_nlpEngine) {
        return "ERROR: NLP Engine Not implemented";
    }

    Nlp::Engine::MatchList nlpRulesMatched;
    QString response = m_nlpEngine->getResponse(input, nlpRulesMatched);

    if (!nlpRulesMatched.isEmpty()) {
        Nlp::RuleId ruleId = nlpRulesMatched.first().first;
        int inputNumber = nlpRulesMatched.first().second;
        matches.append(qMakePair(m_rulesHash[ruleId], inputNumber));

        return response;
    } else if (!m_evasives.isEmpty()) {
        return m_evasives[Common::Random::getInt(0, m_evasives.size() - 1)];
    } else {
        return QObject::tr("Sorry, I don't understand that");
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::refreshNlpEngine()
{
    m_nextRuleId = 0;
    m_rulesHash.clear();
    m_evasives.clear();

    if (m_nlpEngine) {
        Nlp::RuleList nlpRules;

        if (m_rootRule) {
            buildNlpRulesOf(m_rootRule, nlpRules);
        }

        m_nlpEngine->setRules(nlpRules);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::CoreApp::buildNlpRulesOf(BE::Rule *parentRule, Nlp::RuleList &nlpRules)
{
    if (!parentRule) {
        return;
    }

    for (int i = 0; i < parentRule->childCount(); ++i) {
        BE::Rule *child = parentRule->child(i);
        if (child->type() == Rule::OrdinaryRule) {
            m_rulesHash[m_nextRuleId] = child;
            Nlp::Rule nlpRule(m_nextRuleId++, child->input(), child->output());
            nlpRules.append(nlpRule);
        } else if (child->type() == Rule::EvasiveRule) {
            m_evasives = child->output(); // Design decision: It can exist only one evasive rule
            setEvasivesToChatbot(child->output());
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
    virtualUser->setEvasives(m_evasives);

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

void Lvk::BE::CoreApp::setEvasivesToChatbot(const QStringList &evasives)
{
    if (m_chatbot && m_chatbot->virtualUser()) {
        DefaultVirtualUser *virtualUser =
                dynamic_cast<DefaultVirtualUser *>(m_chatbot->virtualUser());

        if (virtualUser) {
            virtualUser->setEvasives(evasives);
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

void Lvk::BE::CoreApp::loadDefaultRules()
{
    m_rootRule = new BE::Rule();

    BE::Rule *catGreetings    = new BE::Rule("Saludos");
    BE::Rule *catPersonalInfo = new BE::Rule("Informacion Personal");

    catGreetings->setType(BE::Rule::ContainerRule);
    catPersonalInfo->setType(BE::Rule::ContainerRule);

    m_rootRule->appendChild(catGreetings);
    m_rootRule->appendChild(catPersonalInfo);

    QStringList rule1InputList;
    QStringList rule1OutputList;
    rule1InputList << QString("Hola") << QString("Holaa") << QString("Holaaa") << QString("Hola *");
    rule1OutputList << QString("Hola!");

    QStringList rule2InputList;
    QStringList rule2OutputList;
    rule2InputList << QString("Buenas") << QString("Buen dia") << QString("Buenas tardes") << QString("Que haces *");
    rule2OutputList << QString("Buenas, Como estas?");

    QStringList rule3InputList;
    QStringList rule3OutputList;
    rule3InputList << QString("Cual es tu nombre?") << QString("Como te llamas?") << QString("Quien sos?");
    rule3OutputList << QString("Andres");

    BE::Rule * rule1 = new BE::Rule("", rule1InputList, rule1OutputList);
    BE::Rule * rule2 = new BE::Rule("", rule2InputList, rule2OutputList);
    BE::Rule * rule3 = new BE::Rule("", rule3InputList, rule3OutputList);

    catGreetings->appendChild(rule1);
    catGreetings->appendChild(rule2);
    catPersonalInfo->appendChild(rule3);

    // evasives

    BE::Rule *evasives    = new BE::Rule("Evasivas");
    evasives->setType(BE::Rule::EvasiveRule);

    m_rootRule->appendChild(evasives);

    QStringList evasivesOutputList;
    evasivesOutputList << QString("Perdon, no entiendo")
                       << QString("No entiendo, puedes explicarlo de otra manera?");

    evasives->setOutput(evasivesOutputList);
}


