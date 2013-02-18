# Cryptographic module

isEmpty(PROJECT_PATH) {
    PROJECT_PATH = .
}

HEADERS += \
    $$PROJECT_PATH/crypto/cipher.h \
    $$PROJECT_PATH/crypto/keymanager.h \
    $$PROJECT_PATH/crypto/defaultkeymanager.h \
    $$PROJECT_PATH/crypto/keymanagerfactory.h \

SOURCES += \
    $$PROJECT_PATH/crypto/cipher.cpp \
    $$PROJECT_PATH/crypto/keymanagerfactory.cpp \
