#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "fanwheel/fan_param.h"
#include "algorithm/include/usr_math.h"
float fan::get_predict_angle_by_predict(float shoot_delay,uint8_t fps)
{
    static uint8_t init_signal = 0;
    static float  start_time = 0.0;
    uint start_num = get_start_section_num(10);
    if((init_signal == 0) && (start_num != 0))
    {
       start_time = (start_num + 0.5f) * 3.33f/16.0f;
       cout<<"start_num"<<start_num<<endl;
       init_signal = 1;
    }
    if(init_signal)
    {
        static float time_gap = 1/static_cast<float>(fps);
        static float time_increase = 0.0f;
        time_increase += time_gap;
        float t0 = start_time + time_increase;
        float t1 = t0 + shoot_delay ;//the period is 3.33s
        float f0 = -0.785f/1.884f*cosf(1.884f*t0) + 1.305f*t0;
        float f1 = -0.785f/1.884f*cosf(1.884f*t1) + 1.305f*t1;
        return (f1-f0)*RAD_TO_ANGLE;
    }
    else
        return 0.0f;
}
