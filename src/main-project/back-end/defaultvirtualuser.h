#ifndef DEFAULTVIRTUALUSER_H
#define DEFAULTVIRTUALUSER_H

#include <QStringList>
#include "chatvirtualuser.h"

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

class DefaultVirtualUser : public CA::VirtualUser
{
public:
    DefaultVirtualUser(Nlp::Engine *engine = 0);
    ~DefaultVirtualUser();

    virtual QString getResponse(const QString &input, const QString &from);

    virtual QPixmap getAvatar();

    void setNlpEngine(Nlp::Engine *engine);
    void setEvasives(const QStringList &evasives);

private:
    DefaultVirtualUser(DefaultVirtualUser&);
    DefaultVirtualUser& operator=(DefaultVirtualUser&);

    Nlp::Engine *m_engine;
    QStringList m_evasives;
    QFile *m_logFile;

    void logConversation(const QString &input, const QString &from, const QString &response,
                         bool matched);
    void logError(const QString &msg);
};

} //namespace BE

} //namespace Lvk

#endif // DEFAULTVIRTUALUSER_H
