# NLP Engine module

HEADERS += \
    nlp-engine/sanitizer.h \
    nlp-engine/defaultsanitizer.h \
    nlp-engine/nullsanitizer.h \
    nlp-engine/lemmatizer.h \
    nlp-engine/nulllemmatizer.h \
    nlp-engine/rule.h \
    nlp-engine/engine.h \
    nlp-engine/lemmatizerfactory.h \
    nlp-engine/sanitizerfactory.h \
    nlp-engine/enginefactory.h \
    nlp-engine/nlpproperties.h \
    nlp-engine/cb2engine.h \
    nlp-engine/tree.h \
    nlp-engine/globaltools.h \
    nlp-engine/scoringalgorithm.h \
    nlp-engine/matchpolicy.h \
    nlp-engine/word.h \
    nlp-engine/node.h \
    nlp-engine/result.h \
    nlp-engine/condoutput.h \
    nlp-engine/varstack.h \

SOURCES += \
    nlp-engine/defaultsanitizer.cpp \
    nlp-engine/lemmatizerfactory.cpp \
    nlp-engine/sanitizerfactory.cpp \
    nlp-engine/enginefactory.cpp \
    nlp-engine/cb2engine.cpp \
    nlp-engine/tree.cpp \
    nlp-engine/globaltools.cpp \
    nlp-engine/scoringalgorithm.cpp \
    nlp-engine/matchpolicy.cpp \


freeling {
    HEADERS += nlp-engine/freelinglemmatizer.h
    SOURCES += nlp-engine/freelinglemmatizer.cpp
}
