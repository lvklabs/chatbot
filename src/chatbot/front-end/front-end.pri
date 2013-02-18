# Front-end module

QT += \
    gui \
    qt3support ### TODO remove this dep

HEADERS += \
    front-end/mainwindow.h \
    front-end/ruletreemodel.h \
    front-end/ruletreeview.h \
    front-end/ruleoutputwidget.h \
    front-end/ruleinputwidget.h \
    front-end/testconversationwidget.h \
    front-end/portdialog.h \
    front-end/exportdialog.h \
    front-end/importdialog.h \
    front-end/rosterwidget.h \
    front-end/autocompletetextedit.h \
    front-end/chathistorywidget.h \
    front-end/linefilteredit.h \
    front-end/ruletextview.h \
    front-end/scorewidget.h \
    front-end/tinyscorewidget.h \
    front-end/detailsdialog.h \
    front-end/welcomewidget.h \
    front-end/optionswindow.h \
    front-end/filedialog.h \
    front-end/mainwindowrefactor.h \
    front-end/sendscoredialog.h \
    front-end/newupdatedialog.h \
    front-end/updateexecutor.h \
    front-end/testinputtext.h \
    front-end/ruleeditwidget.h \
    front-end/connectionwidget.h \
    front-end/memberfunctor.h \
    front-end/uploaderprogressdialog.h \
    front-end/accountverifwidget.h\

SOURCES += \
    front-end/mainwindow.cpp \
    front-end/ruletreemodel.cpp \
    front-end/ruletreeview.cpp \
    front-end/ruleoutputwidget.cpp \
    front-end/ruleinputwidget.cpp \
    front-end/testconversationwidget.cpp \
    front-end/portdialog.cpp \
    front-end/exportdialog.cpp \
    front-end/importdialog.cpp \
    front-end/rosterwidget.cpp \
    front-end/autocompletetextedit.cpp \
    front-end/chathistorywidget.cpp \
    front-end/linefilteredit.cpp \
    front-end/ruletextview.cpp \
    front-end/scorewidget.cpp \
    front-end/tinyscorewidget.cpp \
    front-end/detailsdialog.cpp \
    front-end/optionswindow.cpp \
    front-end/welcomewidget.cpp \
    front-end/mainwindowrefactor.cpp \
    front-end/sendscoredialog.cpp \
    front-end/newupdatedialog.cpp \
    front-end/testinputtext.cpp \
    front-end/ruleeditwidget.cpp \
    front-end/connectionwidget.cpp \
    front-end/uploaderprogressdialog.cpp \
    front-end/accountverifwidget.cpp \

FORMS += \
    front-end/mainwindow.ui \
    front-end/portdialog.ui \
    front-end/chathistorywidget.ui \
    front-end/scorewidget.ui \
    front-end/tinyscorewidget.ui \
    front-end/detailsdialog.ui \
    front-end/optionswindow.ui \
    front-end/welcomewidget.ui \
    front-end/ruleeditwidget.ui \
    front-end/connectionwidget.ui \
    front-end/accountverifwidget.ui


isEmpty( ICON_THEME ) {
    ICON_THEME = gnome
}

RESOURCES += res/$${ICON_THEME}_theme.qrc
