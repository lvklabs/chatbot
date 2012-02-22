#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lemmatizer.h"
#include <list>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->lemmatButton, SIGNAL(clicked()), SLOT(lemmatize()));
    connect(ui->input, SIGNAL(returnPressed()), SLOT(lemmatize()));
    connect(ui->input, SIGNAL(textEdited(QString)), SLOT(lemmatize()));

    m_lemmatizer =  new Lemmatizer();
}

MainWindow::~MainWindow()
{
    delete m_lemmatizer;
    delete ui;
}

void MainWindow::lemmatize()
{
    std::string input = ui->input->text().toStdString();

    if (input.length() > 0) {
        if (input[input.length() - 1] != '.') {
            input.append(".");
        }

        std::list<sentence> ls = m_lemmatizer->lemmatize(input);

        ui->output->setPlainText(convertToSring(ls));
    }
}

QString MainWindow::convertToSring(std::list<sentence> &ls)
{
    int nsentence = 0;
    std::ostringstream oss;

    for (list<sentence>::iterator is = ls.begin(); is != ls.end(); ++is, ++nsentence) {

        for (sentence::const_iterator w = is->begin(); w != is->end(); ++w) {
            if (w->get_form() != ".") {
                oss << w->get_form() << ":";
            } else {
                oss << std::endl << "--------------" << std::endl;
                continue;
            }

            for (word::const_iterator ait = w->selected_begin(); ait != w->selected_end(); ++ait) {
                if (ait != w->selected_begin()) {
                    oss << "  | ";
                }
                if (ait->is_retokenizable()) {
                    std::list<word> rtk = ait->get_retokenizable();
                    std::list<word>::iterator r;
                    std::string lem, par;
                    for (r = rtk.begin(); r != rtk.end(); r++) {
                        lem = lem + "+" + r->get_lemma();
                        par = par + "+" + r->get_parole();
                    }
                    oss << " " << lem.substr(1) << " " << par.substr(1) << " " << ait->get_prob();
                }
                else {
                    oss << " " << ait->get_lemma() << " " << ait-> get_parole() << " " << ait->get_prob();
                }
            }
            oss << std::endl;
        }
        oss << std::endl;
    }

    return QString(oss.str().c_str());
}
