#ifndef LVK_DAS_REST_H
#define LVK_DAS_REST_H

#include <QObject>
#include <QNetworkReply>
#include <QList>
#include <QSslError>
#include <QSslCertificate>

class QNetworkAccessManager;
class QMutex;


namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace DAS
{

/// \ingroup Lvk
/// \addtogroup DAS
/// @{

/**
 * \brief The Rest class provides a way to make REST requests
 */
class Rest : public QObject
{
    Q_OBJECT

public:

    explicit Rest(QObject *parent = 0);

    virtual ~Rest();

    virtual bool request(const QString &url);

    virtual void abort();

    virtual void setIgnoreSslErrors(bool ignore);

    virtual QList<QSslCertificate> peerCertificateChain();

signals:

    void response(const QString &resp);

    void error(QNetworkReply::NetworkError err);

private slots:

    void onFinished();
    void onError(QNetworkReply::NetworkError err);
    void onSslErrors(const QList<QSslError> &errs);

private:
    Rest(const Rest&);
    Rest & operator=(const Rest&);

    QMutex *m_replyMutex;
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    QNetworkReply::NetworkError m_lastErr;
    bool m_ignoreSslErrors;
    QList<QSslCertificate> m_peerCertificateChain;

    void configureSsl(QNetworkReply *reply);
    void unescape(QString &resp);
};


/// @}

} // namespace DAS

/// @}

} // namespace Lvk

#endif // LVK_DAS_REST_H
