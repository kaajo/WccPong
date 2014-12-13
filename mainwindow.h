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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "webcamcap.h"

class Gameplay;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    Gameplay *iLoop;
    int iScore1;
    int iScore2;

    webcamcap::MyFifo fifo;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void addScore(int count);

    void on_checkBox_clicked(bool checked);

    void on_Start_clicked(bool checked);

    void on_NewGame_clicked();
    
    void on_OnePlayer_pressed();

    void on_TwoPlayers_pressed();

private:
    
};

#endif // MAINWINDOW_H
