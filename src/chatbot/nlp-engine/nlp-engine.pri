# NLP Engine module

isEmpty(PROJECT_PATH) {
    PROJECT_PATH = .
}

HEADERS += \
    $$PROJECT_PATH/nlp-engine/sanitizer.h \
    $$PROJECT_PATH/nlp-engine/defaultsanitizer.h \
    $$PROJECT_PATH/nlp-engine/nullsanitizer.h \
    $$PROJECT_PATH/nlp-engine/lemmatizer.h \
    $$PROJECT_PATH/nlp-engine/nulllemmatizer.h \
    $$PROJECT_PATH/nlp-engine/rule.h \
    $$PROJECT_PATH/nlp-engine/engine.h \
    $$PROJECT_PATH/nlp-engine/lemmatizerfactory.h \
    $$PROJECT_PATH/nlp-engine/sanitizerfactory.h \
    $$PROJECT_PATH/nlp-engine/enginefactory.h \
    $$PROJECT_PATH/nlp-engine/nlpproperties.h \
    $$PROJECT_PATH/nlp-engine/cb2engine.h \
    $$PROJECT_PATH/nlp-engine/tree.h \
    $$PROJECT_PATH/nlp-engine/globaltools.h \
    $$PROJECT_PATH/nlp-engine/scoringalgorithm.h \
    $$PROJECT_PATH/nlp-engine/matchpolicy.h \
    $$PROJECT_PATH/nlp-engine/word.h \
    $$PROJECT_PATH/nlp-engine/node.h \
    $$PROJECT_PATH/nlp-engine/result.h \
    $$PROJECT_PATH/nlp-engine/condoutput.h \
    $$PROJECT_PATH/nlp-engine/varstack.h \
    $$PROJECT_PATH/nlp-engine/predicate.h \
    $$PROJECT_PATH/nlp-engine/condoutputlist.h

SOURCES += \
    $$PROJECT_PATH/nlp-engine/defaultsanitizer.cpp \
    $$PROJECT_PATH/nlp-engine/lemmatizerfactory.cpp \
    $$PROJECT_PATH/nlp-engine/sanitizerfactory.cpp \
    $$PROJECT_PATH/nlp-engine/enginefactory.cpp \
    $$PROJECT_PATH/nlp-engine/cb2engine.cpp \
    $$PROJECT_PATH/nlp-engine/tree.cpp \
    $$PROJECT_PATH/nlp-engine/globaltools.cpp \
    $$PROJECT_PATH/nlp-engine/scoringalgorithm.cpp \
    $$PROJECT_PATH/nlp-engine/matchpolicy.cpp \
    $$PROJECT_PATH/nlp-engine/condoutput.cpp \
    $$PROJECT_PATH/nlp-engine/condoutputlist.cpp


freeling {
    HEADERS += \
        $$PROJECT_PATH/nlp-engine/freelinglemmatizer.h

    SOURCES += \
        $$PROJECT_PATH/nlp-engine/freelinglemmatizer.cpp
}







