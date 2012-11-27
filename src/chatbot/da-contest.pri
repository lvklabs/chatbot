# Config options required for "Dale Aceptar" contest

CONFIG  += gelf_stats openssl qssh
DEFINES += DA_CONTEST
INCLUDEPATH +=
HEADERS += \
    da-server/sftpuploader.h \
    da-server/fileuploader.h
SOURCES += \
    da-server/sftpuploader.cpp
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
    DEFINES += GELF_STATS_SUPPORT
    INCLUDEPATH += $$ZLIB_INCLUDE_PATH
    HEADERS += da-server/zlibhelper.h \
        da-server/graylogremotelogger.h \
        da-server/gelf.h \
        da-server/syslog.h
    SOURCES += da-server/zlibhelper.cpp \
        da-server/graylogremotelogger.cpp \
        da-server/gelf.cpp \
        da-server/syslog.cpp
    LIBS += -L$$ZLIB_LIB_PATH $$ZLIB_LIBS
}

openssl {
    DEFINES += OPENSSL_SUPPORT
    INCLUDEPATH += $$OPENSSL_INCLUDE_PATH
    HEADERS +=
    SOURCES +=
    LIBS += -L$$OPENSSL_LIB_PATH $$OPENSSL_LIBS
}

qssh {
    DEFINES += QSSH_SUPPORT
    INCLUDEPATH += $$QSSH_INCLUDE_PATH
    HEADERS +=
    SOURCES +=
    LIBS += -L$$QSSH_LIB_PATH $$QSSH_LIBS
}




