#include "vhelper.h"

vhelper::vhelper()
{
}

QVector2D vhelper::setMag(const QVector2D &v, float magnitude)
{
    QVector2D result = v.normalized();
    return result*magnitude;
}

QVector2D vhelper::limitmin(const QVector2D &v, float magnitude)
{
    float vmag = v.lengthSquared();
    if (vmag < magnitude*magnitude)
    {
        return vhelper::setMag(v, magnitude);
    }
    return v;
}

QVector2D vhelper::limitmax(const QVector2D &v, float magnitude)
{
    float vmag = v.lengthSquared();
    if (vmag > magnitude*magnitude)
    {
        return vhelper::setMag(v, magnitude);
    }
    return v;
}

QVector2D vhelper::limit(const QVector2D &v, float xmin, float xmax, float ymin, float ymax)
{
    QVector2D u(v);
    if (v.x() < xmin)
        u.setX(xmin);
    if (v.x() > xmax)
        u.setX(xmax);
    if (v.y() < ymin)
        u.setY(ymin);
    if (v.y() > ymax)
        u.setY(ymax);
    return u;
}
