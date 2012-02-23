#ifndef REMOTELOGGER_H
#define REMOTELOGGER_H

class QString;
class QByteArray;

class RemoteLogger
{
public:
    RemoteLogger();

    bool logInfo(const QString& msg);
    bool logError(const QString& msg);
    bool logCritical(const QString& msg);

private:
    void buildGelfMessage(int level, const QString &msg, QByteArray &gelfMsg);
    bool sendGelfMessage(const QByteArray &gelfMsg);
    bool compress(const QByteArray &in, QByteArray &out);
};

#endif // REMOTELOGGER_H
