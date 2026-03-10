#include "waypoint.h"
#include <QColor>

Waypoint::Waypoint(QPoint pos)
    : enemy_pos(pos)
    , enemy_nextWaypoint(NULL)
{
}

void Waypoint::setNextWayPoint(Waypoint *nextPoint)
{
    enemy_nextWaypoint = nextPoint;
}

Waypoint* Waypoint::nextWayPoint() const
{
    return enemy_nextWaypoint;
}

const QPoint Waypoint::pos() const
{
    return enemy_pos;
}

void Waypoint::draw(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::red);
    painter->drawEllipse(enemy_pos, 5, 5);
    painter->drawEllipse(enemy_pos, 1, 1);

    if (enemy_nextWaypoint)
        painter->drawLine(enemy_pos, enemy_nextWaypoint->enemy_pos);
    painter->restore();
}

