/*
 *
 * Copyright (C) 2014  Miroslav Krajicek (https://github.com/kaajo) . All Rights Reserved.
 *
 * This file is part of WccAPI.
 *
 * WccAPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU LGPL version 3 as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WccAPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU  LGPL version 3
 * along with WccAPI.  If not, see <http://www.gnu.org/licenses/lgpl-3.0.txt>.
 *
 */

#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QMainWindow>
#include <QTime>

#include "webcamcap.h"

class Movement
{
    glm::vec3 direction;
    float size;
public:
    Movement(glm::vec3 dir, float sz){direction = dir; size = sz;}
};

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QMainWindow
{
    Q_OBJECT

    bool sendPositions, sendMovement;
    int timeout;
    float minSensitivity, minSize, minAcceleration, minSpeed;

    webcamcap::MyFifo *fifo;


    float difTemp;
    //2D section

    std::vector<glm::vec2> points, lastPoints, diff, lastDiff, sendPts;

    //move
    QTime *timer;
    float size;
    glm::vec2 startPoint, endPoint;

    //3D section

public:
    explicit ControlPanel(QWidget *parent = 0);
    ~ControlPanel();

    bool connectServer();
    void disconnectServer();

    webcamcap::MyFifo *getFifo() const {return fifo;}

    void setFifo(webcamcap::MyFifo *value) {fifo = value;}

private:
    Ui::ControlPanel *ui;

    void handlePositions(std::vector<glm::vec2> &pts);
    void handleMovements(std::vector<glm::vec2> &pts);

private slots:
    void handlePoints(std::vector<glm::vec2> pts);
    void handlePoints(std::vector<glm::vec3> pts);

    void on_positions_stateChanged(int arg1);

    void on_movement_stateChanged(int arg1);

    void on_sensitivity_valueChanged(double arg1);

    void on_speed_valueChanged(double arg1);

    void on_acceleration_valueChanged(double arg1);

    void on_size_valueChanged(double arg1);

    void on_timeout_valueChanged(int arg1);

signals:
    void movement(Movement move, size_t index);
    void pointsReady(std::vector<glm::vec2> pts);
    void pointsReady(std::vector<glm::vec3> pts);
};

#endif // CONTROLPANEL_H
