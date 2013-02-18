# Dale Aceptar server connection module

QT += \
    xml \
    network \

isEmpty(PROJECT_PATH) {
    PROJECT_PATH = .
}

HEADERS += \
    $$PROJECT_PATH/da-server/remoteloggerfactory.h \
    $$PROJECT_PATH/da-server/remoteloggerkeys.h \
    $$PROJECT_PATH/da-server/remotelogger.h \
    $$PROJECT_PATH/da-server/nullremotelogger.h \
    $$PROJECT_PATH/da-server/rest.h \
    $$PROJECT_PATH/da-server/userauth.h \
    $$PROJECT_PATH/da-server/updater.h \
    $$PROJECT_PATH/da-server/updateinfo.h \
    $$PROJECT_PATH/da-server/updateversion.h \
    $$PROJECT_PATH/da-server/serverconfig.h \
    $$PROJECT_PATH/da-server/nullserverconfig.h \
    $$PROJECT_PATH/da-server/contestdata.h \
    $$PROJECT_PATH/da-server/contestdatauploader.h \
    $$PROJECT_PATH/da-server/nullcontestdatauploader.h \
    $$PROJECT_PATH/da-server/datauploaderfactory.h \

SOURCES += \
    $$PROJECT_PATH/da-server/remoteloggerfactory.cpp \
    $$PROJECT_PATH/da-server/rest.cpp \
    $$PROJECT_PATH/da-server/userauth.cpp \
    $$PROJECT_PATH/da-server/updater.cpp \
    $$PROJECT_PATH/da-server/datauploaderfactory.cpp \

RESOURCES += \
    $$PROJECT_PATH/res/certs.qrc
