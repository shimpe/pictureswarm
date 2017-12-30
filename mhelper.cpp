#include "mhelper.h"
#include <cmath>

mhelper::mhelper() {}

float mhelper::map(float value, float infrom, float into, float outfrom, float outto)
{
    if (into == infrom)
        return INFINITY;

    float output = ((outfrom + outto) + (outto - outfrom) * (
               (2 * value - (infrom + into)) / float(into - infrom))) / 2.0;
    return output;
}

float mhelper::linexp(float value, float infrom, float into, float outfrom, float outto)
{
    if (outfrom == 0)
        return INFINITY;
    if (into == infrom)
        return INFINITY;

    float output = pow(outto / outfrom, (value - infrom) / (into - infrom)) * outfrom;
    return output;
}

float mhelper::explin(float value, float infrom, float into, float outfrom, float outto)
{
    if (value == 0)
        return INFINITY;
    if (into == 0)
        return INFINITY;
    if (infrom == 0)
        return INFINITY;
    if ((value < 0 && infrom > 0) || (value > 0 && infrom < 0))
        return INFINITY;
    if ((into < 0 && infrom > 0) || (into > 0 && infrom < 0))
        return INFINITY;

    float output = log(value / infrom) / log(into / infrom) * (outto - outfrom) + outfrom;
    return output;
}

float mhelper::expexp(float value, float infrom, float into, float outfrom, float outto)
{
    if (value == 0)
        return INFINITY;
    if (into == 0)
        return INFINITY;
    if (outfrom == 0)
        return INFINITY;
    if (infrom == 0)
        return INFINITY;
    if ((value < 0 && infrom > 0) || (value > 0 && infrom < 0))
        return INFINITY;
    if ((into < 0 && infrom > 0) || (into > 0 && infrom < 0))
        return INFINITY;
    float output = pow(outto / outfrom, log(value / infrom) / log(into / infrom)) * outto;
    return output;
}
