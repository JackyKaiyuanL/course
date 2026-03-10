#ifndef CHOICEPAGE2_H
#define CHOICEPAGE2_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QPaintEvent>
#include <QPixmap>

class choicepage2 : public QWidget
{
    Q_OBJECT
public:
    explicit choicepage2(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

signals:
    void goback();
    void game2start();
private:
    QPushButton back;
    QPushButton game2;
};

#endif // CHOICEPAGE2_H
