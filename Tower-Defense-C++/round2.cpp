#include "round2.h"
#include <QDebug>
static const int kingCost = 100;//设定每安置一个国王炮塔花费100金币
static const int queenCost =150;//设定每安置一个王后炮塔花费100金币
static const int UpdateTowerCost = 200;//设定升级炮塔花费200金币
static const int removevalue=50; //炮塔回收增加50金币

round2::round2(QWidget *parent) : QWidget(parent)
  ,round2_waves(1)
  ,round2_playerHp(2)//第二关减少血量和金币数量，但还是处于开挂状态^_^
  ,round2_playrGold(5000)
  ,round2_ended(false)
  ,round2_ifwin(true)
{
    this->setFixedSize(800,800);
    //设置背景音乐
    groundmusic = new QMediaPlayer(this);
    groundmusic->setMedia(QUrl("qrc:/music/backgroundsound.mp3"));
    groundmusic->setVolume(10);
    groundmusic->play();

    quit2.setParent(this);
    quit2.setStyleSheet("background-color:lightgreen;");
    quit2.setText("Quit");
    quit2.move(720,740);
    quit2.resize(80,60);
    connect(&quit2,&QPushButton::clicked,[=](){
       this->hide();
       emit gochoice2();
    });
    general2.setParent(this);
    general2.move(700,500);
    general2.setFixedSize(100,100);
    general2.setStyleSheet("QPushButton{border-image: "
                  "url(:/images/general.png)}");
    base1 = new towerbase;
    base1->setParent(this);
    base1->move(100,100);
    base2 = new towerbase;
    base2->setParent(this);
    base2->move(300,100);
    base3 = new towerbase;
    base3->setParent(this);
    base3->move(500,300);
     base4 = new towerbase;
    base4->setParent(this);
    base4->move(600,300);
    base5 = new towerbase;
    base5->setParent(this);
    base5->move(100,500);
    base6 = new towerbase;
    base6->setParent(this);
    base6->move(300,500);
    base7 = new towerbase;
    base7->setParent(this);
    base7->move(500,700);
    base8 = new towerbase;
    base8->setParent(this);
    base8->move(600,700);
    //炮塔列表设置空指针数量为8
    for(int i=0;i<8;i++)
    {
        //towerlist1[i]=nullptr;造成数组越界，错误,[]索引一定要在列表有元素后才可进行
        towerlist2.push_back(nullptr);
    }
    //设置基座信号接收机制
    //设置基座信号接收机制
    connect(base1,&towerbase::setking,this,[=](){
        if(round2_playrGold>=kingCost && towerlist2[0]==nullptr)
        {
            Tower *tower= new king(QPoint(150,250), nullptr,this);
            towerlist2[0]=tower;
            round2_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base1,&towerbase::setqueen,this,[=](){
        if(round2_playrGold>=queenCost && towerlist2[0]==nullptr)
        {
            Tower *tower= new queen(QPoint(150,250), nullptr,this);
            towerlist2[0]=tower;
            round2_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base1,&towerbase::advance,this,[=](){
             if(round2_playrGold>=200 && towerlist2[0])
             {
                 towerlist2[0]->tower_ifadvance=true;
                 round2_playrGold-=200;
             }
            });
    connect(base1,&towerbase::remove,this,[=](){
        if(towerlist2[0])
             {
                towerlist2[0]->tower_ifremove=true;
                round2_playrGold+=50;
            }
            });
    connect(base2,&towerbase::setking,this,[=](){
        if(round2_playrGold>=kingCost && towerlist2[1]==nullptr)
        {
            Tower *tower= new king(QPoint(350,250), nullptr,this);
            towerlist2[1]=tower;
            round2_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base2,&towerbase::setqueen,this,[=](){
        if(round2_playrGold>=queenCost && towerlist2[1]==nullptr)
        {
            Tower *tower= new queen(QPoint(350,250), nullptr,this);
            towerlist2[1]=tower;
            round2_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base2,&towerbase::advance,this,[=](){
        if(round2_playrGold>=200 && towerlist2[1])
        {
            towerlist2[1]->tower_ifadvance=true;
            round2_playrGold-=200;
        }
            });
    connect(base2,&towerbase::remove,this,[=](){
        if(towerlist2[1])
             {
                towerlist2[1]->tower_ifremove=true;
                round2_playrGold+=50;
            }
            });
    connect(base3,&towerbase::setking,this,[=](){
        if(round2_playrGold>=kingCost && towerlist2[2]==nullptr)
        {
            Tower *tower= new king(QPoint(550,450), nullptr,this);
            towerlist2[2]=tower;
            round2_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base3,&towerbase::setqueen,this,[=](){
        if(round2_playrGold>=queenCost && towerlist2[2]==nullptr)
        {
            Tower *tower= new queen(QPoint(550,450), nullptr,this);
            towerlist2[2]=tower;
            round2_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base3,&towerbase::advance,this,[=](){
        if(round2_playrGold>=200 && towerlist2[2])
        {
            towerlist2[2]->tower_ifadvance=true;
            round2_playrGold-=200;
        }
            });
    connect(base3,&towerbase::remove,this,[=](){
        if(towerlist2[2])
             {
            towerlist2[2]->tower_ifremove=true;
             round2_playrGold+=50;
        }
            });
    connect(base4,&towerbase::setking,this,[=](){
        if(round2_playrGold>=kingCost && towerlist2[3]==nullptr)
        {
            Tower *tower= new king(QPoint(650,450), nullptr,this);
            towerlist2[3]=tower;
            round2_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base4,&towerbase::setqueen,this,[=](){
        if(round2_playrGold>=queenCost && towerlist2[3]==nullptr)
        {
            Tower *tower= new queen(QPoint(650,450), nullptr,this);
            towerlist2[3]=tower;
            round2_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base4,&towerbase::advance,this,[=](){
        if(round2_playrGold>=200 && towerlist2[3])
        {
            towerlist2[3]->tower_ifadvance=true;
            round2_playrGold-=200;
        }
            });
    connect(base4,&towerbase::remove,this,[=](){
        if(towerlist2[3])
        {
            towerlist2[3]->tower_ifremove=true;
             round2_playrGold+=50;
        }
            });
    connect(base5,&towerbase::setking,this,[=](){
        if(round2_playrGold>=kingCost && towerlist2[4]==nullptr)
        {
            Tower *tower= new king(QPoint(150,450), nullptr,this);
            towerlist2[4]=tower;
            round2_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base5,&towerbase::setqueen,this,[=](){
        if(round2_playrGold>=queenCost && towerlist2[4]==nullptr)
        {
            Tower *tower= new queen(QPoint(150,450), nullptr,this);
            towerlist2[4]=tower;
            round2_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base5,&towerbase::advance,this,[=](){
        if(round2_playrGold>=200 && towerlist2[4])
        {
            towerlist2[4]->tower_ifadvance=true;
            round2_playrGold-=200;
        }
            });
    connect(base5,&towerbase::remove,this,[=](){
        if(towerlist2[4])
             {
               towerlist2[4]->tower_ifremove=true;
               round2_playrGold+=50;
        }
            });
    connect(base6,&towerbase::setking,this,[=](){
        if(round2_playrGold>=kingCost && towerlist2[5]==nullptr)
        {
            Tower *tower= new king(QPoint(350,450),nullptr,this);
            towerlist2[5]=tower;
            round2_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base6,&towerbase::setqueen,this,[=](){
        if(round2_playrGold>=queenCost && towerlist2[5]==nullptr)
        {
            Tower *tower= new queen(QPoint(350,450), nullptr,this);
            towerlist2[5]=tower;
            round2_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base6,&towerbase::advance,this,[=](){
        if(round2_playrGold>=200 && towerlist2[5])
        {
            towerlist2[5]->tower_ifadvance=true;
            round2_playrGold-=200;
        }
            });
    connect(base6,&towerbase::remove,this,[=](){
        if(towerlist2[5])
             {towerlist2[5]->tower_ifremove=true;
             round2_playrGold+=50;}
            });
    connect(base7,&towerbase::setking,this,[=](){
        if(round2_playrGold>=kingCost && towerlist2[6]==nullptr)
        {
            Tower *tower= new king(QPoint(550,650), nullptr,this);
            towerlist2[6]=tower;
            round2_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base7,&towerbase::setqueen,this,[=](){
        if(round2_playrGold>=queenCost && towerlist2[6]==nullptr)
        {
            Tower *tower= new queen(QPoint(550,650), nullptr,this);
            towerlist2[6]=tower;
            round2_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base7,&towerbase::advance,this,[=](){
        if(round2_playrGold>=200 && towerlist2[6])
        {
            towerlist2[6]->tower_ifadvance=true;
            round2_playrGold-=200;
        }
            });
    connect(base7,&towerbase::remove,this,[=](){
        if(towerlist2[6])
             {towerlist2[6]->tower_ifremove=true;
             round2_playrGold+=50;}
            });
    connect(base8,&towerbase::setking,this,[=](){
        if(round2_playrGold>=kingCost && towerlist2[7]==nullptr)
        {
            Tower *tower= new king(QPoint(650,650), nullptr,this);
            towerlist2[7]=tower;
            round2_playrGold-=100;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }

            });
    connect(base8,&towerbase::setqueen,this,[=](){
        if(round2_playrGold>=queenCost && towerlist2[7]==nullptr)
        {
            Tower *tower= new queen(QPoint(650,650), nullptr,this);
            towerlist2[7]=tower;
            round2_playrGold-=150;
            QMediaPlayer *towersound = new QMediaPlayer(this);
            towersound->setMedia(QUrl("qrc:/music/tower_place.mp3"));
            towersound->setVolume(50);
            towersound->play();
            towersound=nullptr;
        }
            });
    connect(base8,&towerbase::advance,this,[=](){
        if(round2_playrGold>=200 && towerlist2[7])
        {
            towerlist2[7]->tower_ifadvance=true;
            round2_playrGold-=200;
        }
            });
    connect(base8,&towerbase::remove,this,[=](){
        if(towerlist2[7])
             {towerlist2[7]->tower_ifremove=true;
             round2_playrGold+=50;}
            });
    //设置怪物航点
    setwaypoint();
    //这里对怪物列表进行填充
    for(int i=0;i<8;i++)
    {
        if(i<6)
        {
            Enemy *enemy=new Enemy(this->enemy_waypointlist2[0]);
            enemylist2.push_back(enemy);
        }
        else
        {
            Enemy *enemy=new Enemyplus(this->enemy_waypointlist2[0]);
            enemylist2.push_back(enemy);
        }
    }
    for(int i=8;i<16;i++)
    {
        if(i<12)
        {
            Enemy *enemy=new Enemy(this->enemy_waypointlist2[0]);
            enemylist2.push_back(enemy);
        }
        else
        {
            Enemy *enemy=new Enemy2(this->enemy_waypointlist2[0]);
            enemylist2.push_back(enemy);
        }
    }
    for(int i=16;i<24;i++)
    {
        if(i<20)
        {
            Enemy *enemy=new Enemyplus(this->enemy_waypointlist2[0]);
            enemylist2.push_back(enemy);
        }
        else
        {
            Enemy *enemy=new Enemy2(this->enemy_waypointlist2[0]);
            enemylist2.push_back(enemy);
        }
    }
    //一个循环结构的时间检定，30ms更新一次页面，
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);

    // 设置300ms后游戏启动,singleShot是在该时间间隔之后运行一次,仅一次
   // QTimer::singleShot(300, this, SLOT(gameStart()));
}
void round2::paintEvent(QPaintEvent *)
{

    if (round2_ifwin==false || round2_ended==true)
    {
        QString text = round2_ifwin ? "YOU WIN!!!" : "YOU LOST!!!";
        QPainter painter(this);
        painter.setPen(QPen(Qt::red));
        painter.drawText(rect(), Qt::AlignCenter, text);
        timer->stop();//新发现的BUG，及时修改
        return;
    }
    else if(round2_ifwin==true && round2_ended==false)
    {

        QPainter painter1(this);
        QPixmap pixmap(":/images/round2map.jpg");
        painter1.drawPixmap(0,0,
                          this->width(),
                          this->height(),pixmap);
        foreach (const Waypoint *wayPoint, enemy_waypointlist2)
        {
            wayPoint->draw(&painter1);
        }

        for(int i=0;i<8;i++)//这里完成炮塔升级移除功能,并绘制炮塔
        {
            if(towerlist2[i]==nullptr)//不能使用empty()，这里列表中有元素，只不过是空指针
            {
                continue;
            }
            if(towerlist2[i]->tower_ifadvance==true && towerlist2[i])
            {
                if(towerlist2[i]->tower_type==1)
                {
                    Tower *towerdeldete=towerlist2[i];
                    Tower *towerreplace= new kingplus(towerlist2[i]->getPosition(),nullptr,this);
                    towerlist2.replace(i,towerreplace);
                    delete  towerdeldete;
                    QMediaPlayer *towersound = new QMediaPlayer(this);
                    towersound->setMedia(QUrl("qrc:/music/advance.mp3"));
                    towersound->setVolume(50);
                    towersound->play();
                    towersound=nullptr;
                }
                else if(towerlist2[i]->tower_type==2)
                {
                    Tower *towerdeldete=towerlist2[i];
                    Tower *towerreplace= new queenplus(towerlist2[i]->getPosition(),nullptr,this);
                    towerlist2.replace(i,towerreplace);
                    delete  towerdeldete;
                    QMediaPlayer *towersound = new QMediaPlayer(this);
                    towersound->setMedia(QUrl("qrc:/music/advance.mp3"));
                    towersound->setVolume(50);
                    towersound->play();
                    towersound=nullptr;
                }
            }
            if(towerlist2[i]->tower_ifremove==true && towerlist2[i])
            {
                Tower *towerdeldete=towerlist2[i];
                towerlist2.replace(i,nullptr);
                delete  towerdeldete;
                QMediaPlayer *towersound = new QMediaPlayer(this);
                towersound->setMedia(QUrl("qrc:/music/advance.mp3"));
                towersound->setVolume(50);
                towersound->play();
                towersound=nullptr;
            }
            if(towerlist2[i])
            {
                towerlist2[i]->draw(&painter1);
            }//在此处修改一个BUG，如果为空指针，则无法调用成员函数，会造成程序闪退
        }

        //这里检查怪物血量以及移除，并画出怪物
        int ifwavefinished=0;
        for(int i=(round2_waves-1)*8;i<round2_waves*8;i++)
        {
            if(round2_waves>3)
            {
                round2_ended=true;
                break;
            }
            if(enemylist2[i]==nullptr)
            {
                ifwavefinished++;//这个BUG解决了很久，原来是空指针惹的事，所以加上这个
            }
            if(enemylist2[i] &&
                    enemylist2[i]->enemy_active==false)
            {
                int spawntime;
                spawntime=8000+2000*(i%8);//设置每波间隔时间8s
                QTimer::singleShot(spawntime,enemylist2[i],SLOT(doActivate()));
            }
            if(enemylist2[i] && enemylist2[i]->enemy_currentHp <=0)
            {
                //Enemy *enemydelete=enemylist1[i];//清除缓存
                enemylist2[i]=nullptr;
                //delete enemydelete;//清除缓存(必须注释掉，因为基类含有虚函数，如果直接delete，会报错
                ifwavefinished++;
                round2_playrGold+=50;//击败怪物奖励金币
                QMediaPlayer *towersound = new QMediaPlayer(this);
                towersound->setMedia(QUrl("qrc:/music/Enemydie.mp3"));
                towersound->setVolume(50);
                towersound->play();
                towersound=nullptr;
            }
            if(enemylist2[i] && enemylist2[i]->enemy_destinationWayPoint==nullptr)
            {
                //说明怪物已经到达基地
                round2_playerHp-=1;
                QMediaPlayer *towersound = new QMediaPlayer(this);
                towersound->setMedia(QUrl("qrc:/music/life_lose.mp3"));
                towersound->setVolume(50);
                towersound->play();
                towersound=nullptr;
                if(round2_playerHp<=0)
                {
                    round2_ifwin=false;
                }
                //Enemy *enemydelete=enemylist2[i];//清除缓存
                enemylist2[i]=nullptr;
                //delete enemydelete;//清除缓存
                ifwavefinished++;
            }
            if(enemylist2[i])
            {
                enemylist2[i]->draw(&painter1);
            }
        }

        if(ifwavefinished>=8 && round2_waves<=3)
        {
            round2_waves++;
        }
        qDebug()<<"workround2-1"<<endl;
        drawWave(&painter1);
        drawHP(&painter1);
        drawPlayerGold(&painter1);
        qDebug()<<"workround2-2"<<endl;

    }

}
void round2::setwaypoint()//设置航线
{
    Waypoint *waypoint21 = new Waypoint(QPoint(50, 350));
    enemy_waypointlist2.push_back(waypoint21);

    Waypoint *waypoint22 = new Waypoint(QPoint(450, 350));
    enemy_waypointlist2.push_back(waypoint22);
    waypoint21->setNextWayPoint(waypoint22);

    Waypoint *waypoint23 = new Waypoint(QPoint(450, 550));
    enemy_waypointlist2.push_back(waypoint23);
    waypoint22->setNextWayPoint(waypoint23);

    Waypoint *waypoint24 = new Waypoint(QPoint(750, 550));
    enemy_waypointlist2.push_back(waypoint24);
    waypoint23->setNextWayPoint(waypoint24);
}
void round2::updateMap()
{

        foreach (Enemy *enemy,enemylist2)
        {
            if(enemy)
            {
                enemy->move();//空指针不能调用函数
            }
        }
        foreach (Tower *tower, towerlist2)
        {//空指针不能调用函数
            if(tower)
            {

                tower->checkEnemyInRange();

            }
        }
        update();

}
void round2::gamestart()
{
    update();
}
void round2::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(200, 50, 100, 100), QString("WAVE : %2").arg(round2_waves ));
}

void round2::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(50, 50, 100, 100), QString("HP : %2").arg(round2_playerHp));
}

void round2::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(400, 50, 100, 100), QString("GOLD : %2").arg(round2_playrGold));
}
