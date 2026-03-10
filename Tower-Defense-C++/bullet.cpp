#include "bullet.h"
#include <QDebug>

const QSize Bullet::Bullet_fixedSize(10, 10);

Bullet::Bullet(QPoint startPos,
                       QPoint targetPoint,
                       Enemy *target,int dama,int speed,
               const QPixmap &sprite)
    : bullet_startPos(startPos)
    ,bullet_targetPos(targetPoint)
    , bullet_sprite(sprite)
    , bullet_currentPos(startPos)
    , bullet_target(target)
    ,bullet_damage(dama)
    ,bullet_speed(speed)
    ,whetherused(false)
{}
void Bullet::setCurrentPos(QPoint pos)
{
    bullet_currentPos = pos;
}

QPoint Bullet::currentPos() const
{
    return bullet_currentPos;
}
void Bullet::draw(QPainter *painter) const
{
    painter->drawPixmap(bullet_currentPos, bullet_sprite);
}

void Bullet::move()
{
    static const int duration = bullet_speed;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "bullet_currentPos");
    animation->setDuration(duration);
    animation->setStartValue(bullet_startPos);
    animation->setEndValue(bullet_targetPos);
    connect(animation, SIGNAL(finished()), this, SLOT(hittarget()));//槽函数拼写错误的BUG

    animation->start();
    qDebug()<<"bulletmovedout"<<endl;
}

firebullet::firebullet(QPoint startPos,
                       QPoint targetPoint,
                       Enemy *target,int dama,int speed,
               const QPixmap &sprite)
    : Bullet(startPos,targetPoint,target,dama,speed,sprite)
{
    whetherused=false;
}
icebullet::icebullet(QPoint startPos,
                       QPoint targetPoint,
                       Enemy *target,int dama,int speed,
               const QPixmap &sprite)
    : Bullet(startPos,targetPoint,target,dama,speed,sprite)
{
    whetherused=false;
}
void firebullet::hittarget()
{
    qDebug()<<"bullethit"<<endl;
    bullet_target->getDamage(bullet_damage);
    whetherused=true;
}
void icebullet::hittarget()
{
    bullet_target->getDamage(bullet_damage);
    whetherused=true;
}
