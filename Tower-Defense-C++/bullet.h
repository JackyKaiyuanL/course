#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QPixmap>
#include <QPainter>
#include <enemy.h>
#include <QPoint>
#include <QPropertyAnimation>

class Bullet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint bullet_currentPos READ currentPos WRITE setCurrentPos)
    //定义宏
public:
    Bullet(QPoint startPos, QPoint targetPoint, Enemy *target,
               int dama,int speed,
            const QPixmap &sprite
               );
    //在这里遇到一个BUG，解决了很久，形参赋值一定要放在最后！！！

    void draw(QPainter *painter) const;
    void move();
    void setCurrentPos(QPoint pos);
    QPoint currentPos() const;

public slots:
    virtual void hittarget()
    {
        return;
    }

public:
    const QPoint	bullet_startPos;
    const QPoint	bullet_targetPos;
    const QPixmap	bullet_sprite;
    QPoint			bullet_currentPos;
    Enemy *			bullet_target;
    int             bullet_damage;
    int             bullet_speed;
    bool            whetherused;

    static const QSize Bullet_fixedSize;

};
//king炮塔专用子弹，速度快，伤害一般，低于冰子弹
class firebullet : public Bullet
{
    Q_OBJECT
    Q_PROPERTY(QPoint bullet_currentPos READ currentPos WRITE setCurrentPos)
    //定义宏
public:
    firebullet(QPoint startPos, QPoint targetPoint, Enemy *target,
               int dama,int speed,
            const QPixmap &sprite = QPixmap(":/images/firebullet.png")
               );
public slots:
    virtual void hittarget();

};
//queen炮塔专用子弹，速度低于火子弹，威力大
class icebullet : public Bullet
{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)
    //定义宏
public:
    icebullet(QPoint startPos, QPoint targetPoint, Enemy *target,
              int dama,int speed,
            const QPixmap &sprite = QPixmap(":/images/icebullet.png")
              );
public slots:
    virtual void hittarget();

};
#endif // BULLET_H
