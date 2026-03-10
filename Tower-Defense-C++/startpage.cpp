#include "startpage.h"
#include "ui_startpage.h"
#include <QDebug>

Startpage::Startpage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Startpage),r1(nullptr),r2(nullptr)
{
    this->setFixedSize(800,600);
    ui->setupUi(this);
    start.setParent(this);
    start.setStyleSheet("background-color:lightgreen;");
    start.setText("Start");
    start.move(400,400);
    start.resize(80,60);
    connect(&start,&QPushButton::clicked,[=](){
       this->hide();
        choice1.show();
    });//完成首页到选择页1的切换
    connect(&choice1,&choicepage1::goforward,[=](){
        choice1.hide();
        choice2.show();
    });//完成选择页1到选择页2的切换
    connect(&choice1,&choicepage1::game1start,[=](){
        if(r1)
        {
            delete r1;
            r1=nullptr;
        }
        r1= new round1;
        choice1.hide();
        r1->show();
        connect(r1,&round1::gochoice1,[=]()
        {
            choice1.show();
            delete r1;
            r1=nullptr;
        });//完成游戏页1到选择页1的切换
    });//完成选择页1到游戏页1的切换
    connect(&choice2,&choicepage2::goback,[=](){
        choice2.hide();
        choice1.show();
    });//完成选择页2到选择页1的切换
    connect(&choice2,&choicepage2::game2start,[=](){
        if(r2)
        {
            delete r2;
            r2=nullptr;
        }//一定需要删除数据，要不然程序一运行，各个游戏页面就开始游戏
        r2= new round2;
        choice2.hide();
        r2->show();
        connect(r2,&round2::gochoice2,[=]()
        {
            choice2.show();
            delete r2;
            r2=nullptr;
        });//经过反复尝试，必须将round2的信号连接在choice2内
    });//完成选择页2到游戏页2的切换
}

Startpage::~Startpage()
{
    delete ui;
}
void Startpage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap(":/images/startmap.jpg");
    painter.drawPixmap(0,0,
                          this->width(),
                          this->height(),pixmap);
}
