#ifndef DEFAULTVIRTUALUSER_H
#define DEFAULTVIRTUALUSER_H

#include <QObject>
#include <QStringList>
#include "chatvirtualuser.h"
#include "conversation.h"

class QFile;

namespace Lvk
{

namespace Nlp
{
    class Engine;
}

namespace BE
{

/**
 * \brief Default implementation of the Abstract class Virtual User
 */

class DefaultVirtualUser : public QObject, public CA::VirtualUser
{
    Q_OBJECT

public:
    DefaultVirtualUser(Nlp::Engine *engine = 0, QObject *parent = 0);
    ~DefaultVirtualUser();

    virtual QString getResponse(const QString &input, const QString &from);

    virtual QPixmap getAvatar();

    void setNlpEngine(Nlp::Engine *engine);
    void setEvasives(const QStringList &evasives);

signals:
    void newConversationEntry(const Conversation::Entry &entry);

private:
    DefaultVirtualUser(DefaultVirtualUser&);
    DefaultVirtualUser& operator=(DefaultVirtualUser&);

    Nlp::Engine *m_engine;
    QStringList m_evasives;
    QFile *m_logFile;

    void logConversationEntry(const Conversation::Entry &entry);
    void logError(const QString &msg);
};

} //namespace BE

} //namespace Lvk

#endif // DEFAULTVIRTUALUSER_H
