#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "fanwheel/fan_param.h"
#include "algorithm/include/usr_math.h"
float fan::get_predict_angle_by_test(float shoot_delay)
{
    static float sec = 0.0f;
    static float last_sec = 0.0f;
    float k1 = get_sec_by_kal3(10);
    k1 = continue_sec_1(k1);
    float k2 = get_sec_by_slope(10) - 3.0f;
    if(k2 <= 0)
        k2 += 16.0f;
    //k2 = continue_sec_2(k2);
    float k3 = get_sec_by_time(last_sec,50);
    plot_y1 = static_cast<double>(k1);
    plot_y2 = static_cast<double>(k2);
//    plot_y3 = static_cast<double>((k1+k2)/2);
//    plot_y4 = static_cast<double>(last_sec);
    //sec = (k1+k2)/2.0f;
    sec = k1;
    last_sec = sec;
    if(sec > 0.0f)
    {
        float t0 = (sec + 0.5f) * 3.33f/16.0f;
        float t1 = t0 + shoot_delay ;//the period is 3.33s
        float f0 = -0.785f/1.884f*cosf(1.884f*t0) + 1.305f*t0;
        float f1 = -0.785f/1.884f*cosf(1.884f*t1) + 1.305f*t1;
        float angle = (f1-f0)*RAD_TO_ANGLE;
        float a1 = kalman1_filter(&m_ang_kal1,angle);
        return a1;
    }
    else
        return 0.0f;
}
float fan::get_sec_by_time(float sec_last,uint8_t fps)
{
    static float time_gap = 1/static_cast<float>(fps)*16/3.33f;
    float sec = sec_last + time_gap;
    while (sec > 16.0f)
    {
        sec -= 16.0f;
    }
    //sec = loop_float_constrain(sec,1.0f,16.0f);
    return sec;
}
float fan::get_sec_by_kal3(uint8_t max_error)
{
    static float spd_set = 75;//75
    if(v_spd_kal3.size() == 5)
    {
        float mid_value = (v_spd_kal3[4]+v_spd_kal3[3])/2.0f;
        float slope = fabs(v_spd_kal3[4]) - fabs(v_spd_kal3[3]);
        for(size_t i=0;i<5;i++)
        {
            if(v_spd_kal3[i] == 0.0f)
                return 0;
            float delta = fabs(v_spd_kal3[i] - mid_value);
            if(delta > max_error)
                return 0;
        }
        float spd_compare = (fabs(mid_value) - spd_set)/fabs(45.0f)*90/60.f;//set the model is 1
        return get_the_seclector_by_test(spd_compare,slope);
    }
    return 0;
}
float fan::get_sec_by_slope(uint8_t max_error)
{
    static float slope_3 =0.0f;
    static float last_slope_3 =0.0f;
    if(v_spd_kal3.size() == 5)
    {
        float mid_value = (v_spd_kal3[4]+v_spd_kal3[3])/2.0f;
        float slope = fabs(v_spd_kal3[4]) - fabs(v_spd_kal3[3]);
        float slope_2 = kalman1_filter(&m_slope_kal2,slope);
        last_slope_3 = slope_3;
        slope_3 = kalman1_filter(&m_slope_kal3,slope_2);
//        plot_y1 = static_cast<double>(slope);
//        plot_y2 = static_cast<double>(slope_2);
//        plot_y3 = static_cast<double>(slope_3);
        for(size_t i=0;i<5;i++)
        {
            if(v_spd_kal3[i] == 0.0f)
                return 0;
            float delta = fabs(v_spd_kal3[i] - mid_value);
            if(delta > max_error)
                return 0;
        }
        float spd_compare = slope_3/2.4f;
        return get_the_seclector_by_test(spd_compare,slope_3-last_slope_3);
    }
    return 0;
}
float fan::continue_sec_1(float sec)
{
    static bool init_mark = 0;
    static float last_sec = 0;
    static float return_val;
    return_val = sec;
    float diff = sec - last_sec;
    if(init_mark)
    {
        if(last_sec < 16.0f)
        {
           if(diff < 0)
           {
                return_val =  last_sec;
           }
           else if(diff > 1.0f)
                return_val =  last_sec + 1.0f;
        }
        else
        {
            if(diff < 0)
            {
                if(diff > -15.0f)
                    return_val =  last_sec;
                else
                    return_val = 1.0f;
            }
        }
    }
    else
        init_mark = true;
    last_sec = return_val;
    return return_val;
}
float fan::continue_sec_2(float sec)
{
    static bool init_mark = 0;
    static float last_sec = 0;
    static float return_val;
    return_val = sec;
    float diff = sec - last_sec;
    if(init_mark)
    {
        if(last_sec < 16.0f)
        {
           if(diff < 0)
           {
                return_val =  last_sec;
           }
           else if(diff > 1.0f)
                return_val =  last_sec + 1.0f;
        }
        else
        {
            if(diff < 0)
            {
                if(diff > -15.0f)
                    return_val =  last_sec;
                else
                    return_val = 1.0f;
            }
        }
    }
    else
        init_mark = true;
    last_sec = return_val;
    return return_val;
}
