#ifndef CHOICEPAGE1_H
#define CHOICEPAGE1_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QPaintEvent>
#include <QPixmap>

class choicepage1 : public QWidget
{
    Q_OBJECT
public:
    explicit choicepage1(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

signals:
    void goforward();
    void game1start();
private:
    QPushButton forward;
    QPushButton game1;
};

#endif // CHOICEPAGE1_H
