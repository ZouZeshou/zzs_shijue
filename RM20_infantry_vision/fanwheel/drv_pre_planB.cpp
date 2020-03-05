#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "fanwheel/fan_param.h"
#include "algorithm/include/usr_math.h"
float fan::get_sec_by_test(float *last_sec)
{
    static float sec = 0.0f;
    float k1 = get_sec_by_kal3();
    k1 = continue_sec_1(k1);
    float k2 = get_sec_by_slope() - fan_param.PRE_SLOPE_OFFSET/100.0f;
    if(k2 <= 0)
        k2 += 16.0f;
//    plot_y3 = static_cast<double>(k2);
    k2 = continue_sec_2(k2);
    float k3 = get_sec_by_time(*last_sec,m_fps);
    k3 = continue_sec_2(k3);
    sec = get_merge_sec(*last_sec,k1,k2,k3,fan_param.PRE_SPEED_PROBABILITY/100.0f,\
            fan_param.PRE_SLOPE_PROBABILITY/100.0f,fan_param.PRE_TIME_PROBABILITY/100.0f);
    plot_y2 = static_cast<double>(k1);
    plot_y3 = static_cast<double>(k2);
    plot_y4 = static_cast<double>(k3);
    if(!m_state_changing)
    {
        *last_sec = sec;
//        cout<<"stable"<<endl;
    }
    return sec;

/*    static float sec = 0.0f;
    float k4 = get_sec_by_kal1();
    k4 = continue_sec_4(k4);
    float k3 = get_sec_by_time(*last_sec,fps);
    k3 = continue_sec_2(k3);
//    plot_y1 = static_cast<double>(k4);
//    plot_y2 = static_cast<double>(k3);
    sec = k4;
    if(!is_unstable)
        *last_sec = sec;
*/
}
float fan::get_sec_by_kal1()
{
    static float spd_set = fan_param.PRE_SPEED_OFFSET;//75
    if(v_spd_kal1.size() == 5)
    {
        float mid_value = (v_spd_kal1[4]+v_spd_kal1[3])/2.0f;
        float slope = fabs(v_spd_kal1[4]) - fabs(v_spd_kal1[3]);
        float spd_compare = (fabs(mid_value) - spd_set)/fabs(45.0f);//set the model is 1
        return static_cast<float>(get_the_seclector_by_test(spd_compare,slope));
    }
    return 0.0f;
}
float fan::get_sec_by_kal3()
{
    static float spd_set = fan_param.PRE_SPEED_OFFSET;//75
    if(v_spd_kal3.size() == 5)
    {
        float mid_value = (v_spd_kal3[4]+v_spd_kal3[3])/2.0f;
        float slope = fabs(v_spd_kal3[4]) - fabs(v_spd_kal3[3]);
        float spd_compare = (fabs(mid_value) - spd_set)/fabs(45.0f)*fan_param.PRE_SPEED_RATIO/100.0f;//set the model is 1
        //plot_y2 = static_cast<double>(spd_compare);
        return static_cast<float>(get_the_seclector_by_test(spd_compare,slope));
    }
    return 0.0f;
}
float fan::get_sec_by_slope()
{
    static float slope_3 =0.0f;
    static float last_slope_3 =0.0f;
    if(v_spd_kal3.size() == 5)
    {
        float slope = fabs(v_spd_kal3[4]) - fabs(v_spd_kal3[3]);
        float slope_2 = kalman1_filter(&m_slope_kal2,slope);
        last_slope_3 = slope_3;
        slope_3 = kalman1_filter(&m_slope_kal3,slope_2);
//        plot_y2 = static_cast<double>(slope_3);
        float spd_compare = slope_3*100.0f/fan_param.PRE_SLOPE_SACLE;
        return static_cast<float>(get_the_seclector_by_test(spd_compare,slope_3-last_slope_3));
    }
    return 0.0f;
}
float fan::get_sec_by_time(float sec_last,float fps)
{
    static float time_gap = 1/fps*16/3.33f;
    float sec = sec_last + time_gap;
    while (sec > 16.0f)
    {
        sec -= 15.0f;
    }
    return sec;
}
float fan::continue_sec_1(float sec)
{
    static bool init_mark = 0;
    static float last_sec = 0;
    static float return_val=0;
    float diff = sec - last_sec;
    if(init_mark)
    {
       if(last_sec >= 12.0f)
       {
           if(diff < -8.0f)
               return_val = 1.0f;
           else if(diff >= 1.0f&&diff <= 4.0f)
               return_val =  last_sec + 1.0f;
       }
       else
       {
           if(diff >= 1.0f&&diff <= 5.0f)
               return_val =  last_sec + 1.0f;
       }
       if(return_val > 16.0f)
           return_val -= 16.0f;
    }
    else
        init_mark = true;
    last_sec = return_val;
    if(m_state_changing)
    {
        return_val = m_last_sec;
        last_sec = m_last_sec;
    }
    return return_val;
}
float fan::continue_sec_2(float sec)
{
    static bool init_mark = 0;
    static float last_sec = 0;
    static float return_val=0;
    float diff = sec - last_sec;
    if(init_mark)
    {
       if(last_sec >= 12.0f)
       {
           if(diff < -8.0f)
               return_val = 1.0f;
           else if(diff >= 1.0f&&diff <= 5.0f)
               return_val =  last_sec + 1.0f;
       }
       else
       {
           if(diff >= 1.0f&&diff <= 8.0f)
               return_val =  last_sec + 1.0f;
       }
       if(return_val > 16.0f)
           return_val -= 16.0f;
    }
    else
        init_mark = true;
    last_sec = return_val;
    if(m_state_changing)
    {
        return_val = m_last_sec;
        last_sec = m_last_sec;
    }
    return return_val;
}
float fan::continue_sec_3(float sec)
{
    static bool init_mark = 0;
    static float last_sec = 0;
    static float return_val=0;
    return_val = sec;
    float diff = sec - last_sec;
    if(init_mark)
    {
       if(last_sec >= 14.0f)
       {
           if(diff < 0.0f && diff > -1.0f)
               return_val = last_sec;
       }
    }
    else
        init_mark = true;
    last_sec = return_val;
    if(m_state_changing)
    {
        return_val = m_last_sec;
        last_sec = m_last_sec;
    }
    return return_val;
}
float fan::continue_sec_4(float sec)
{
    static bool init_mark = 0;
    static float last_sec = 0;
    static float return_val=0;
    float diff = sec - last_sec;
    if(init_mark)
    {
       if(last_sec >= 12.0f)
       {
           if(diff < -8.0f)
               return_val = 1.0f;
           else if(diff >= 1.0f&&diff <= 4.0f)
               return_val =  last_sec + 1.0f;
       }
       else
       {
           if(diff >= 1.0f&&diff <= 5.0f)
               return_val =  last_sec + 1.0f;
       }
       if(return_val > 16.0f)
           return_val -= 16.0f;
    }
    else
        init_mark = true;
    last_sec = return_val;
    if(m_state_changing)
    {
        return_val = m_last_sec;
        last_sec = m_last_sec;
    }
    return return_val;
}
//float fan::continue_sec_1(float sec)
//{
//    float return_val;
//    float diff = sec - m_last_sec;
//    if(m_last_sec >= 12.0f)
//    {
//       if(diff < -8.0f)
//           return_val = 1.0f;
//       else if(diff >= 1.0f&&diff <= 4.0f)
//           return_val =  m_last_sec + 1.0f;
//    }
//    else
//    {
//       if(diff >= 1.0f&&diff <= 5.0f)
//           return_val =  m_last_sec + 1.0f;
//    }
//    if(return_val > 16.0f)
//       return_val -= 16.0f;
//    return return_val;
//}
//float fan::continue_sec_2(float sec)
//{
//    float return_val;
//    float diff = sec - m_last_sec;
//    if(m_last_sec >= 12.0f)
//    {
//       if(diff < -8.0f)
//           return_val = 1.0f;
//       else if(diff >= 1.0f&&diff <= 5.0f)
//           return_val =  m_last_sec + 1.0f;
//    }
//    else
//    {
//       if(diff >= 1.0f&&diff <= 8.0f)
//           return_val =  m_last_sec + 1.0f;
//    }
//    if(return_val > 16.0f)
//       return_val -= 16.0f;
//    return return_val;
//}
//float fan::continue_sec_3(float sec)
//{
//    float return_val;
//    return_val = sec;
//    float diff = sec - m_last_sec;
//    if(m_last_sec >= 14.0f)
//    {
//       if(diff < 0.0f && diff > -1.0f)
//           return_val = m_last_sec;
//    }
//    return return_val;
//}
//float fan::continue_sec_4(float sec)
//{
//    float return_val;
//    float diff = sec - m_last_sec;
//    if(m_last_sec >= 12.0f)
//    {
//       if(diff < -8.0f)
//           return_val = 1.0f;
//       else if(diff >= 1.0f&&diff <= 4.0f)
//           return_val =  m_last_sec + 1.0f;
//    }
//    else
//    {
//       if(diff >= 1.0f&&diff <= 5.0f)
//           return_val =  m_last_sec + 1.0f;
//    }
//    if(return_val > 16.0f)
//       return_val -= 16.0f;
//    return return_val;
//}
float fan::get_merge_sec(float last_sec,float k1,float k2,float k3,float p1,float p2,float p3)
{
    float merge_val=0;
    if(k3 < 1.0f)
        k3 += 15.0f;
    float average = (k1+k2+k3)/3.0f;
    float diff = (fabs(k1-average)+fabs(k2-average)+fabs(k3-average));
    if(diff > 3)
    {
        if(k3 <= 1)
        {
            merge_val = 1;
        }
        else
        {
            float k1_delta = fabs(k1-last_sec);
            float k2_delta = fabs(k2-last_sec);
            if((fabs(k1-k2))<=6)
                merge_val = (k1+k2)/2.0f;
            else
                merge_val = k1_delta > k2_delta ? k2 : k1;
//            merge_val = k1;
        }
    }
    else
        merge_val = k1*p1+k2*p2+k3*p3;
    return merge_val;
}
float fan::get_merge_sec2(float last_sec,float k1,float k2,float p1,float p2)
{
    float merge_val=0;
    if(k2 < 1.0f)
        k2 += 15.0f;
    float average = (k1+k2)/2.0f;
    float diff = (fabs(k1-average)+fabs(k2-average));
    if(diff > 2)
    {
        if(k2 <= 1)
        {
            merge_val = 1;
        }
        else
        {
            merge_val = last_sec;
        }
    }
    else
        merge_val = k1*p1+k2*p2;
    return merge_val;
}
