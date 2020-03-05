#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "fanwheel/fan_param.h"
#include "algorithm/include/usr_math.h"
float fan::get_sec_by_predict(float *last_sec)
{ 
    *last_sec = get_sec_by_time(*last_sec,m_fps);
    return *last_sec;
}
float fan::get_predict_angle(float sec,float shoot_delay)
{
    if(sec > 0.0f)
    {
        float t0 = (sec + fan_param.PRE_SECTION_OFFSET/100.0f) * 3.33f/16.0f;
        float t1 = t0 + shoot_delay ;//the period is 3.33s
        float f0 = -0.785f/1.884f*cosf(1.884f*t0) + 1.305f*t0;
        float f1 = -0.785f/1.884f*cosf(1.884f*t1) + 1.305f*t1;
        float angle = (f1-f0)*RAD_TO_ANGLE;
        float a1 = kalman1_filter(&m_ang_kal2,angle);
        return a1;
    }
    else
        return 0.0f;
}
