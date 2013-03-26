# Config options required for "Dale Aceptar" contest

CONFIG  += gelf_stats openssl qssh
DEFINES += DA_CONTEST
INCLUDEPATH +=
LIBS +=

unix:!mac{
  QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\''
  QMAKE_LFLAGS_RPATH=
}

isEmpty(PROJECT_PATH) {
    PROJECT_PATH = .
}

RC_FILE += $$PROJECT_PATH/res/chatbot.rc
OTHER_FILES += $$PROJECT_PATH/res/chatbot.rc

exists (da-server/daserverconfig.h) {
    DEFINES += DA_SERVER_CONFIG
    HEADERS += $$PROJECT_PATH/da-server/daserverconfig.h
}

exists (crypto/dakeymanager.h) {
    DEFINES += DA_KEY_MANAGER
    HEADERS += $$PROJECT_PATH/crypto/dakeymanager.h
    SOURCES += $$PROJECT_PATH/crypto/dakeymanager.cpp
}

gelf_stats {
    HEADERS += \
        $$PROJECT_PATH/da-server/zlibhelper.h \
        $$PROJECT_PATH/da-server/graylogremotelogger.h \
        $$PROJECT_PATH/da-server/gelf.h \
        $$PROJECT_PATH/da-server/syslog.h
    SOURCES += \
        $$PROJECT_PATH/da-server/zlibhelper.cpp \
        $$PROJECT_PATH/da-server/graylogremotelogger.cpp \
        $$PROJECT_PATH/da-server/gelf.cpp \
        $$PROJECT_PATH/da-server/syslog.cpp
}

qssh {
    HEADERS += $$PROJECT_PATH/da-server/sftpcontestdatauploader.h
    SOURCES += $$PROJECT_PATH/da-server/sftpcontestdatauploader.cpp
}

