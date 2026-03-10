#include "round1.h"
#include <QDebug>


static const int kingCost = 100;//设定每安置一个国王炮塔花费100金币
static const int queenCost =150;//设定每安置一个王后炮塔花费100金币
static const int UpdateTowerCost = 200;//设定升级炮塔花费200金币
static const int removevalue=50; //炮塔回收增加50金币

round1::round1(QWidget *parent) : QWidget(parent)
  ,round1_waves(1)
  ,round1_playerHp(30)
  ,round1_playrGold(10000)
  ,round1_ended(false)
  ,round1_ifwin(true)
{
    this->setFixedSize(800,800);
    //设置背景音乐
    groundmusic = new QMediaPlayer(this);
    groundmusic->setMedia(QUrl("qrc:/music/backgroundsound.mp3"));
    groundmusic->setVolume(10);
    groundmusic->play();

    //设置退出按钮
    quit1.setParent(this);
    quit1.setStyleSheet("background-color:lightgreen;");
    quit1.setText("Quit");
    quit1.move(720,740);
    quit1.resize(80,60);
    connect(&quit1,&QPushButton::clicked,[=](){
        this->hide();
        emit gochoice1();
    });
    //设置终点图像，label和button都可以显示图像，前面已包含button，所以这里也采用button
    general1.setParent(this);
    general1.move(700,300);
    general1.setFixedSize(100,100);
    general1.setStyleSheet("QPushButton{border-image: "
                  "url(:/images/general.png)}");
    //设置炮塔基座
    base1 = new towerbase;
    base1->setParent(this);
    base1->move(100,100);
    base2 = new towerbase;
    base2->setParent(this);
    base2->move(300,100);
    base3 = new towerbase;
    base3->setParent(this);
    base3->move(500,100);
    base4 = new towerbase;
    base4->setParent(this);
    base4->move(600,100);
    base5 = new towerbase;
    base5->setParent(this);
    base5->move(100,500);
    base6 = new towerbase;
    base6->setParent(this);
    base6->move(300,500);
    base7 = new towerbase;
    base7->setParent(this);
    base7->move(500,500);
    base8 = new towerbase;
    base8->setParent(this);
    base8->move(600,500);
    //炮塔列表设置空指针数量为8
    for(int i=0;i<8;i++)
    {
        //towerlist1[i]=nullptr;造成数组越界，错误,[]索引一定要在列表有元素后才可进行
        towerlist1.push_back(nullptr);
    }
    //设置基座信号接收机制
    connect(base1,&towerbase::setking,this,[=](){
        if(round1_playrGold>=kingCost && towerlist1[0]==nullptr)
        {
            Tower *tower= new king(QPoint(150,250), this,nullptr);
            towerlist1[0]=tower;
            round1_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base1,&towerbase::setqueen,this,[=](){
        if(round1_playrGold>=queenCost && towerlist1[0]==nullptr)
        {
            Tower *tower= new queen(QPoint(150,250), this,nullptr);
            towerlist1[0]=tower;
            round1_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base1,&towerbase::advance,this,[=](){
             if(round1_playrGold>=200 && towerlist1[0])
             {
                 towerlist1[0]->tower_ifadvance=true;
                 round1_playrGold-=200;
             }
            });
    connect(base1,&towerbase::remove,this,[=](){
        if(towerlist1[0])
             {
                towerlist1[0]->tower_ifremove=true;
                round1_playrGold+=50;
            }
            });
    connect(base2,&towerbase::setking,this,[=](){
        if(round1_playrGold>=kingCost && towerlist1[1]==nullptr)
        {
            Tower *tower= new king(QPoint(350,250), this,nullptr);
            towerlist1[1]=tower;
            round1_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base2,&towerbase::setqueen,this,[=](){
        if(round1_playrGold>=queenCost && towerlist1[1]==nullptr)
        {
            Tower *tower= new queen(QPoint(350,250), this,nullptr);
            towerlist1[1]=tower;
            round1_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base2,&towerbase::advance,this,[=](){
        if(round1_playrGold>=200 && towerlist1[1])
        {
            towerlist1[1]->tower_ifadvance=true;
            round1_playrGold-=200;
        }
            });
    connect(base2,&towerbase::remove,this,[=](){
        if(towerlist1[1])
             {
                towerlist1[1]->tower_ifremove=true;
                round1_playrGold+=50;
            }
            });
    connect(base3,&towerbase::setking,this,[=](){
        if(round1_playrGold>=kingCost && towerlist1[2]==nullptr)
        {
            Tower *tower= new king(QPoint(550,250), this,nullptr);
            towerlist1[2]=tower;
            round1_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base3,&towerbase::setqueen,this,[=](){
        if(round1_playrGold>=queenCost && towerlist1[2]==nullptr)
        {
            Tower *tower= new queen(QPoint(550,250), this,nullptr);
            towerlist1[2]=tower;
            round1_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base3,&towerbase::advance,this,[=](){
        if(round1_playrGold>=200 && towerlist1[2])
        {
            towerlist1[2]->tower_ifadvance=true;
            round1_playrGold-=200;
        }
            });
    connect(base3,&towerbase::remove,this,[=](){
        if(towerlist1[2])
             {
            towerlist1[2]->tower_ifremove=true;
             round1_playrGold+=50;
        }
            });
    connect(base4,&towerbase::setking,this,[=](){
        if(round1_playrGold>=kingCost && towerlist1[3]==nullptr)
        {
            Tower *tower= new king(QPoint(650,250), this,nullptr);
            towerlist1[3]=tower;
            round1_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base4,&towerbase::setqueen,this,[=](){
        if(round1_playrGold>=queenCost && towerlist1[3]==nullptr)
        {
            Tower *tower= new queen(QPoint(650,250), this,nullptr);
            towerlist1[3]=tower;
            round1_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base4,&towerbase::advance,this,[=](){
        if(round1_playrGold>=200 && towerlist1[3])
        {
            towerlist1[3]->tower_ifadvance=true;
            round1_playrGold-=200;
        }
            });
    connect(base4,&towerbase::remove,this,[=](){
        if(towerlist1[3])
        {
            towerlist1[3]->tower_ifremove=true;
             round1_playrGold+=50;
        }
            });
    connect(base5,&towerbase::setking,this,[=](){
        if(round1_playrGold>=kingCost && towerlist1[4]==nullptr)
        {
            Tower *tower= new king(QPoint(150,450), this,nullptr);
            towerlist1[4]=tower;
            round1_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base5,&towerbase::setqueen,this,[=](){
        if(round1_playrGold>=queenCost && towerlist1[4]==nullptr)
        {
            Tower *tower= new queen(QPoint(150,450), this,nullptr);
            towerlist1[4]=tower;
            round1_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base5,&towerbase::advance,this,[=](){
        if(round1_playrGold>=200 && towerlist1[4])
        {
            towerlist1[4]->tower_ifadvance=true;
            round1_playrGold-=200;
        }
            });
    connect(base5,&towerbase::remove,this,[=](){
        if(towerlist1[4])
             {
               towerlist1[4]->tower_ifremove=true;
               round1_playrGold+=50;
        }
            });
    connect(base6,&towerbase::setking,this,[=](){
        if(round1_playrGold>=kingCost && towerlist1[5]==nullptr)
        {
            Tower *tower= new king(QPoint(350,450), this,nullptr);
            towerlist1[5]=tower;
            round1_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base6,&towerbase::setqueen,this,[=](){
        if(round1_playrGold>=queenCost && towerlist1[5]==nullptr)
        {
            Tower *tower= new queen(QPoint(350,450), this,nullptr);
            towerlist1[5]=tower;
            round1_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base6,&towerbase::advance,this,[=](){
        if(round1_playrGold>=200 && towerlist1[5])
        {
            towerlist1[5]->tower_ifadvance=true;
            round1_playrGold-=200;
        }
            });
    connect(base6,&towerbase::remove,this,[=](){
        if(towerlist1[5])
             {towerlist1[5]->tower_ifremove=true;
             round1_playrGold+=50;}
            });
    connect(base7,&towerbase::setking,this,[=](){
        if(round1_playrGold>=kingCost && towerlist1[6]==nullptr)
        {
            Tower *tower= new king(QPoint(550,450), this,nullptr);
            towerlist1[6]=tower;
            round1_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base7,&towerbase::setqueen,this,[=](){
        if(round1_playrGold>=queenCost && towerlist1[6]==nullptr)
        {
            Tower *tower= new queen(QPoint(550,450), this,nullptr);
            towerlist1[6]=tower;
            round1_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base7,&towerbase::advance,this,[=](){
        if(round1_playrGold>=200 && towerlist1[6])
        {
            towerlist1[6]->tower_ifadvance=true;
            round1_playrGold-=200;
        }
            });
    connect(base7,&towerbase::remove,this,[=](){
        if(towerlist1[6])
             {towerlist1[6]->tower_ifremove=true;
             round1_playrGold+=50;}
            });
    connect(base8,&towerbase::setking,this,[=](){
        if(round1_playrGold>=kingCost && towerlist1[7]==nullptr)
        {
            Tower *tower= new king(QPoint(650,450), this,nullptr);
            towerlist1[7]=tower;
            round1_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base8,&towerbase::setqueen,this,[=](){
        if(round1_playrGold>=queenCost && towerlist1[7]==nullptr)
        {
            Tower *tower= new queen(QPoint(650,450), this,nullptr);
            towerlist1[7]=tower;
            round1_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base8,&towerbase::advance,this,[=](){
        if(round1_playrGold>=200 && towerlist1[7])
        {
            towerlist1[7]->tower_ifadvance=true;
            round1_playrGold-=200;
        }
            });
    connect(base8,&towerbase::remove,this,[=](){
        if(towerlist1[7])
             {towerlist1[7]->tower_ifremove=true;
             round1_playrGold+=50;}
            });

    //添加怪物航点
    setwaypoint();
    //这里对怪物列表进行填充
    for(int i=0;i<8;i++)
    {
        if(i<6)
        {
            Enemy *enemy=new Enemy(this->enemy_waypointlist1[0]);
            enemylist1.push_back(enemy);
        }
        else
        {
            Enemy *enemy=new Enemyplus(this->enemy_waypointlist1[0]);
            enemylist1.push_back(enemy);
        }
    }
    for(int i=8;i<16;i++)
    {
        if(i<12)
        {
            Enemy *enemy=new Enemy(this->enemy_waypointlist1[0]);
            enemylist1.push_back(enemy);
        }
        else
        {
            Enemy *enemy=new Enemy2(this->enemy_waypointlist1[0]);
            enemylist1.push_back(enemy);
        }
    }
    for(int i=16;i<24;i++)
    {
        if(i<20)
        {
            Enemy *enemy=new Enemyplus(this->enemy_waypointlist1[0]);
            enemylist1.push_back(enemy);
        }
        else
        {
            Enemy *enemy=new Enemy2(this->enemy_waypointlist1[0]);
            enemylist1.push_back(enemy);
        }
    }
    //一个循环结构的时间检定，30ms更新一次页面，
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
    // 设置300ms后游戏启动,singleShot是在该时间间隔之后运行一次,仅一次
    //QTimer::singleShot(300, this, SLOT(gameStart()));
}
void round1::paintEvent(QPaintEvent *)
{
    qDebug()<<"work here1"<<endl;
    if (round1_ifwin==false || round1_ended==true)
    {
        QString text = round1_ifwin ? "YOU WIN!!!" : "YOU LOST!!!";
        QPainter painter(this);
        painter.setPen(QPen(Qt::red));
        painter.drawText(rect(), Qt::AlignCenter, text);
        timer->stop();
        return;
    }
    else if(round1_ifwin==true && round1_ended==false)
    {
        qDebug()<<"work here2"<<endl;
        QPainter painter1(this);
        QPixmap pixmap(":/images/round2map.jpg");
        painter1.drawPixmap(0,0,
                          this->width(),
                          this->height(),pixmap);
        foreach (const Waypoint *wayPoint, enemy_waypointlist1)
        {
            wayPoint->draw(&painter1);
        }
        qDebug()<<"work here3"<<endl;
        for(int i=0;i<8;i++)//这里完成炮塔升级移除功能,并绘制炮塔
        {
            if(towerlist1[i]==nullptr)//不能使用empty()，这里列表中有元素，只不过是空指针
            {
                continue;
            }
            if(towerlist1[i]->tower_ifadvance==true && towerlist1[i])
            {
                if(towerlist1[i]->tower_type==1)
                {
                    Tower *towerdeldete=towerlist1[i];
                    Tower *towerreplace= new kingplus(towerlist1[i]->getPosition(),this,nullptr);
                    towerlist1.replace(i,towerreplace);
                    delete  towerdeldete;
                    QMediaPlayer *towersound = new QMediaPlayer(this);
                    towersound->setMedia(QUrl("qrc:/music/advance.mp3"));
                    towersound->setVolume(50);
                    towersound->play();
                    towersound=nullptr;
                }
                else if(towerlist1[i]->tower_type==2)
                {
                    Tower *towerdeldete=towerlist1[i];
                    Tower *towerreplace= new queenplus(towerlist1[i]->getPosition(),this,nullptr);
                    towerlist1.replace(i,towerreplace);
                    delete  towerdeldete;
                    QMediaPlayer *towersound = new QMediaPlayer(this);
                    towersound->setMedia(QUrl("qrc:/music/advance.mp3"));
                    towersound->setVolume(50);
                    towersound->play();
                    towersound=nullptr;
                }
            }
            if(towerlist1[i]->tower_ifremove==true && towerlist1[i])
            {
                Tower *towerdeldete=towerlist1[i];
                towerlist1.replace(i,nullptr);
                delete  towerdeldete;
                QMediaPlayer *towersound = new QMediaPlayer(this);
                towersound->setMedia(QUrl("qrc:/music/advance.mp3"));
                towersound->setVolume(50);
                towersound->play();
                towersound=nullptr;
            }
            if(towerlist1[i])
            {
                towerlist1[i]->draw(&painter1);
            }//在此处修改一个BUG，如果为空指针，则无法调用成员函数，会造成程序闪退
        }
        qDebug()<<"work here4"<<endl;
        //这里检查怪物血量以及移除，并画出怪物
        int ifwavefinished=0;
        for(int i=(round1_waves-1)*8;i<round1_waves*8;i++)
        {
            if(round1_waves>3)
            {
                round1_ended=true;
                break;
            }
            if(enemylist1[i]==nullptr)
            {
                ifwavefinished++;//这个BUG解决了很久，原来是空指针惹的事，所以加上这个
            }
            if(enemylist1[i] &&
                    enemylist1[i]->enemy_active==false)
            {
                int spawntime;
                spawntime=8000+2000*(i%8);//设置每波间隔时间8s
                QTimer::singleShot(spawntime,enemylist1[i],SLOT(doActivate()));
            }
            if(enemylist1[i] && enemylist1[i]->enemy_currentHp <=0)
            {
                //Enemy *enemydelete=enemylist1[i];//清除缓存
                enemylist1[i]=nullptr;
                //delete enemydelete;//清除缓存(必须注释掉，因为基类含有虚函数，如果直接delete，会报错
                ifwavefinished++;
                round1_playrGold+=50;//击败怪物奖励金币
                QMediaPlayer *towersound = new QMediaPlayer(this);
                towersound->setMedia(QUrl("qrc:/music/Enemydie.mp3"));
                towersound->setVolume(50);
                towersound->play();
                towersound=nullptr;
            }
            if(enemylist1[i] && enemylist1[i]->enemy_destinationWayPoint==nullptr)
            {
                //说明怪物已经到达基地
                round1_playerHp-=1;
                qDebug()<<"HP LOST"<<endl;
                QMediaPlayer *towersound = new QMediaPlayer(this);
                towersound->setMedia(QUrl("qrc:/music/life_lose.mp3"));
                towersound->setVolume(50);
                towersound->play();
                towersound=nullptr;
                if(round1_playerHp<=0)
                {
                    round1_ifwin=false;
                }
                Enemy *enemydelete=enemylist1[i];//清除缓存
                enemylist1[i]=nullptr;
                delete enemydelete;//清除缓存
                ifwavefinished++;
            }
            if(enemylist1[i])
            {
                enemylist1[i]->draw(&painter1);
            }
        }
        qDebug()<<"work here5"<<endl;
        if(ifwavefinished>=8 && round1_waves<=3)
        {
            round1_waves++;
        }
        drawWave(&painter1);
        drawHP(&painter1);
        drawPlayerGold(&painter1);
        qDebug()<<"work here6"<<endl;

    }
    qDebug()<<"painting done"<<endl;
}
void round1::setwaypoint()
{
    Waypoint *waypoint11 = new Waypoint(QPoint(50, 350));
    enemy_waypointlist1.push_back(waypoint11);

    Waypoint *waypoint12 = new Waypoint(QPoint(750, 350));
    enemy_waypointlist1.push_back(waypoint12);
    waypoint11->setNextWayPoint(waypoint12);
}
void round1::updateMap()
{
    qDebug()<<"work hereupdate"<<endl;

        foreach (Enemy *enemy,enemylist1)
        {
            if(enemy)
            {
                enemy->move();//空指针不能调用函数
            }
        }
        foreach (Tower *tower, towerlist1)
        {//空指针不能调用函数
            if(tower)
            {
                qDebug()<<"check1"<<endl;
                tower->checkEnemyInRange();
                qDebug()<<"check2"<<endl;
            }
        }
        update();
        qDebug()<<"work hereupdatedone"<<endl;

}
void round1::gamestart()
{
    update();
}
void round1::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(200, 50, 100, 100), QString("WAVE : %2").arg(round1_waves ));
}

void round1::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(50, 50, 100, 100), QString("HP : %2").arg(round1_playerHp));
}

void round1::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(400, 50, 100, 100), QString("GOLD : %2").arg(round1_playrGold));
}
