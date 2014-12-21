/*
 *
 * Copyright (C) 2014  Miroslav Krajicek (https://github.com/kaajo) . All Rights Reserved.
 *
 * This file is part of WccPong.
 *
 * WccPong is free software: you can redistribute it and/or modify
 * it under the terms of the GNU LGPL version 3 as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WccPong is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU  LGPL version 3
 * along with WccPong.  If not, see <http://www.gnu.org/licenses/lgpl-3.0.txt>.
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include "gameplay.h"
#include <QPen>
#include <QResizeEvent>
#include <QDebug>
#include <QMessageBox>
#include <QObject>


#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    iScore1(0), iScore2(0)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, ui->boardView->size().width()-30, ui->boardView->size().height()-30);

    QGraphicsRectItem *p1 = new QGraphicsRectItem(0, 0, 20, 80);
    p1->setBrush(QBrush(Qt::blue));
    QGraphicsRectItem *p2 = new QGraphicsRectItem(0, 0, 20, 80);
    p2->setBrush(QBrush(Qt::green));

    QGraphicsEllipseItem *ball = new QGraphicsEllipseItem(0, 0, 15, 15);
    ball->setBrush(QBrush(Qt::magenta));

    ui->boardView->setScene(scene);

    iLoop = new Gameplay(*scene, p1, p2, ball, this);
    QSize m(scene->sceneRect().size().width() + 10, scene->sceneRect().size().height() + 10);
    ui->boardView->setMinimumSize(m);
    ui->boardView->installEventFilter(iLoop);

    QObject::connect(iLoop, SIGNAL(goal(int)),
                     this, SLOT(addScore(int)));


    panel =  new ControlPanel(this);
    connect(panel, SIGNAL(pointsReady(std::vector<glm::vec2>)), iLoop, SLOT(setPoints(std::vector<glm::vec2>)));

    panel->show();
}

MainWindow::~MainWindow()
{
    delete panel;
    delete iLoop;
    delete ui;
}

void MainWindow::addScore(int count)
{
    if(count < 0)
    {
        iScore2++;
        ui->lcdNumber2->display(iScore2);
    }
    else
    {
        iScore1++;
        ui->lcdNumber->display(iScore1);
    }
}


void MainWindow::on_checkBox_clicked(bool checked)
{
    if(checked)
    {
        if(!panel->connectServer())
        {
            QMessageBox::warning(this, QString("Warning"), QString("Could not connect to main WebCamCap program"));
            ui->checkBox->setChecked(false);
        }
    }
    else
    {
        panel->disconnectServer();
    }
}

void MainWindow::on_Start_clicked(bool checked)
{
    if(checked)
    {
        ui->Start->setText(QString("Pause"));
        iLoop->PauseGame(false);
    }
    else
    {
        ui->Start->setText(QString("Start"));
        iLoop->PauseGame(true);
    }
}

void MainWindow::on_NewGame_clicked()
{
    on_Start_clicked(false);
    ui->Start->setChecked(false);
    iLoop->Restart();

    iScore1 = 0;
    iScore2 = 0;
}

void MainWindow::on_OnePlayer_pressed()
{
    iLoop->setPlayers(1);
}

void MainWindow::on_TwoPlayers_pressed()
{
    iLoop->setPlayers(2);
}
