#include "mhelper.h"

mhelper::mhelper()
{

}

float mhelper::map(float value, float infrom, float into, float outfrom, float outto)
{
    float output = ((outfrom + outto) + (outto - outfrom) * (
               (2 * value - (infrom + into)) / float(into - infrom))) / 2.0;
    return output;
}
