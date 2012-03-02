TARGET = main

QT += ui
TEMPLATE = app

INCLUDEPATH += front-end\
               back-end

HEADERS += \
    front-end/mainwindow.h \
    front-end/ruletreemodel.h \
    back-end/ruleitem.h \
    back-end/mainapp.h

SOURCES += \
    main.cpp \
    front-end/mainwindow.cpp \
    front-end/ruletreemodel.cpp \
    back-end/ruleitem.cpp \
    back-end/mainapp.cpp

FORMS += \
    front-end/mainwindow.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS = strings_es.ts





