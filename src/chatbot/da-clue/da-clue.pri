# Dale Aceptar Clue module

QT += xml

isEmpty(PROJECT_PATH) {
    PROJECT_PATH = .
}

HEADERS += \
    $$PROJECT_PATH/da-clue/scriptmanager.h \
    $$PROJECT_PATH/da-clue/scriptline.h \
    $$PROJECT_PATH/da-clue/script.h \
    $$PROJECT_PATH/da-clue/genericscript.h \
    $$PROJECT_PATH/da-clue/clueengine.h \
    $$PROJECT_PATH/da-clue/character.h \
    $$PROJECT_PATH/da-clue/analyzedscript.h \
    $$PROJECT_PATH/da-clue/scriptparser.h \
    $$PROJECT_PATH/da-clue/analyzedline.h \
    $$PROJECT_PATH/da-clue/scripterror.h \
    $$PROJECT_PATH/da-clue/regexp.h


SOURCES += \
    $$PROJECT_PATH/da-clue/scriptmanager.cpp \
    $$PROJECT_PATH/da-clue/scriptparser.cpp \
    $$PROJECT_PATH/da-clue/clueengine.cpp \
    $$PROJECT_PATH/da-clue/regexp.cpp



