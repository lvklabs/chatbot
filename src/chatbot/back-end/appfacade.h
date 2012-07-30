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
#include <QPair>
#include <QVariant>
#include <QSet>

#include "chatbotrulesfile.h"
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

class Rule;
class DefaultVirtualUser;

/// \ingroup Lvk
/// \addtogroup BE
/// @{

/**
 * \brief The AppFacade class provides the core functionality of the application.
 *
 * The AppFacade class provides a unified and simplified interface to several subsystems
 * of the application that makes the subsystems easier to use.
 *
 * The most important subsystems are the Chatbot Rules files, the NLP engine and the chat adapters.
 */

class AppFacade : public QObject
{
    Q_OBJECT

public:

    /**
     * Constructs an AppFacade object with parent object \a parent.
     */
    AppFacade(QObject *parent = 0);

    /**
     * Constructs an AppFacade object with NLP engine \a nlpEngine and parent object \a parent.
     */
    AppFacade(Nlp::Engine *nlpEngine, QObject *parent = 0);

    /**
     * Destroys the object. Closes the current file and disconnect from chat server if necessary.
     */
    ~AppFacade();

    /**
     * Loads \a filename. Filename must be a valid chabot file or the empty string.
     * If \a filename is empty it creates a new chatbot file with the default initial rules.
     * Returns true on success. Otherwise; false.
     */
    bool load(const QString &filename);

    /**
     * Saves the current file with a new \a filename.
     * Returns true on success. Otherwise; false.
     */
    bool saveAs(const QString &filename);

    /**
     * Saves all the changes made.
     * Returns true on success. Otherwise; false.
     */
    bool save();

    /**
     * Returns true if there are unsaved changes in the current file. Otherwise; false.
     */
    bool hasUnsavedChanges() const;

    /**
     * Closes the current file.
     */
    void close();

    /**
     * Saves \a value with \a key in the metadata section of the current file.
     */
    void setMetadata(const QString &key, const QVariant &value);

    /**
     * Returns the \a value of \a key in the metadata section of the current file.
     * If the key is not found, returns QVariant().
     */
    const QVariant &metadata(const QString &key) const;

    /**
     * Imports rules from the filename \a inputFile.
     * Returns true on success. Otherwise; false.
     */
    bool importRules(const QString &inputFile);

    /**
     * Imports rules from the filename \a inputFile into \a container.
     * Returns true on success. Otherwise; false.
     */
    bool importRules(BE::Rule *container, const QString &inputFile);

    /**
     * Exports rules in \a container to file \a outputFile.
     * Returns true on success. Otherwise; false.
     */
    bool exportRules(const Rule *container, const QString &outputFile);

    /**
     * Merges rules in \a container with the current rules.
     * Returns true on success. Otherwise; false.
     */
    bool mergeRules(Rule *container);

    /**
     * Returns the root rule.
     */
    Rule *rootRule();

    /**
     * Returns the evasive rule.
     */
    Rule *evasivesRule();

    /**
     * Provides a list of pairs (Rule Id, Input index) used to store which rules has matched with
     * which rule input.
     *
     * \see getResponse, getTestUserResponse
     */
    typedef QList< QPair<quint64, int> > MatchList;

    /**
     * Gets a response for the given \a input and \a target.
     *
     * Returns the response if there is a match and \a matches contains
     * the list of rules and inputs that has matched. If the list contains two or more
     * objects it means there was recursion. Otherwise; returns an empty string and \a matches
     * is empty.
     */
    QString getResponse(const QString &input, const QString &target, MatchList &matches) const;

    /**
     * Gets a response for the given \a input ignoring target rules.
     *
     * Returns the response if there is a match and \a matches contains
     * the list of rules and inputs that has matched. If the list contains two or more
     * objects it means there was recursion. Otherwise; returns an empty string and \a matches
     * is empty.
     */
    QString getResponse(const QString &input, MatchList &matches) const;

    /**
     * Refreshes the NLP engine. Invoke this method if the root rule or any other child rule has
     * been changed.
     */
    void refreshNlpEngine();

    /**
     * Chat server type
     */
    enum ChatType {
        FbChat,     ///< Facebook chat
        GTalkChat   ///< Gtalk or Gmail chat
    };

    /**
     * Connection error
     */
    enum ConnectionError {
        SocketError,           ///< Error due to TCP socket
        KeepAliveError,        ///< Error due to no response to a keep alive
        XmppStreamError,       ///< Error due to XML stream
        SSLNotSupportedError,  ///< Error due to SSL not supported on the system
        UnknownXmppError,      ///< Error due to unknown cause on the XMPP protocol
        ChatbotInternalError,  ///< Error due to Chatbot internal error
        UnknownServerTypeError ///< Error due to unknown server \a ChatType
    };

    /**
     * Verifies a chat account of type \a accountType with user \a user, and password \a
     * passwd. This method should not be called again if there is a connection already in progress,
     * in that case call cancelVerifyAccount().
     * Emits \a accountOk if the account could be verified. Otherwise, emits \a accountError.
     */
    void verifyAccount(ChatType accountType, const QString &user, const QString &passwd);

    /**
     * Cancels a verification in progress. If there is no verification in progress this method
     * does nothing.
     */
    void cancelVerifyAccount();

    /**
     * Connects to a chat server of type \a accountType with user \a user, and password \a
     * passwd. This method should not be called if there is a connection already in progress,
     * in that case call disconnectFromChat().
     * Emits \a connected on success. Otherwise; emits \a connectionError.
     * If the connection ends prematurely, emits \a disconnected.
     */
    void connectToChat(ChatType accountType, const QString &user, const QString &passwd);

    /**
     * Disconnects from the current chat server. If there is no connection this method does nothing.
     */
    void disconnectFromChat();

    /**
     * Returns the roster of the last connected or verified user. Before calling this method
     * must be connected.
     */
    Roster roster();

    /**
     * Sets the list of people the chatbot cannot talk to. Before calling this method you
     * must be connected.
     */
    void setBlackListRoster(const Roster &roster);

    /**
     * Returns the chat history of the current chatbot. Before calling this method you
     * must \a load() a chatbot file.
     */
    const Conversation &chatHistory();

    /**
     * Clears the chat history of the current chatbot. All persisted data is also deleted.
     */
    void clearChatHistory();

    /**
     * Clears chat history held with \a user on \a date.
     */
    void clearChatHistory(const QDate &date, const QString &user);


signals:

    /**
     * This signal is emitted after invoking verifyAccount() if the account was verified.
     * Returns the \a roster of the given account.
     */
    void accountOk(const BE::Roster &roster);

    /**
     * This signal is emitted after invoking verifyAccount() if there was an error while trying
     * to verify the account. \a err is one of \a ConnectionError.
     */
    void accountError(int err);

    /**
     * This signal is emitted after invoking verifyAccount() if the connection was successful.
     */
    void connected();

    /**
     * This signal is emitted after invoking verifyAccount() if the connection has ended.
     */
    void disconnected();

    /**
     * This signal es emitted after invoking verifyAccount() if there was an error while trying
     * to connect to the chat server. \a err is one of \a ConnectionError.
     */
    void connectionError(int err);

    /**
     * This signal is emitted whenever the chatbot receives a chat message. \a entry
     * contains the received message, the chatbot response and other useful information.
     * \see BE::Conversation::Entry
     */
    void newConversationEntry(const BE::Conversation::Entry &entry);

private slots:

    void onAccountOk();

    void onAccountError(int err);

private:
    AppFacade(AppFacade&);
    AppFacade& operator=(AppFacade&);

    ChatbotRulesFile m_rulesFile;
    Rule *m_evasivesRule;
    Nlp::Engine *m_nlpEngine;
    CA::Chatbot *m_chatbot;
    CA::Chatbot *m_tmpChatbot;
    ChatType m_currentChatbotType;
    QSet<QString> m_targets;

    bool setDefaultRules();

    inline DefaultVirtualUser *virtualUser();
    void buildNlpRulesOf(const Rule* parentRule, Nlp::RuleList &nlpRules);
    void storeTargets(const TargetList &targets);
    QStringList getEvasives() const;
    void setupChatbot(ChatType type);
    void deleteCurrentChatbot();
    void refreshEvasivesToChatbot();
    void connectChatClientSignals();
};

/// @}

} // namespace BE

/// @}

} // namespace Lvk

#endif // LVK_BE_APPFACADE_H
