#ifndef VHELPER_H
#define VHELPER_H

#include <QVector2D>

class vhelper
{
public:
    vhelper();
    static QVector2D setMag(const QVector2D &v, float magnitude);
    static QVector2D limit(const QVector2D &v, float magnitude);
    static QVector2D limit(const QVector2D &v, float xmin, float xmax, float ymin, float ymax);

};

#endif // VHELPER_H
