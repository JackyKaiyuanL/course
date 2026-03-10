#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QObject>
#include <QMainWindow>
#include <QPainter>
#include <QPushButton>
#include <QPaintEvent>
#include "choicepage1.h"
#include "choicepage2.h"
#include "round1.h"
#include "round2.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Startpage; }
QT_END_NAMESPACE

class Startpage : public QMainWindow
{
    Q_OBJECT

public:
    Startpage(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    ~Startpage();

private:
    Ui::Startpage *ui;
    QPushButton start;
    choicepage1 choice1;
    choicepage2 choice2;
    round1 *r1;
    round2 *r2;
};
#endif // STARTPAGE_H
