#include "tower.h"
#include "round1.h"
#include "round2.h"
#include <QDebug>
const QSize Tower::tower_fixedSize(80, 80);

Tower::Tower(QPoint pos, const QPixmap &sprite
             ,round1 *rou1,round2 *rou2)
    :tower_attacking(false)
    , tower_attackRange(150)
    , tower_damage(10)
    , tower_fireRate(1000)
    , tower_chooseEnemy(NULL)
    ,tower_firespeed(100)
    , tower_pos(pos)
    , tower_sprite(sprite)
    ,tower_ifadvance(false)
    ,tower_ifremove(false)
    ,tower_type(0)
{
    tower_fireRateTimer = new QTimer(this);
    connect(tower_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    if(rou1)
    {
        game1=rou1;
        game2=nullptr;
        tower_chooseroundtype=1;
    }
    else
    {
        game2=rou2;
        game1=nullptr;
        tower_chooseroundtype=2;
    }
}

Tower::~Tower()
{
    delete tower_fireRateTimer;
    tower_fireRateTimer = NULL;
}

king::king(QPoint pos, round1 *rou1,round2 *rou2,const QPixmap &sprite)
    : Tower(pos,sprite,rou1,rou2)
{
    tower_attackRange=150;
    tower_damage=5;
    tower_fireRate=1000;
    tower_chooseEnemy=nullptr;
    tower_firespeed=50;
    tower_fireRateTimer = new QTimer(this);
    tower_ifremove=false;
    tower_ifadvance=false;
    tower_type=1;
    connect(tower_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    if(rou1)
    {
        game1=rou1;
        game2=nullptr;
        tower_chooseroundtype=1;
    }
    else
    {
        game2=rou2;
        game1=nullptr;
        tower_chooseroundtype=2;
    }
}

king::~king()
{
    delete tower_fireRateTimer;
    tower_fireRateTimer = NULL;
}

kingplus::kingplus(QPoint pos, round1 *rou1,round2 *rou2,const QPixmap &sprite)
    : Tower(pos,sprite,rou1,rou2)
{
    tower_attackRange=200;
    tower_damage=10;
    tower_fireRate=1000;
    tower_chooseEnemy=nullptr;
    tower_firespeed=50;
    tower_ifremove=false;
    tower_ifadvance=false;
    tower_type=1;
    tower_fireRateTimer = new QTimer(this);
    connect(tower_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    if(rou1)
    {
        game1=rou1;
        game2=nullptr;
        tower_chooseroundtype=1;
    }
    else
    {
        game2=rou2;
        game1=nullptr;
        tower_chooseroundtype=2;
    }
}

kingplus::~kingplus()
{
    delete tower_fireRateTimer;
    tower_fireRateTimer = NULL;
}
queen::queen(QPoint pos, round1 *rou1,round2 *rou2,const QPixmap &sprite)
    : Tower(pos,sprite,rou1,rou2)
{
    tower_attackRange=150;
    tower_damage=10;
    tower_fireRate=1000;
    tower_chooseEnemy=nullptr;
    tower_firespeed=100;
    tower_ifremove=false;
    tower_ifadvance=false;
    tower_type=2;
    tower_fireRateTimer = new QTimer(this);
    connect(tower_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    if(rou1)
    {
        game1=rou1;
        game2=nullptr;
        tower_chooseroundtype=1;
    }
    else
    {
        game2=rou2;
        game1=nullptr;
        tower_chooseroundtype=2;
    }
}

queen::~queen()
{
    delete tower_fireRateTimer;
    tower_fireRateTimer = NULL;
}
queenplus::queenplus(QPoint pos, round1 *rou1,round2 *rou2,const QPixmap &sprite)
    : Tower(pos,sprite,rou1,rou2)
{
    tower_attackRange=200;
    tower_damage=10;
    tower_fireRate=1000;
    tower_chooseEnemy=nullptr;
    tower_firespeed=50;
    tower_ifremove=false;
    tower_ifadvance=false;
    tower_type=2;
    tower_fireRateTimer = new QTimer(this);
    connect(tower_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    if(rou1)
    {
        game1=rou1;
        game2=nullptr;
        tower_chooseroundtype=1;
    }
    else
    {
        game2=rou2;
        game1=nullptr;
        tower_chooseroundtype=2;
    }
}

queenplus::~queenplus()
{
    delete tower_fireRateTimer;
    tower_fireRateTimer = NULL;
}

QPoint Tower::getPosition()
{
    return tower_pos;
}

void Tower::checkEnemyInRange()
{
    if (tower_chooseEnemy)
    {
        qDebug()<<"inrangecheck1"<<endl;
        if (!collisionWithCircle(tower_pos, tower_attackRange, tower_chooseEnemy->pos(), 1)
                || tower_chooseEnemy->enemy_active==false)
        {
            qDebug()<<"inrangecheck2"<<endl;
            lostSightOfEnemy();
            qDebug()<<"inrangecheck3"<<endl;
        }
        else if (tower_chooseEnemy->enemy_currentHp<=0)
        {
            qDebug()<<"inrangecheck4"<<endl;
            //Enemy *enemydelete=tower_chooseEnemy;
            tower_chooseEnemy = NULL;
            tower_fireRateTimer->stop();
            //delete  enemydelete;//清除缓存
            qDebug()<<"inrangecheck5"<<endl;
        }//血量为零，不再继续攻击
    }
    else
    {
        // 遍历敌人,看是否有敌人在攻击范围内
        if(tower_chooseroundtype==1)
        {
            qDebug()<<"inrangecheck6"<<endl;
            QList<Enemy *> enemyList = game1->enemylist1;
            foreach (Enemy *enemy, enemyList)
            {
                if(enemy==nullptr)//空指针，一定要注意
                {
                    continue;
                }
                else if (collisionWithCircle(tower_pos, tower_attackRange, enemy->pos(), 1)
                        && enemy->enemy_currentHp >0
                        && enemy->enemy_active==true)
                {
                    chooseEnemyForAttack(enemy);
                    break;
                }
            }
            qDebug()<<"inrangecheck7"<<endl;
        }
        else if(tower_chooseroundtype==2)
        {
            QList<Enemy *> enemyList2 = game2->enemylist2;
            foreach (Enemy *enemy, enemyList2)
            {
                if(enemy==nullptr)//空指针，一定要注意
                {
                    continue;
                }
                if (collisionWithCircle(tower_pos, tower_attackRange, enemy->pos(), 1)
                        && enemy->enemy_currentHp >0
                        && enemy->enemy_active==true)
                {
                    chooseEnemyForAttack(enemy);
                    break;
                }
            }
        }
    }
}

void Tower::draw(QPainter *painter) //此处若为常函数，则子弹列表无法移除元素
{
    painter->save();
    painter->setPen(Qt::white);
    // 绘制攻击范围
    painter->drawEllipse(tower_pos, tower_attackRange, tower_attackRange);

    // 绘制炮塔并选择炮塔
    QPoint offsetpoint(tower_pos.x()-tower_fixedSize.width()/2
                       ,tower_pos.y()-tower_fixedSize.height()/2);
    painter->drawPixmap(offsetpoint.x(),offsetpoint.y(),
                        tower_fixedSize.width(),tower_fixedSize.height(),
                        tower_sprite);
    //绘制子弹
    foreach(Bullet *bullet,bulletlist)
    {
        if(bulletlist.empty())
        {
            break;
        }
        if(bullet->whetherused)
        {
            bulletlist.removeOne(bullet);
            delete bullet;
        }
        else if(bullet)
        {
            bullet->draw(painter);
        }
    }
    painter->restore();
}

void Tower::attackEnemy()
{
    tower_fireRateTimer->start(tower_fireRate);
}

void Tower::chooseEnemyForAttack(Enemy *enemy)
{
    tower_chooseEnemy = enemy;
    attackEnemy();
}

void king::shootWeapon()
{
    firebullet *bullet = new firebullet(tower_pos,
                                        tower_chooseEnemy->pos(),
                                        tower_chooseEnemy
                                        ,tower_damage
                                        ,tower_firespeed );
    bulletlist.push_back(bullet);
    bullet->move();

}
void kingplus::shootWeapon()
{
    firebullet *bullet = new firebullet(tower_pos,
                                        tower_chooseEnemy->pos(),
                                        tower_chooseEnemy
                                        ,tower_damage
                                        ,tower_firespeed );
    bulletlist.push_back(bullet);
    bullet->move();
}
void queen::shootWeapon()
{
    icebullet *bullet = new icebullet(tower_pos,
                                        tower_chooseEnemy->pos(),
                                        tower_chooseEnemy
                                        ,tower_damage
                                        ,tower_firespeed );
  bulletlist.push_back(bullet);
    bullet->move();
}
void queenplus::shootWeapon()
{
    icebullet *bullet = new icebullet(tower_pos,
                                        tower_chooseEnemy->pos(),
                                        tower_chooseEnemy
                                        ,tower_damage
                                        ,tower_firespeed );
    bulletlist.push_back(bullet);
    qDebug()<<"bulletinlist"<<endl;
    bullet->move();
}
void Tower::lostSightOfEnemy()
{
    if (tower_chooseEnemy)
    {
        tower_chooseEnemy = NULL;
    }
    tower_fireRateTimer->stop();
}


