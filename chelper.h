#ifndef CHELPER_H
#define CHELPER_H

#include <QVector>
#include <QColor>

class chelper
{
public:
    chelper();

    static QColor avg(const QVector<QColor> &colors);
};

#endif // CHELPER_H
