#include "algorithm/include/usr_math.h"

/****** this function is for contrain a value to a interval ***************/
float loop_float_constrain(float input,float min_value,float max_value)
{
    if(max_value < min_value)
    {
        return input;
    }
    if(input > max_value)
    {
        float len = max_value - min_value;
        while(input > max_value)
        {
            input -= len;
        }
    }
    else if(input < min_value)
    {
        float len = max_value - min_value;
        while(input < min_value)
        {
            input += len;
        }
    }
    return input;
}
