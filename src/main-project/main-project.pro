TARGET = main

QT += ui
TEMPLATE = app

INCLUDEPATH += front-end\
               back-end\
               nlp-engine

HEADERS += \
    front-end/mainwindow.h \
    front-end/ruletreemodel.h \
    back-end/rule.h \
    back-end/coreapp.h \
    nlp-engine/nlpengine.h \
    nlp-engine/nlprule.h

SOURCES += \
    main.cpp \
    front-end/mainwindow.cpp \
    front-end/ruletreemodel.cpp \
    back-end/rule.cpp \
    back-end/coreapp.cpp

FORMS += \
    front-end/mainwindow.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS = strings_es.ts

















