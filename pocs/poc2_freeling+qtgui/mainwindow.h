#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <list>
#include "freeling.h"

namespace Ui {
    class MainWindow;
}

class Lemmatizer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void lemmatize();

private:
    Ui::MainWindow *ui;
    Lemmatizer *m_lemmatizer;

    QString convertToSring(std::list<sentence> &ls);
};

#endif // MAINWINDOW_H
