TARGET = main

QT += ui
TEMPLATE = app

HEADERS += \
    mainwindow.h \
    ruletreemodel.h \
    ruleitem.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ruletreemodel.cpp \
    ruleitem.cpp

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS = strings_es.ts



