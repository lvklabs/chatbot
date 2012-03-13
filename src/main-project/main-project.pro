TARGET = main

QT += \
    ui \
    xml \           #required for ProgramQ
    qt3support      #required for ProgramQ

TEMPLATE = app

INCLUDEPATH += \
    front-end \
    back-end \
    nlp-engine \
    common \
    $$PWD/../third-party

HEADERS += \
    front-end/mainwindow.h \
    front-end/ruletreemodel.h \
    front-end/ruleoutputwidget.h \
    front-end/ruleinputwidget.h \
    front-end/testconversationwidget.h \
    back-end/rule.h \
    back-end/coreapp.h \
    nlp-engine/nlpengine.h \
    nlp-engine/nlprule.h \
    nlp-engine/exactmatchengine.h \
    nlp-engine/aimlengine.h \
    common/random.h\
    $$PWD/../third-party/ProgramQ/aimlparser.h

SOURCES += \
    main.cpp \
    front-end/mainwindow.cpp \
    front-end/ruletreemodel.cpp \
    front-end/ruleoutputwidget.cpp \
    front-end/ruleinputwidget.cpp \
    front-end/testconversationwidget.cpp \
    back-end/rule.cpp \
    back-end/coreapp.cpp \
    nlp-engine/exactmatchengine.cpp \
    nlp-engine/aimlengine.cpp \
    common/random.cpp\
    $$PWD/../third-party/ProgramQ/aimlparser.cpp

FORMS += \
    front-end/mainwindow.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS = strings_es.ts





























