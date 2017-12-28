#include "chelper.h"

chelper::chelper() { }

QColor chelper::avg(const QVector<QColor> &colors)
{
    int total_r = 0;
    int total_g = 0;
    int total_b = 0;
    int total_a = 0;
    for (const auto &c : colors)
    {
        int r = 0;
        int g = 0;
        int b = 0;
        int a = 0;
        c.getRgb(&r, &g, &b, &a);
        total_r += r;
        total_g += g;
        total_b += b;
        total_a += a;
    }
    int size = colors.size();
    return QColor(total_r/size, total_g/size, total_b/size, total_a/size);
}
