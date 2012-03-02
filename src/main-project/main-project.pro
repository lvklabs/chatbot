TARGET = main

QT += ui
TEMPLATE = app

INCLUDEPATH += front-end\
               back-end

HEADERS += \
    front-end/mainwindow.h \
    front-end/ruletreemodel.h \
    back-end/mainapp.h \
    back-end/rule.h

SOURCES += \
    main.cpp \
    front-end/mainwindow.cpp \
    front-end/ruletreemodel.cpp \
    back-end/mainapp.cpp \
    back-end/rule.cpp

FORMS += \
    front-end/mainwindow.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS = strings_es.ts









