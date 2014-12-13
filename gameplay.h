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

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <QObject>
#include <QGraphicsScene>
#include <QTime>

#include <glm/glm.hpp>

#include <webcamcap.h>

class QGraphicsItem;
class QTimer;

class Gameplay : public QObject
{
    Q_OBJECT

    QGraphicsScene & iScene;
    QGraphicsItem *iP1, *iP2, *iBall;
    QTimer *iTimer;

    QPointF iBallDirection;
    qreal iP1Direction;
    qreal iP2Direction;

    qreal lastposY1, posY1;
    qreal lastposY2, posY2;

    bool Paused;
    size_t Players;

    webcamcap::MyFifo *Fifo;

    std::vector<glm::vec2> Points;

public:
    explicit Gameplay(QGraphicsScene & scene, QGraphicsItem *p1, QGraphicsItem *p2, QGraphicsItem *ball, QObject *parent,
                      webcamcap::MyFifo *fifo);

    void PauseGame(bool pause);
    void Restart();

    void setPlayers(size_t pl);
protected:

signals:
    void goal(int player);

private slots:
    void tick();
    void setPoints(std::vector<glm::vec2> pts);
    void resize(const QRectF&);

private:
    qreal calculateP2Direction();
    void defaultParams();
    void analyzeMessage();
    bool verifyDistance(glm::vec2 point1, glm::vec2 point2, float max);
};

#endif // GAMEPLAY_H
