#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QObject>
#include <QPoint>
#include <QPainter>

class Waypoint
{
public:
    Waypoint(QPoint pos);
    void setNextWayPoint(Waypoint *nextPoint);
    Waypoint* nextWayPoint() const;
    const QPoint pos() const;
    void draw(QPainter *painter) const;
signals:
private:
    const QPoint enemy_pos;
    Waypoint * enemy_nextWaypoint;

};

#endif // WAYPOINT_H
