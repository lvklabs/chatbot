# Statistics module

isEmpty(PROJECT_PATH) {
    PROJECT_PATH = .
}

HEADERS += \
    $$PROJECT_PATH/stats/statsmanager.h \
    $$PROJECT_PATH/stats/statsfile.h \
    $$PROJECT_PATH/stats/history.h \
    $$PROJECT_PATH/stats/timeinterval.h \
    $$PROJECT_PATH/stats/securestatsfile.h \
    $$PROJECT_PATH/stats/score.h \
    $$PROJECT_PATH/stats/metric.h \
    $$PROJECT_PATH/stats/rulestatshelper.h \
    $$PROJECT_PATH/stats/historystatshelper.h \
    $$PROJECT_PATH/stats/statshelper.h \

SOURCES += \
    $$PROJECT_PATH/stats/statsmanager.cpp \
    $$PROJECT_PATH/stats/history.cpp \
    $$PROJECT_PATH/stats/securestatsfile.cpp \
    $$PROJECT_PATH/stats/historystatshelper.cpp \
