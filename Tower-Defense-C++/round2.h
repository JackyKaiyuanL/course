#ifndef ROUND2_H
#define ROUND2_H

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

class round2 : public QWidget
{
    Q_OBJECT
public:
    explicit round2(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void setwaypoint();
    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
signals:
    void gochoice2();
private:
    QPushButton quit2;
    QPushButton general2;
    towerbase *base1;
    towerbase *base2;
    towerbase *base3;
    towerbase *base4;
    towerbase *base5;
    towerbase *base6;
    towerbase *base7;
    towerbase *base8;
    QTimer  *timer;
    QMediaPlayer *groundmusic;
public:
    QList<Waypoint*> enemy_waypointlist2;
    QList<Enemy*>    enemylist2;
    QList<Tower*>    towerlist2;
    int				 round2_waves;
    int		   		 round2_playerHp;
    int				 round2_playrGold;
    bool             round2_ended;
    bool             round2_ifwin;
 private slots://QT的private槽 当前类及其子类可以将信号与之相连接
     void updateMap();
     void gamestart();
};

#endif // ROUND2_H
