
#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include "QXmppClient.h"
#include "QXmppMessage.h"

class AIMLParser;
class QXmppVCardIq;

class XmppClient : public QXmppClient
{
    Q_OBJECT

public:
    XmppClient(QObject *parent = 0);
    ~XmppClient();

public slots:
    void messageReceived(const QXmppMessage&);
    void vCardReceived(const QXmppVCardIq&);

private:
    AIMLParser *m_parser;
    QString m_nickname;
    QXmppMessage m_queuedMessage;
};

#endif // ECHOCLIENT_H
