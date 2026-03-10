#ifndef UTILITY_H
#define UTILITY_H

#include <QtMath>
#include <QPoint>

inline bool collisionWithCircle(QPoint point1, int r1, QPoint point2, int r2)
{
    const int x1 = point1.x() - point2.x();
    const int y1 = point1.y() - point2.y();
    const int distance = sqrt(x1 * x1 + y1 * y1);

    if (distance <= r1 + r2)
        return true;

    return false;
}

#endif // UTILITY_H
