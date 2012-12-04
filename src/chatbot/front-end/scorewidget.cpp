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

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline Lvk::Stats::Score makeScore(double sr, double sc, double sh, double st)
{
    Lvk::Stats::Score score;
    score.rules = sr;
    score.contacts = sc;
    score.conversations = sh;
    score.total = st;

    return score;
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

    connect(ui->uploadButton, SIGNAL(clicked()), SIGNAL(upload()));
}

//--------------------------------------------------------------------------------------------------

Lvk::FE::ScoreWidget::~ScoreWidget()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScoreWidget::setScore(const Stats::Score &score)
{
    ui->convPointsValue->setText(QString::number(score.conversations));
    ui->contactsPointsValue->setText(QString::number(score.contacts));
    ui->rulePointsValue->setText(QString::number(score.rules));
    ui->totalPointsValue->setText(QString::number(score.total));
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScoreWidget::setUploadEnabled(bool enabled)
{
    ui->uploadButton->setEnabled(enabled);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScoreWidget::setUploadVisible(bool visible)
{
    ui->uploadButton->setVisible(visible);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScoreWidget::setLabelsVisible(bool visible)
{
    ui->convPointsLabel->setVisible(visible);
    ui->contactsPointsLabel->setVisible(visible);
    ui->rulePointsLabel->setVisible(visible);
    ui->totalPointsLabel->setVisible(visible);
}

//--------------------------------------------------------------------------------------------------

void Lvk::FE::ScoreWidget::clear()
{
    setScore(makeScore(0,0,0,0));
}
