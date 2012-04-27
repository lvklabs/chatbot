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

#ifndef LVK_BE_COREAPP_H
#define LVK_BE_COREAPP_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QPair>

#include "nlprule.h"
#include "conversation.h"

class QFile;

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

    bool hasUnsavedChanges() const;

    void close();

    Rule *rootRule();

    typedef QList< QPair<const Rule *, int> > MatchList;

    QString getResponse(const QString &input, MatchList &matches) const;

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
    QHash<Nlp::RuleId, const Rule *> m_rulesHash;
    QStringList m_evasives;
    CA::Chatbot *m_chatbot;
    ChatType m_currentChatbotType;
    bool m_isFirstTime;

    void markAsSaved();

    void buildNlpRulesOf(const Rule* parentRule, Nlp::RuleList &nlpRules);

    void createChatbot(ChatType type);
    void deleteCurrentChatbot();
    void setEvasivesToChatbot(const QStringList &evasives);
    void connectChatClientSignals();

    bool read(QFile &file);
    bool write(QFile &file);
    bool loadDefaultRules();
    bool loadDefaultFirstTimeRules();
};

} // namespace BE

} // namespace Lvk

#endif // LVK_BE_COREAPP_H
