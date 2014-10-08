#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <QObject>
#include <QGraphicsScene>

#include <glm/glm.hpp>

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

    bool Paused;
    size_t Players;

    //calibration info
    glm::vec3 p1Top, p1Bottom, p2Top, p2Bottom;

public:
    explicit Gameplay(QGraphicsScene & scene, QGraphicsItem *p1, QGraphicsItem *p2, QGraphicsItem *ball, QObject *parent);

    void PauseGame(bool pause);
    void Restart();
protected:
    virtual bool eventFilter(QObject *, QEvent *);

signals:
    void goal(int player);

private slots:
    void tick();

private:
    qreal calculateP2Direction();
    void defaultParams();
    void calib();
    void setPlayers();

};

#endif // GAMEPLAY_H
