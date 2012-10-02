#ifndef LVK_DAS_REST_H
#define LVK_DAS_REST_H

#include <QObject>
#include <QNetworkReply>
#include <QList>
#include <QSslError>

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
 * \brief The Rest class provides methods to make REST requests
 */
class Rest : public QObject
{
    Q_OBJECT

public:

    explicit Rest(QObject *parent = 0);

    virtual ~Rest();

    virtual bool request(const QString &url);

    virtual void abort();

signals:

    virtual void response(const QString &resp);

    virtual void error(QNetworkReply::NetworkError err);

private slots:

    void onFinished();
    void onError(QNetworkReply::NetworkError err);
    void onSslErrors(const QList<QSslError> &errs);

private:
    QMutex *m_replyMutex;
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    QNetworkReply::NetworkError m_lastErr;

    void unescape(QString &resp);
};


/// @}

} // namespace DAS

/// @}

} // namespace Lvk

#endif // LVK_DAS_REST_H
