#ifndef ROUND1_H
#define ROUND1_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QPaintEvent>
#include <QPixmap>
#include <QList>
#include <QTimer>
#include "towerbase.h"
#include "waypoint.h"
#include "enemy.h"
#include "tower.h"
#include <QMediaPlayer>


class round1 : public QWidget
{
    Q_OBJECT
public:
    explicit round1(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void setwaypoint();
    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
signals:
    void gochoice1();
private:
    QPushButton quit1;
    QPushButton general1;
    towerbase *base1;
    towerbase *base2;
    towerbase *base3;
    towerbase *base4;
    towerbase *base5;
    towerbase *base6;
    towerbase *base7;
    towerbase *base8;
    QTimer *timer;
    QMediaPlayer *groundmusic;
public:
    QList<Waypoint*> enemy_waypointlist1;
    QList<Enemy*>    enemylist1;
    QList<Tower*>    towerlist1;
    int				 round1_waves;
    int		   		 round1_playerHp;
    int				 round1_playrGold;
    bool             round1_ended;
    bool             round1_ifwin;
 private slots://QT的private槽 当前类及其子类可以将信号与之相连接
     void updateMap();
     void gamestart();
};

#endif // ROUND1_H
