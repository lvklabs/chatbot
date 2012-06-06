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

#ifndef LVK_BE_APPFACADE_H
#define LVK_BE_APPFACADE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QPair>
#include <QVariant>
#include <QSet>

#include <memory>

#include "nlprule.h"
#include "conversation.h"
#include "roster.h"
#include "target.h"

class QFile;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

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

/// \ingroup Lvk
/// \addtogroup BE
/// @{

class Rule;


/**
 * \brief The AppFacade class provides the core functionality of the application.
 *
 * The AppFacade class provides a unified and simplified interface to several subsystems
 * of the application that makes the subsystems easier to use.
 *
 * The most important subsystems are the NLP engine and the chat adapters.
 */

class AppFacade : public QObject
{
    Q_OBJECT

public:

    AppFacade(QObject *parent = 0);

    AppFacade(Nlp::Engine *nlpEngine, QObject *parent = 0);

    ~AppFacade();

    bool load(const QString &filename, bool create = true);

    bool saveAs(const QString &filename);

    bool save();

    bool hasUnsavedChanges() const;

    void close();

    void setMetadata(const QString &key, const QVariant &value);

    const QVariant &metadata(const QString &key) const;

    bool importRules(const QString &inputFile);

    bool importRules(BE::Rule *container, const QString &inputFile);

    bool exportRules(const Rule *container, const QString &outputFile);

    bool mergeRules(Rule *container);

    Rule *rootRule();

    Rule *evasivesRule();

    typedef QList< QPair<const Rule *, int> > MatchList;

    QString getResponse(const QString &input, const QString &target, MatchList &matches) const;

    QString getTestUserResponse(const QString &input, MatchList &matches) const;

    void refreshNlpEngine();

    enum ChatType { FbChat, GTalkChat };

    enum ConnectionError {
        SocketError,        ///< Error due to TCP socket
        KeepAliveError,     ///< Error due to no response to a keep alive
        XmppStreamError,    ///< Error due to XML stream
        UnknownServerError  ///< Error due to unknown server
    };

    void verifyAccount(ChatType accountType, const QString &user, const QString &passwd);

    void cancelVerifyAccount();

    void connectToChat(ChatType accountType, const QString &user, const QString &passwd);

    void disconnectFromChat();

    Roster roster();

    void setBlackListRoster(const Roster &roster);

    const Conversation &conversationHistory();

signals:

    void accountOk();

    void accountError(int err);

    void connected();

    void disconnected();

    void connectionError(int err);

    void newConversationEntry(const BE::Conversation::Entry &entry);

private slots:

    void onAccountOk();

    void onAccountError(int err);

private:
    AppFacade(AppFacade&);
    AppFacade& operator=(AppFacade&);

    typedef QHash<QString, QVariant> FileMetadata;

    QString m_filename;
    FileMetadata m_metadata;
    bool m_metadataUnsaved;
    std::auto_ptr<Rule> m_rootRule;
    Rule *m_evasivesRule;
    Nlp::Engine *m_nlpEngine;
    Nlp::RuleId m_nextRuleId;
    QHash<Nlp::RuleId, const Rule *> m_rulesHash;
    CA::Chatbot *m_chatbot;
    QString m_chatbotId;
    CA::Chatbot *m_tmpChatbot;
    ChatType m_currentChatbotType;
    bool m_isFirstTime;
    QSet<QString> m_targets;

    void markAsSaved();

    void buildNlpRulesOf(const Rule* parentRule, Nlp::RuleList &nlpRules);

    void storeTargets(const TargetList &targets);

    QStringList getEvasives() const;

    void setupChatbot(ChatType type);
    void deleteCurrentChatbot();
    void refreshEvasivesToChatbot();
    void connectChatClientSignals();

    bool read(QFile &file);
    bool write(QFile &file);
    bool loadDefaultRules();
    bool loadDefaultFirstTimeRules();
};

/// @}

} // namespace BE

/// @}

} // namespace Lvk

#endif // LVK_BE_APPFACADE_H
