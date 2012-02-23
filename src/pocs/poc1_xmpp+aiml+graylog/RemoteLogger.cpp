#include "RemoteLogger.h"
#include "ZlibHelper.h"

#include <QUdpSocket>
#include <QString>
#include <QByteArray>
#include <QDateTime>
#include <QHostInfo>
#include <iostream>

//#define USE_SPLUNK_SERVER

#ifdef USE_SPLUNK_SERVER
# define LOG_SERVER_HOST "127.0.0.1"
# define LOG_SERVER_PORT 8001
#else
# define LOG_SERVER_HOST "127.0.0.1"
# define LOG_SERVER_PORT 12201
#endif

RemoteLogger::RemoteLogger()
{
}

bool RemoteLogger::logInfo(const QString& msg)
{
    QByteArray gelfMsg;

    buildGelfMessage(6, msg, gelfMsg);

    return sendGelfMessage(gelfMsg);
}

bool RemoteLogger::logError(const QString& msg)
{
    QByteArray gelfMsg;

    buildGelfMessage(3, msg, gelfMsg);

    return sendGelfMessage(gelfMsg);
}

bool RemoteLogger::logCritical(const QString& msg)
{
    QByteArray gelfMsg;

    buildGelfMessage(2, msg, gelfMsg);

    return sendGelfMessage(gelfMsg);
}


void RemoteLogger::buildGelfMessage(int level, const QString &msg, QByteArray &gelfMsg)
{
    /* A GELF message is a GZIP'd or ZLIB'd JSON string with the following fields:
         version: GELF spec version – "1.0" (string); MUST be set by client library.
         host: the name of the host or application that sent this message (string); MUST be set by client library.
         short_message: a short descriptive message (string); MUST be set by client library.
         full_message: a long message that can i.e. contain a backtrace and environment variables (string); optional.
         timestamp: UNIX microsecond timestamp (decimal); SHOULD be set by client library.
         level: the level equal to the standard syslog levels (decimal); optional, default is 1 (ALERT).
         facility: (string or decimal) optional, MUST be set by server to GELF if empty.
         line: the line in a file that caused the error (decimal); optional.
         file: the file (with path if you want) that caused the error (string); optional.
         _[additional field]: every other field you send and prefix with a _ (underscore) will be treated as an additional field.
    */

    /* Syslog levels:
        0 Emergency: system is unusable
        1 Alert: action must be taken immediately
        2 Critical: critical conditions
        3 Error: error conditions
        4 Warning: warning conditions
        5 Notice: normal but significant condition
        6 Informational: informational messages
        7 Debug: debug-level messages
    */

    QString strGelfMsg(
    "{"
      "\"version\": \"1.0\","
      "\"host\": \"" + QHostInfo::localHostName() + "\","
      "\"short_message\": \"" + msg + "\","
//    "\"full_message\": \"" + msg + "\","
//    "\"timestamp\": " + QString::number(0/*QDateTime::currentMSecsSinceEpoch()*1000*/) + ","
      "\"level\": " + QString::number(level) +  ","
      "\"facility\": \"SadoskiPoc1\","
//    "\"file\": \"/var/www/somefile.rb\","
//    "\"line\": 356,"
      "\"_user_id\": 42,"
      "\"_event_type\": \"UsageEvent\","
//    "\"_something_else\": \"foo\""
    "}");


    // TODO check encoding
#ifdef USE_SPLUNK_SERVER
    gelfMsg = strGelfMsg.toUtf8();
#else
    compress(strGelfMsg.toUtf8(), gelfMsg);
#endif
}

bool RemoteLogger::sendGelfMessage(const QByteArray &gelfMsg)
{
    if (gelfMsg.size() == 0) {
        return false;
    }

    qint64 bytes = QUdpSocket().writeDatagram(gelfMsg, QHostAddress(LOG_SERVER_HOST), LOG_SERVER_PORT);

    if (bytes == -1) {
        std::cout << "ERROR: UDP write failed" << std::endl;
    }

    return bytes != -1;
}

bool RemoteLogger::compress(const QByteArray &in, QByteArray &out)
{
    char *dest = new char[in.size()*2];
    long dsize = in.size()*2;

    int ret = zlib_deflate(in.data(), in.size(), dest, &dsize, Z_DEFAULT_COMPRESSION);
    if (ret == Z_OK) {
        out = QByteArray(dest, dsize);
    } else {
        std::cout << "ERROR: Compression failed!" << std::endl;
    }

    delete[] dest;

    return ret != Z_OK;
}
