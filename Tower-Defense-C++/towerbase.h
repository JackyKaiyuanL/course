#ifndef TOWERBASE_H
#define TOWERBASE_H

#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QPushButton>
#include <QAction>

class towerbase : public QPushButton
{
    Q_OBJECT
public:
    towerbase();

signals:
    void setking();
    void setqueen();
    void advance();
    void remove();
};

#endif // TOWERBASE_H
