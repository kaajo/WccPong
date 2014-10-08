#include "gameplay.h"
#include <QGraphicsItem>
#include <QDebug>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QMessageBox>

Gameplay::Gameplay(QGraphicsScene & scene, QGraphicsItem *p1, QGraphicsItem *p2, QGraphicsItem *ball, QObject *parent) :
    QObject(parent),
    iScene ( scene ),
    iP1 ( p1 ),
    iP2 ( p2 ),
    iBall ( ball )
{
    defaultParams();

    iTimer = new QTimer(this);
    iTimer->setInterval(12);
    QObject::connect(iTimer, SIGNAL(timeout()), this, SLOT(tick()));
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


void Gameplay::tick()
{
    //smer lopty
    qreal newX = iBall->pos().x() + iBallDirection.x();
    qreal newY = iBall->pos().y() + iBallDirection.y();

    //smer oboch obdlznickov
    qreal pnewx = iP1->pos().y() + iP1Direction;
    qreal p2newx = iP2->pos().y() + iP2Direction;


    //hranice gulicky
    if ( ( newX < 0 ) || ( newX + iBall->boundingRect().right() > iScene.sceneRect().right() ) )
    {
        emit goal(newX / abs(newX));
        iBallDirection.rx() *= -1;
    }

    if ( ( newY < 0 ) || ( newY + iBall->boundingRect().bottom() > iScene.sceneRect().bottom() ) )
    {
        iBallDirection.ry() *= -1;
    }

    //hranice obdlznickov
    if ( ( pnewx < 0 ) || ( pnewx + iP1->boundingRect().top() > iScene.sceneRect().bottom()-80 ) )
    {
        iP1Direction = 0;
    }

    if ( ( p2newx < 0 ) || ( p2newx + iP2->boundingRect().top() > iScene.sceneRect().bottom()-80 ) )
    {
        iP2Direction = 0;
    }

    //kolizie
    if ( ( iP1->collidesWithItem(iBall) ) && ( iBallDirection.x() < 0 ) )
    {
        iBallDirection.rx() *= -1;
    }

    if ( ( iP2->collidesWithItem(iBall) ) && ( iBallDirection.x() > 0 ) )
    {
        iBallDirection.rx() *= -1;
    }

    //OP AI
    if ( qrand() % 10 == 0 )
    {
        iP2Direction = calculateP2Direction();
    }

    //pohyb
    iBall->moveBy(iBallDirection.x(), iBallDirection.y());
    iP1->moveBy(0, iP1Direction);
    iP2->moveBy(0, iP2Direction);
}

bool Gameplay::eventFilter(QObject *target, QEvent *e)
{
    Q_UNUSED(target);

    bool handled = false;
    if ( e->type() == QEvent::KeyPress )
    {
        QKeyEvent *keyEvent = (QKeyEvent *)e;

        if ( keyEvent->key() == Qt::Key_W )
        {
            iP1Direction = -5;
            handled = true;
        }
        else if ( keyEvent->key() == Qt::Key_S)
        {
            iP1Direction  = 5;
            handled = true;
        }
    }

    return handled;
}

qreal Gameplay::calculateP2Direction()
{
    qreal dir = 0;

    if ( iBall->pos().y() + iBallDirection.y() > iP2->sceneBoundingRect().top() )
    {
        // move right
        dir = 5;
    }
    else if ( iBall->pos().y() + iBallDirection.y() < iP2->sceneBoundingRect().bottom() )
    {
        // move left
        dir = -5;
    }

    return dir;

}

void Gameplay::defaultParams()
{
    iBallDirection = QPointF(-3,3);
    iP1Direction = 0;
    iP2Direction = 0;

    iScene.setSceneRect(0, 0, 350, 320);
    iScene.addItem(iP1);
    iScene.addItem(iP2);
    iScene.addItem(iBall);

    iP1->setPos(5, 135);
    iP2->setPos(325, 135);
    iBall->setPos(150, 150);
    Paused = true;
    Players = 1;
}

void Gameplay::calib()
{
    setPlayers();
}

void Gameplay::setPlayers()
{
    QMessageBox msgBox;
    msgBox.addButton(QString("1 Player"), QMessageBox::AcceptRole);
    msgBox.addButton(QString("2 PLayers"), QMessageBox::RejectRole);

    int selection = msgBox.exec();

    if (selection == QMessageBox::AcceptRole)
    {
        Players = 1;
    }
    else if (selection == QMessageBox::RejectRole)
    {
        Players = 2;
    }
}
