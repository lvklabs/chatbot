#include <QApplication>
#include "sadoskyuiprototype1.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SadoskyUIPrototype1 ui;
    ui.show();

    return a.exec();

}
