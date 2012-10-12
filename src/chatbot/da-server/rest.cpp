#include "da-server/rest.h"
#include "common/version.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QtDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QFile>
#include <QDir>
#include <QSslConfiguration>


//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline void appendCertificate(const QString &filename, QList<QSslCertificate> &certs)
{
    QFile *f = new QFile(filename);
    f->open(QFile::ReadOnly);
    certs.append(QSslCertificate::fromDevice(f, QSsl::Pem));
    delete f;
}

} // namespace


//--------------------------------------------------------------------------------------------------
// Rest
//--------------------------------------------------------------------------------------------------

Lvk::DAS::Rest::Rest(QObject *parent) :
    QObject(parent),
    m_replyMutex(new QMutex(QMutex::Recursive)),
    m_manager(new QNetworkAccessManager(this)),
    m_reply(0),
    m_lastErr(QNetworkReply::NoError),
    m_ignoreSslErrors(false)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::DAS::Rest::~Rest()
{
    {
        QMutexLocker locker(m_replyMutex);
        delete m_reply;
        delete m_manager;
    }

    delete m_replyMutex;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::DAS::Rest::request(const QString &url)
{
    qDebug() << "Rest::request";

    QMutexLocker locker(m_replyMutex);

    if (m_reply) {
        if (m_reply->isRunning()) {
            qCritical() << "Rest: Previous REST request still runnning";
            return false;
        }

        delete m_reply;
        m_reply = 0;
    }

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("User-Agent", APP_NAME "-" APP_VERSION_STR);

    QNetworkReply *reply = m_manager->get(request);

    configureSsl(reply);

    connect(reply, SIGNAL(finished()), SLOT(onFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            SLOT(onError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(onSslErrors(QList<QSslError>)));

    m_lastErr = QNetworkReply::NoError;
    m_reply = reply;
    m_peerCertificateChain.clear();

    return true;
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::Rest::abort()
{
    qDebug() << "Rest::abort";

    QMutexLocker locker(m_replyMutex);

    if (m_reply) {
        m_reply->abort();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::Rest::setIgnoreSslErrors(bool ignore)
{
    m_ignoreSslErrors = ignore;
}


//--------------------------------------------------------------------------------------------------

QList<QSslCertificate> Lvk::DAS::Rest::peerCertificateChain()
{
    QMutexLocker locker(m_replyMutex);

    if (m_reply) {
        return m_reply->sslConfiguration().peerCertificateChain();
    } else {
        return QList<QSslCertificate>();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::Rest::onFinished()
{
    qDebug() << "Rest::onFinished";

    QString resp;

    {
        QMutexLocker locker(m_replyMutex);

        if (m_reply->error() == QNetworkReply::NoError) {
            resp = QString::fromUtf8(m_reply->readAll());
            unescape(resp);
            m_peerCertificateChain = m_reply->sslConfiguration().peerCertificateChain();
        }
    }

    if (resp.size() > 0) {
        qDebug() << "Rest: Got response: " << resp;
        emit response(resp);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::Rest::onError(QNetworkReply::NetworkError err)
{
    qDebug() << "Rest::onError" << err;

    m_replyMutex->lock();

    // emit only one error per request
    if (m_lastErr == QNetworkReply::NoError) {
        m_lastErr = err;
        m_replyMutex->unlock();

        emit error(err);
    } else {
        m_replyMutex->unlock();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::Rest::onSslErrors(const QList<QSslError> &errs)
{
    qDebug() << "Rest::onSslErrors";
    foreach (const QSslError &err, errs) {
        qDebug() << "Error: " << err.errorString();
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::Rest::unescape(QString &resp)
{
    // unescape UTF-16 chars
    QRegExp regex("(\\\\u[0-9a-fA-F]{4})");
    int pos = 0;
    while ((pos = regex.indexIn(resp, pos)) != -1) {
        resp.replace(pos++, 6, QChar(regex.cap(1).right(4).toUShort(0, 16)));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::Rest::configureSsl(QNetworkReply *reply)
{
    if (!m_ignoreSslErrors) {
        QSslConfiguration conf = reply->sslConfiguration();
        QList<QSslCertificate> certs = conf.caCertificates();

        appendCertificate(":/certs/GeoTrustGlobalCA.pem", certs);
        appendCertificate(":/certs/RapidSSLCA.pem", certs);

        conf.setCaCertificates(certs);

        reply->setSslConfiguration(conf);
    } else {
        reply->ignoreSslErrors();
    }
}
