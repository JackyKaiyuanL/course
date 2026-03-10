#include "choicepage2.h"

choicepage2::choicepage2(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(800,600);
    back.setParent(this);
    back.setText("backward");
    back.resize(80,60);
    back.move(700,500);
    back.setStyleSheet("background-color:green;");
    game2.setParent(this);
    game2.setText("2");
    game2.resize(80,60);
    game2.setStyleSheet("background-color:green;");
    game2.move(0,500);
    connect(&back,&QPushButton::clicked,[=](){
        this->hide();
        emit goback();
    });
    connect(&game2,&QPushButton::clicked,[=](){
        this->hide();
        emit game2start();
    });
}
void choicepage2::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap(":/images/choose2.jpg.png");
    painter.drawPixmap(0,0,
                          this->width(),
                          this->height(),pixmap);
}
