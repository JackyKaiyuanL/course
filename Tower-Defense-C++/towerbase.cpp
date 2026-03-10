#include "towerbase.h"
#include <QSize>
//通过该塔防基座右击可以实现建造升级移除防御塔
towerbase::towerbase():
    QPushButton(0)
{
    this->setFixedSize(100,100);
    setStyleSheet("QPushButton{border-image: "
                  "url(:/images/towerbase.png)}");
    //给基座按钮设置菜单项
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction *action1 =new QAction(this);
    action1->setText("setKing");
    QAction *action2 =new QAction(this);
    action2->setText("setQueen");
    QAction *action3 =new QAction(this);
    action3->setText("Advance");
    QAction *action4 =new QAction(this);
    action4->setText("removeTower");
    this->addAction(action1);
    this->addAction(action2);
    this->addAction(action3);
    this->addAction(action4);
    connect(action1,&QAction::triggered,this,[=](){
        emit setking();
    });
    connect(action2,&QAction::triggered,this,[=](){
        emit setqueen();
    });
    connect(action3,&QAction::triggered,this,[=](){
        emit advance();
    });
    connect(action4,&QAction::triggered,this,[=](){
        emit remove();
    });
}
