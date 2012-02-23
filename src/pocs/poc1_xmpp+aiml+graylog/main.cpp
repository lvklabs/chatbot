#include <QtCore/QCoreApplication>
#include <QSettings>
#include "XmppClient.h"
#include "QXmppLogger.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    QXmppLogger::getLogger()->setLoggingType(QXmppLogger::StdoutLogging);

    QSettings settings("config.ini", QSettings::IniFormat);

    if (settings.status() == QSettings::NoError) {
        QString username = settings.value("username").toString();
        QString password = settings.value("password").toString();

        XmppClient client;

        client.connectToServer(username, password);

        return a.exec();

    } else if (settings.status() == QSettings::FormatError) {
        std::cerr << "Error: Cannot read config.ini. Invalid format." << std::endl;
        return -1;
    } else {
        std::cerr << "Error: Cannot open config.ini" << std::endl;
        return -1;
    }
}
