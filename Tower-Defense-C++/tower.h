#ifndef TOWER_H
#define TOWER_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>
#include "enemy.h"
#include "bullet.h"
#include "utility.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>
class Enemy;
class round1;
class round2;

class Tower : public QObject
{
    Q_OBJECT
public:
    Tower(QPoint pos,const QPixmap &sprite,round1 *rou1,round2 * rou2);
    //设置虚函数
    ~Tower();
    virtual void draw(QPainter *painter) ;
    virtual void checkEnemyInRange();
    virtual void attackEnemy();
    void chooseEnemyForAttack(Enemy *enemy);
    virtual void lostSightOfEnemy();

    QPoint getPosition();

public slots:
    virtual void shootWeapon()
    {
        return;
    }
public:
    bool			tower_attacking;
    int				tower_attackRange;	// 代表塔可以攻击到敌人的距离
    int				tower_damage;		// 代表攻击敌人时造成的伤害
    int				tower_fireRate;		// 代表再次攻击敌人的时间间隔
    round1 *        game1;
    round2 *        game2;
    Enemy *			tower_chooseEnemy;
    QTimer *		tower_fireRateTimer;
    int             tower_chooseroundtype;
    int             tower_firespeed;
    const QPoint	tower_pos;
    const QPixmap	tower_sprite;
    bool            tower_ifadvance;
    bool            tower_ifremove;
    int             tower_type;
    static const QSize tower_fixedSize;
public:
    QList<Bullet *> bulletlist;

signals:

};
class king:public Tower//使用firebullet
{
    Q_OBJECT
public:
    king(QPoint pos,round1 *rou1,round2 * rou2,
         const QPixmap &sprite=
            QPixmap(":/images/tower1.png"));
    ~king();
public slots:
    virtual void shootWeapon();
};
class kingplus:public Tower//升级后子弹威力加倍
{
    Q_OBJECT
public:

    kingplus(QPoint pos,round1 *rou1,round2 * rou2,
             const QPixmap &sprite=
                QPixmap(":/images/tower1plus.png"));
    ~kingplus();
public slots:
    virtual void shootWeapon();
};
class queen:public Tower//使用icebullet
{
    Q_OBJECT
public:
    queen(QPoint pos,round1 *rou1,round2 * rou2,
          const QPixmap &sprite=
             QPixmap(":/images/tower2.png"));//再次注意赋值形参一定要放在最后
    ~queen();
public slots:
    virtual void shootWeapon();
};
class queenplus:public Tower//升级后子弹速度加倍
{
    Q_OBJECT
public:
    queenplus(QPoint pos,round1 *rou1,round2 * rou2,
              const QPixmap &sprite=
                 QPixmap(":/images/tower2plus.png"));
    ~queenplus();
public slots:
    virtual void shootWeapon();
};

#endif // TOWER_H
