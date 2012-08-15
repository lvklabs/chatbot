/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Chatbot.
 *
 * LVK Chatbot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Chatbot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Chatbot.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "scorewidget.h"
#include "ui_scorewidget.h"

#define PARTIAL_SCORE_HTML_FORMAT "<span style=\"/*font-size:12pt*/;\">%1: </span>"\
                                  "<span style=\"/*font-size:12pt*/; color:#d30000;\">%2</span>"
#define TOTAL_SCORE_HTML_FORMAT   "<span style=\"font-size:16pt;\">%1: </span>"\
                                  "<span style=\"font-size:16pt; color:#d30000;\">%2</span>"

//#define SCORES_HTML_FORMAT        "<p>%1</p><p>%2</p><p>%3</p><p>%4</p>"
#define SCORES_HTML_FORMAT        "<p>%1</p><p>%2</p><p>%3</p>"


//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline Lvk::BE::Score makeScore(double sr, double sc, double sh, double st)
{
    Lvk::BE::Score score;
    score.rules = sr;
    score.connection = sc;
    score.history = sh;
    score.total = st;

    return score;
}

//--------------------------------------------------------------------------------------------------

inline QString getHtmlPartialScore(const QString &scoreStr, unsigned score)
{
    return QString(PARTIAL_SCORE_HTML_FORMAT).arg(scoreStr, QString::number(score));
}

//--------------------------------------------------------------------------------------------------

inline QString getHtmlTotalScore(const QString &scoreStr, unsigned score)
{
    return QString(TOTAL_SCORE_HTML_FORMAT).arg(scoreStr, QString::number(score));
}

//--------------------------------------------------------------------------------------------------

inline QString getHtmlScore(const Lvk::BE::Score &score)
{
    return QString(SCORES_HTML_FORMAT).arg(
                getHtmlPartialScore(QObject::tr("Rule defintions score"), score.rules),
                //getHtmlPartialScore(QObject::tr("Chatbot connection score"), score.connection),
                getHtmlPartialScore(QObject::tr("Chatbot conversations score"), score.history),
                getHtmlTotalScore(QObject::tr("Total score"), score.total));
}

} // namespace


//--------------------------------------------------------------------------------------------------
// ScoreWidget
//--------------------------------------------------------------------------------------------------

Lvk::FE::ScoreWidget::ScoreWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScoreWidget)
{
    ui->setupUi(this);
    setScore(makeScore(0,0,0,0));
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::ScoreWidget::~ScoreWidget()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScoreWidget::setScore(const BE::Score &score)
{
    ui->textEdit->setHtml(getHtmlScore(score));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScoreWidget::clear()
{
    setScore(makeScore(0,0,0,0));
}

