#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <waypoint.h>

class Enemy : public QObject
{
    Q_OBJECT
public:
    Enemy(Waypoint *startWayPoint,
          const QPixmap &sprite = QPixmap(":/images/enemy1.png"),//image忘记加s,怪不得一直画不出敌人
          int maxHp = 40,qreal walkingSpeed = 1.0);
    //设置为虚函数
    virtual void draw(QPainter *painter) const;
    virtual void move();
    virtual void getDamage(int damage);
    QPoint pos() const;

public slots:
    void doActivate();

public:
    bool			enemy_active;
    int				enemy_maxHp;
    int				enemy_currentHp;
    double			enemy_walkingSpeed;

    QPoint			enemy_pos;
    Waypoint *		enemy_destinationWayPoint;

    const QPixmap	enemy_sprite;
    static const QSize enemy_fixedSize;

signals:

};
//该怪物血量减半，速度加倍
class Enemyplus:public Enemy
{
    Q_OBJECT
public:
    Enemyplus(Waypoint *startWayPoint,
          const QPixmap &sprite = QPixmap(":/images/enemy1plus.png"),
          int maxHp = 20,double walkingSpeed = 2.0);
};
//该怪物血量加倍，速度减半
class Enemy2:public Enemy
{
    Q_OBJECT
public:
    Enemy2(Waypoint *startWayPoint,
          const QPixmap &sprite = QPixmap(":/images/enemy2.png"),
          int maxHp = 40,qreal walkingSpeed = 0.5);
};
#endif // ENEMY_H
