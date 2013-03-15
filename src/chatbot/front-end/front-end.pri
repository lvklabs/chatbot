# Front-end module

QT += \
    gui

isEmpty(PROJECT_PATH) {
    PROJECT_PATH = .
}

HEADERS += \
    $$PROJECT_PATH/front-end/mainwindow.h \
    $$PROJECT_PATH/front-end/ruletreemodel.h \
    $$PROJECT_PATH/front-end/ruletreeview.h \
    $$PROJECT_PATH/front-end/ruleoutputwidget.h \
    $$PROJECT_PATH/front-end/ruleinputwidget.h \
    $$PROJECT_PATH/front-end/testconversationwidget.h \
    $$PROJECT_PATH/front-end/portdialog.h \
    $$PROJECT_PATH/front-end/exportdialog.h \
    $$PROJECT_PATH/front-end/importdialog.h \
    $$PROJECT_PATH/front-end/rosterwidget.h \
    $$PROJECT_PATH/front-end/autocompletetextedit.h \
    $$PROJECT_PATH/front-end/chathistorywidget.h \
    $$PROJECT_PATH/front-end/linefilteredit.h \
    $$PROJECT_PATH/front-end/ruletextview.h \
    $$PROJECT_PATH/front-end/scorewidget.h \
    $$PROJECT_PATH/front-end/tinyscorewidget.h \
    $$PROJECT_PATH/front-end/detailsdialog.h \
    $$PROJECT_PATH/front-end/welcomewidget.h \
    $$PROJECT_PATH/front-end/optionswindow.h \
    $$PROJECT_PATH/front-end/filedialog.h \
    $$PROJECT_PATH/front-end/mainwindowrefactor.h \
    $$PROJECT_PATH/front-end/sendscoredialog.h \
    $$PROJECT_PATH/front-end/newupdatedialog.h \
    $$PROJECT_PATH/front-end/updateexecutor.h \
    $$PROJECT_PATH/front-end/testinputtext.h \
    $$PROJECT_PATH/front-end/ruleeditwidget.h \
    $$PROJECT_PATH/front-end/connectionwidget.h \
    $$PROJECT_PATH/front-end/memberfunctor.h \
    $$PROJECT_PATH/front-end/uploaderprogressdialog.h \
    $$PROJECT_PATH/front-end/accountverifwidget.h\
    $$PROJECT_PATH/front-end/cluewidget.h \
    $$PROJECT_PATH/front-end/scriptcoveragewidget.h

SOURCES += \
    $$PROJECT_PATH/front-end/mainwindow.cpp \
    $$PROJECT_PATH/front-end/ruletreemodel.cpp \
    $$PROJECT_PATH/front-end/ruletreeview.cpp \
    $$PROJECT_PATH/front-end/ruleoutputwidget.cpp \
    $$PROJECT_PATH/front-end/ruleinputwidget.cpp \
    $$PROJECT_PATH/front-end/testconversationwidget.cpp \
    $$PROJECT_PATH/front-end/portdialog.cpp \
    $$PROJECT_PATH/front-end/exportdialog.cpp \
    $$PROJECT_PATH/front-end/importdialog.cpp \
    $$PROJECT_PATH/front-end/rosterwidget.cpp \
    $$PROJECT_PATH/front-end/autocompletetextedit.cpp \
    $$PROJECT_PATH/front-end/chathistorywidget.cpp \
    $$PROJECT_PATH/front-end/linefilteredit.cpp \
    $$PROJECT_PATH/front-end/ruletextview.cpp \
    $$PROJECT_PATH/front-end/scorewidget.cpp \
    $$PROJECT_PATH/front-end/tinyscorewidget.cpp \
    $$PROJECT_PATH/front-end/detailsdialog.cpp \
    $$PROJECT_PATH/front-end/optionswindow.cpp \
    $$PROJECT_PATH/front-end/welcomewidget.cpp \
    $$PROJECT_PATH/front-end/mainwindowrefactor.cpp \
    $$PROJECT_PATH/front-end/sendscoredialog.cpp \
    $$PROJECT_PATH/front-end/newupdatedialog.cpp \
    $$PROJECT_PATH/front-end/testinputtext.cpp \
    $$PROJECT_PATH/front-end/ruleeditwidget.cpp \
    $$PROJECT_PATH/front-end/connectionwidget.cpp \
    $$PROJECT_PATH/front-end/uploaderprogressdialog.cpp \
    $$PROJECT_PATH/front-end/accountverifwidget.cpp \
    $$PROJECT_PATH/front-end/cluewidget.cpp \
    $$PROJECT_PATH/front-end/scriptcoveragewidget.cpp

FORMS += \
    $$PROJECT_PATH/front-end/mainwindow.ui \
    $$PROJECT_PATH/front-end/portdialog.ui \
    $$PROJECT_PATH/front-end/chathistorywidget.ui \
    $$PROJECT_PATH/front-end/scorewidget.ui \
    $$PROJECT_PATH/front-end/tinyscorewidget.ui \
    $$PROJECT_PATH/front-end/detailsdialog.ui \
    $$PROJECT_PATH/front-end/optionswindow.ui \
    $$PROJECT_PATH/front-end/welcomewidget.ui \
    $$PROJECT_PATH/front-end/ruleeditwidget.ui \
    $$PROJECT_PATH/front-end/connectionwidget.ui \
    $$PROJECT_PATH/front-end/accountverifwidget.ui \
    $$PROJECT_PATH/front-end/cluewidget.ui \
    $$PROJECT_PATH/front-end/scriptcoveragewidget.ui


isEmpty( ICON_THEME ) {
    ICON_THEME = gnome
}

RESOURCES += $$PROJECT_PATH/res/$${ICON_THEME}_theme.qrc
