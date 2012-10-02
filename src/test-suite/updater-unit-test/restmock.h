#ifndef RESTMOCK_H
#define RESTMOCK_H

#include <QObject>
#include <QString>
#include "da-server/rest.h"

class RestMock : public Lvk::DAS::Rest
{
    Q_OBJECT

public:
    explicit RestMock(QObject *parent = 0);

    bool setResponse(const QString &filename);

    virtual bool request(const QString &url);

    virtual void abort();

signals:

    virtual void response(const QString &resp);

    virtual void error(QNetworkReply::NetworkError err);

private:
    QString m_response;
};

#endif // RESTMOCK_H
