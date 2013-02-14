# Paths to third party libraries
THIRD_PARTY_PATH      = $$PWD/../third-party

# QXmpp - Mandatory
QXMPP_BASE_PATH       = $$THIRD_PARTY_PATH/QXmpp
QXMPP_INCLUDE_PATH    = $$QXMPP_BASE_PATH/include
QXMPP_LIB_PATH        = $$QXMPP_BASE_PATH/lib

# Freeling - Optional
FREELING_BASE_PATH    = $$THIRD_PARTY_PATH/Freeling
FREELING_INCLUDE_PATH = $$FREELING_BASE_PATH/include
FREELING_LIB_PATH     = $$FREELING_BASE_PATH/lib

# Zlib - Optional
ZLIB_BASE_PATH        = $$THIRD_PARTY_PATH/zlib
ZLIB_INCLUDE_PATH     = $$ZLIB_BASE_PATH/include
ZLIB_LIB_PATH         = $$ZLIB_BASE_PATH/lib

# OpenSSL - Optional
OPENSSL_BASE_PATH     = $$THIRD_PARTY_PATH/openssl
OPENSSL_INCLUDE_PATH  = $$OPENSSL_BASE_PATH/include
OPENSSL_LIB_PATH      = $$OPENSSL_BASE_PATH/lib

# QSsh - Optional
QSSH_BASE_PATH        = $$THIRD_PARTY_PATH/QSsh
QSSH_INCLUDE_PATH     = $$QSSH_BASE_PATH/include
QSSH_LIB_PATH         = $$QSSH_BASE_PATH/lib


CONFIG(debug, debug|release) {
    win32 {
        QXMPP_LIBS         = -lqxmpp_win32_d
        FREELING_LIBS      = -lmorfo_win32 -lfries_win32 -lomlet_win32 -lpcre_win32
        ZLIB_LIBS          = -lz_win32
        OPENSSL_LIBS       = -lcrypto_win32 -lgdi32
        QSSH_LIBS          = -lBotan -lQSsh
    } else:mac {
        QXMPP_LIBS         = -lqxmpp_mac_d
        FREELING_LIBS      = # TODO compile freeling for Mac
        ZLIB_LIBS          = # TODO compile zlib for Mac
        QSSH_LIBS          = # TODO compile QSsh for Mac
    } else {
        QXMPP_LIBS         = -lqxmpp_d
        FREELING_LIBS      = -lmorfo -lfries -lomlet
        ZLIB_LIBS          = -lz
        OPENSSL_LIBS       = -lcrypto
        QSSH_LIBS          = -lBotan -lQSsh
    }
} else {
    win32 {
        QXMPP_LIBS         = -lqxmpp_win32
        FREELING_LIBS      = -lmorfo_win32 -lfries_win32 -lomlet_win32 -lpcre_win32
        ZLIB_LIBS          = -lz_win32
        OPENSSL_LIBS       = -lcrypto_win32 -lgdi32
        QSSH_LIBS          = -lBotan -lQSsh
    } else:mac {
        QXMPP_LIBS         = -lqxmpp_mac
        FREELING_LIBS      = # TODO compile freeling for Mac
        ZLIB_LIBS          = # TODO compile zlib for Mac
        OPENSSL_LIBS       = # TODO compile openssl for Mac
        QSSH_LIBS          = # TODO compile QSsh for Mac
    } else {
        QXMPP_LIBS         = -lqxmpp
        FREELING_LIBS      = -lmorfo -lfries -lomlet
        ZLIB_LIBS          = -lz
        OPENSSL_LIBS       = -lcrypto
        QSSH_LIBS          = -lBotan -lQSsh
    }
}

INCLUDEPATH += $$THIRD_PARTY_PATH

qxmpp {
    DEFINES += QXMPP_SUPPORT
    INCLUDEPATH += $$QXMPP_INCLUDE_PATH
    LIBS += -L$$QXMPP_LIB_PATH $$QXMPP_LIBS
}

freeling {
    DEFINES += PCRE_STATIC FREELING_SUPPORT
    INCLUDEPATH += $$FREELING_INCLUDE_PATH
    LIBS += -L$$FREELING_LIB_PATH $$FREELING_LIBS
}

gelf_stats {
    DEFINES += GELF_STATS_SUPPORT
    INCLUDEPATH += $$ZLIB_INCLUDE_PATH
    LIBS += -L$$ZLIB_LIB_PATH $$ZLIB_LIBS
}

openssl {
    DEFINES += OPENSSL_SUPPORT
    INCLUDEPATH += $$OPENSSL_INCLUDE_PATH
    LIBS += -L$$OPENSSL_LIB_PATH $$OPENSSL_LIBS
}

qssh {
    DEFINES += QSSH_SUPPORT
    INCLUDEPATH += $$QSSH_INCLUDE_PATH
    LIBS += -L$$QSSH_LIB_PATH $$QSSH_LIBS
}
