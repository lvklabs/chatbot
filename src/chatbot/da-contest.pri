# Config options required for "Dale Aceptar" contest

CONFIG  += gelf_stats openssl qssh
DEFINES += DA_CONTEST
INCLUDEPATH +=
LIBS +=

unix:!mac{
  QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\''
  QMAKE_LFLAGS_RPATH=
}

exists (da-server/daserverconfig.h) {
    DEFINES += DA_SERVER_CONFIG
    HEADERS += da-server/daserverconfig.h
}

exists (crypto/dakeymanager.h) {
    DEFINES += DA_KEY_MANAGER
    HEADERS += crypto/dakeymanager.h
    SOURCES += crypto/dakeymanager.cpp
}

gelf_stats {
    HEADERS += da-server/zlibhelper.h \
        da-server/graylogremotelogger.h \
        da-server/gelf.h \
        da-server/syslog.h
    SOURCES += da-server/zlibhelper.cpp \
        da-server/graylogremotelogger.cpp \
        da-server/gelf.cpp \
        da-server/syslog.cpp
}

qssh {
    HEADERS += da-server/sftpcontestdatauploader.h
    SOURCES += da-server/sftpcontestdatauploader.cpp
}
