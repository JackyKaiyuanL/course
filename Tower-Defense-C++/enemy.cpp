#include "enemy.h"
#include "waypoint.h"
#include "utility.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
#include <QtMath>
// 实现敌人类
static const int Health_Bar_Width = 20;

const QSize Enemy::enemy_fixedSize(50, 50);

Enemy::Enemy(Waypoint *startWayPoint,
             const QPixmap &sprite,
             int maxHp, double walkingSpeed)
    : QObject(0)
    , enemy_active(false)
    , enemy_maxHp(maxHp)
    ,enemy_currentHp(maxHp)
    ,enemy_walkingSpeed(walkingSpeed)
    , enemy_pos(startWayPoint->pos())
    , enemy_destinationWayPoint(startWayPoint->nextWayPoint())
    , enemy_sprite(sprite)
{
}

void Enemy::doActivate()
{
    enemy_active = true;
}

void Enemy::move()
{
    if (enemy_active==0)//没有激活的怪不能动
        return;

    if (collisionWithCircle(enemy_pos, 1, enemy_destinationWayPoint->pos(), 1))
    {
        // 敌人抵达了一个航点
        if (enemy_destinationWayPoint->nextWayPoint())
        {
            // 还有下一个航点
            enemy_pos = enemy_destinationWayPoint->pos();
            enemy_destinationWayPoint = enemy_destinationWayPoint->nextWayPoint();
        }
        else
        {
            // 表示进入基地,将在主界面的绘图事件里进行相关操作
            enemy_active=false;
            enemy_destinationWayPoint=nullptr;
            return;
        }
    }

    // 还在前往航点的路上
    // 目标航点的坐标
    QPoint targetPoint = enemy_destinationWayPoint->pos();
    // 向量标准化
    qreal movementSpeed = enemy_walkingSpeed;
    QVector2D normalized(targetPoint - enemy_pos);
    normalized.normalize();
    enemy_pos = enemy_pos + normalized.toPoint() * movementSpeed;
}

void Enemy::draw(QPainter *painter) const
{
    if (enemy_active==0)
    {
        return;
    }
    painter->save();

    QPoint healthBarPoint = enemy_pos + QPoint(-enemy_fixedSize.width() / 2 , -enemy_fixedSize.height()/2-10);
    // 绘制血条,红绿两种血条叠加效果
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 5));
    painter->drawRect(healthBarBackRect);
    if(enemy_currentHp>0)//血量大于零
    {
        painter->setBrush(Qt::green);
        QRect healthBarRect(healthBarPoint, QSize((double)enemy_currentHp / enemy_maxHp * Health_Bar_Width, 5));
        painter->drawRect(healthBarRect);
    }
    // 绘制偏转坐标,由中心+偏移=左上,绘制敌人
    QPoint offsetpoint(enemy_pos.x()-enemy_fixedSize.width()/2
                       ,enemy_pos.y()-enemy_fixedSize.height()/2);
    painter->drawPixmap(offsetpoint.x(),offsetpoint.y(),
                        enemy_fixedSize.width(),enemy_fixedSize.height(),
                        enemy_sprite);
    qDebug()<<"enemypaintdone"<<endl;
    painter->restore();
}

void Enemy::getDamage(int damage)
{
    enemy_currentHp -= damage;
}


QPoint Enemy::pos() const
{
    return enemy_pos;
}

//Enemyplus------
Enemyplus::Enemyplus(Waypoint *startWayPoint,
             const QPixmap &sprite,
             int maxHp, double walkingSpeed)
    :Enemy(startWayPoint,sprite,maxHp,walkingSpeed)
{
}
//Enemy2-------
Enemy2::Enemy2(Waypoint *startWayPoint,
             const QPixmap &sprite,
             int maxHp, double walkingSpeed)
    :Enemy(startWayPoint,sprite,maxHp,walkingSpeed)
{
}
