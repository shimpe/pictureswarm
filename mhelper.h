#ifndef MHELPER_H
#define MHELPER_H

class mhelper
{
public:
    mhelper();
    static float map(float value, float infrom, float into, float outfrom, float outto);
    static float linexp(float value, float infrom, float into, float outfrom, float outto);
    static float explin(float value, float infrom, float into, float outfrom, float outto);
    static float expexp(float value, float infrom, float into, float outfrom, float outto);
};

#endif // MHELPER_H
