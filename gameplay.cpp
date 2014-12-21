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

#include "gameplay.h"
#include <QGraphicsItem>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QEvent>
#include <QKeyEvent>
#include <QMessageBox>

#include <iostream>

Gameplay::Gameplay(QGraphicsScene & scene, QGraphicsItem *p1, QGraphicsItem *p2, QGraphicsItem *ball, QObject *parent):
    QObject(parent),
    iScene ( scene ),
    iP1 (p1),
    iP2 (p2),
    iBall (ball)
{
    iScene.addItem(iP1);
    iScene.addItem(iP2);
    iScene.addItem(iBall);

    defaultParams();

    iTimer = new QTimer(this);
    iTimer->setInterval(16);
    QObject::connect(iTimer, SIGNAL(timeout()), this, SLOT(tick()));
    Players = 1;

    QObject::connect(&iScene, SIGNAL(sceneRectChanged(const QRectF&)), this, SLOT(resize(const QRectF&)) );
}

void Gameplay::PauseGame(bool pause)
{
    if(pause)
    {
        Paused = true;
        iTimer->stop();
    }
    else
    {
        Paused = false;
        iTimer->start();
    }
}

void Gameplay::Restart()
{
    defaultParams();
}

void Gameplay::setPlayers(size_t pl)
{
    if(pl > 1)
    {
        Players = 2;
    }
    else
    {
        Players = 1;
    }
}

void Gameplay::tick()
{
    //ball direction
    qreal newX = iBall->pos().x() + iBallDirection.x();
    qreal newY = iBall->pos().y() + iBallDirection.y();

    //edges of ball
    if ( ( newX < 0 ) || ( newX + iBall->boundingRect().right() > iScene.sceneRect().right() ) )
    {
        emit goal(newX / abs(newX));
        iBallDirection.rx() *= -1;
    }

    if ( ( newY < 0 ) || ( newY + iBall->boundingRect().bottom() > iScene.sceneRect().bottom() ) )
    {
        iBallDirection.ry() *= -1;
    }

    //colisions
    if ( ( iP1->collidesWithItem(iBall) ) && ( iBallDirection.x() < 0 ) )
    {
        iBallDirection.rx() *= -1;
    }

    if ( ( iP2->collidesWithItem(iBall) ) && ( iBallDirection.x() > 0 ) )
    {
        iBallDirection.rx() *= -1;
    }

    //OP AI
    if(Players == 1)
    {
        qreal p2newx = iP2->pos().y() + iP2Direction;

        if ( ( p2newx < 0 ) || ( p2newx + iP2->boundingRect().top() > iScene.sceneRect().bottom() - 80 ) )
        {
            iP2Direction = 0;
        }

        if ( qrand() % 10 == 0 )
        {
            iP2Direction = calculateP2Direction();
        }
        iP2->moveBy(0, iP2Direction);
    }
    else
    {
        iP2->setPos(iP2->pos().x(), posY2);
    }


    //movement
    iBall->moveBy(iBallDirection.x(), iBallDirection.y());

    iP1->setPos(iP1->pos().x(), posY1);
}

qreal Gameplay::calculateP2Direction()
{
    qreal dir = 0;

    if ( iBall->pos().y() + iBallDirection.y() > iP2->sceneBoundingRect().top() )
    {
        // move up
        dir = 5;
    }
    else if ( iBall->pos().y() + iBallDirection.y() < iP2->sceneBoundingRect().bottom() )
    {
        // move down
        dir = -5;
    }

    return dir;
}

void Gameplay::defaultParams()
{
    iBallDirection = QPointF(-3,3);
    iP1Direction = 0;
    iP2Direction = 0;

    iP1->setPos(5, iScene.height()/2-40);
    iP2->setPos(iScene.width()-25, iScene.height()/2-40);
    iBall->setPos(iScene.width()/2-10, iScene.height()/2-10);
    Paused = true;
    posY1 = iScene.height()/2-40;
    posY2 = iScene.height()/2-40;
}

/*bool Gameplay::verifyDistance(glm::vec2 point1, glm::vec2 point2, float max)
{
    return (glm::distance(point1, point2) < max);
}*/

void Gameplay::setPoints(std::vector<glm::vec2> pts)
{
    if(!pts.empty())
    {
        posY1 = pts[0].y * iScene.height();
    }

    if(pts.size() > 1 && Players == 2)
    {
        if(pts[0].x > pts[1].x)
        {
            posY2 = pts[1].y * iScene.height();
        }
        else
        {
            posY1 = pts[1].y * iScene.height();
            posY2 = pts[0].y * iScene.height();
        }
    }

}

void Gameplay::resize(const QRectF &)
{
    iP1->setPos(5, iScene.height()/2-40);
    iP2->setPos(iScene.width()-25, iScene.height()/2-40);
}
