#include "eng_math.h"

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

bool IsInInterval(float value, float min, float max)
{
    return (value <= max && value >= min);
}