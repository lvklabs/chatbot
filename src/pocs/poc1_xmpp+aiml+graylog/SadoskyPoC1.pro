########################################
# QXmpp Common definitions

QT += network xml
QXMPP_INCLUDE_DIR = /home/andres/Software/qxmpp-0.3.0/src
QXMPP_LIBRARY_DIR = /home/andres/Software/qxmpp-0.3.0/lib

CONFIG(debug, debug|release) {
    QXMPP_LIBRARY_NAME = qxmpp_d
} else {
    QXMPP_LIBRARY_NAME = qxmpp
}

# Libraries used internal by QXmpp
symbian {
    QXMPP_INTERNAL_LIBS = -lesock
}

# Libraries for apps which use QXmpp
symbian {
    # Symbian needs a .lib extension to recognise the library as static
    QXMPP_LIBS = -L$$QXMPP_LIBRARY_DIR -l$${QXMPP_LIBRARY_NAME}.lib
} else {
    QXMPP_LIBS = -L$$QXMPP_LIBRARY_DIR -l$${QXMPP_LIBRARY_NAME}
}

# FIXME: we should be able to use the link_prl option to automatically pull
# in the extra libraries which the qxmpp library needs, but this does not
# seem to work on win32, so we specify the dependencies here:
QXMPP_LIBS += $$QXMPP_INTERNAL_LIBS

# Path of the QXmpp library file, for expressing dependency.
symbian {
} else:unix {
    QXMPP_LIBRARY_FILE = $${QXMPP_LIBRARY_DIR}/lib$${QXMPP_LIBRARY_NAME}.a
}

#######################################
#include(../examples.pri)

TEMPLATE = app
CONFIG += console

INCLUDEPATH += $$QXMPP_INCLUDE_DIR
LIBS += $$QXMPP_LIBS
PRE_TARGETDEPS += $$QXMPP_LIBRARY_FILE

#######################################

TARGET = SadoskyPoC1

SOURCES +=  main.cpp \
    aimlparser.cpp \
    ZlibHelper.cpp \
    RemoteLogger.cpp \
    XmppClient.cpp

HEADERS +=  \
    aimlparser.h \
    ZlibHelper.h \
    RemoteLogger.h \
    XmppClient.h

OTHER_FILES += \
    config.ini \
    rules.aiml


#######################################
#aimlparser
QT      += qt3support xml
CONFIG	+= uic3

#######################################

LIBS += -lz



















