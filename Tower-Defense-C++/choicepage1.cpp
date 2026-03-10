#include "choicepage1.h"

choicepage1::choicepage1(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(800,600);
    forward.setParent(this);
    forward.setText("forward");
    forward.resize(80,60);
    forward.move(700,500);
    forward.setStyleSheet("background-color:green;");
    game1.setParent(this);
    game1.setText("1");
    game1.resize(80,60);
    game1.setStyleSheet("background-color:green;");
    game1.move(0,500);
    connect(&forward,&QPushButton::clicked,[=](){
        this->hide();
        emit goforward();
    });
    connect(&game1,&QPushButton::clicked,[=](){
        this->hide();
        emit game1start();
    });
}
void choicepage1::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap(":/images/choose1.jpg.png");
    painter.drawPixmap(0,0,
                          this->width(),
                          this->height(),pixmap);
}
