# Dale Aceptar server connection module

QT += \
    xml \
    network \

HEADERS += \
    da-server/remoteloggerfactory.h \
    da-server/remoteloggerkeys.h \
    da-server/remotelogger.h \
    da-server/nullremotelogger.h \
    da-server/rest.h \
    da-server/userauth.h \
    da-server/updater.h \
    da-server/updateinfo.h \
    da-server/updateversion.h \
    da-server/serverconfig.h \
    da-server/nullserverconfig.h \
    da-server/contestdata.h \
    da-server/contestdatauploader.h \
    da-server/nullcontestdatauploader.h \
    da-server/datauploaderfactory.h \

SOURCES += \
    da-server/remoteloggerfactory.cpp \
    da-server/rest.cpp \
    da-server/userauth.cpp \
    da-server/updater.cpp \
    da-server/datauploaderfactory.cpp \

RESOURCES += \
    res/certs.qrc
