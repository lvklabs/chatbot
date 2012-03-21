#ifndef LVK_BE_COREAPP_H
#define LVK_BE_COREAPP_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QPair>

#include "nlprule.h"
#include "conversation.h"

namespace Lvk
{

namespace Nlp
{
    class Engine;
}

namespace CA
{
    class Chatbot;
}

namespace BE
{

class Rule;


/**
 * \brief This class provides the core functionality of the application.
 */

class CoreApp : public QObject
{
    Q_OBJECT

public:

    CoreApp(QObject *parent = 0);

    CoreApp(Nlp::Engine *nlpEngine, QObject *parent = 0);

    ~CoreApp();

    bool load(const QString &filename);

    bool saveAs(const QString &filename);

    bool save();

    void close();

    Rule *rootRule();

    typedef QList< QPair<Rule *, int> > MatchList;

    QString getResponse(const QString &input, MatchList &matches);

    void refreshNlpEngine();

    enum ChatType { FbChat, GTalkChat };

    enum ConnectionError {
        SocketError,        ///< Error due to TCP socket
        KeepAliveError,     ///< Error due to no response to a keep alive
        XmppStreamError,    ///< Error due to XML stream
        UnknownServerError  ///< Error due to unknown server
    };

    void connectToChat(ChatType chatType, const QString &user, const QString &passwd);

    void disconnectFromChat();

    const Conversation &conversationHistory();

signals:

    void connected();
    void disconnected();
    void connectionError(int err);

    void newConversationEntry(const BE::Conversation::Entry &entry);

private:
    CoreApp(CoreApp&);
    CoreApp& operator=(CoreApp&);

    QString m_filename;
    Rule *m_rootRule;
    Nlp::Engine *m_nlpEngine;
    Nlp::RuleId m_nextRuleId;
    QHash<Nlp::RuleId, Rule *> m_rulesHash;
    QStringList m_evasives;
    CA::Chatbot *m_chatbot;
    ChatType m_currentChatbotType;

    void buildNlpRulesOf(Rule* parentRule, Nlp::RuleList &nlpRules);

    void createChatbot(ChatType type);
    void deleteCurrentChatbot();
    void setEvasivesToChatbot(const QStringList &evasives);
    void connectChatClientSignals();
};

} // namespace BE

} // namespace Lvk

#endif // LVK_BE_COREAPP_H
