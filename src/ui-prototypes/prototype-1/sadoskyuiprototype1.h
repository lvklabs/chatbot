#ifndef SADOSKYUIPROTOTYPE1_H
#define SADOSKYUIPROTOTYPE1_H

#include <QMainWindow>

namespace Ui {
    class SadoskyUIPrototype1;
}

class SadoskyUIPrototype1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit SadoskyUIPrototype1(QWidget *parent = 0);
    ~SadoskyUIPrototype1();

private:
    Ui::SadoskyUIPrototype1 *ui;
};

#endif // SADOSKYUIPROTOTYPE1_H
