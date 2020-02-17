#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "fanwheel/fan_param.h"
#include "algorithm/include/usr_math.h"
float get_predict_angle_by_predict(vector<float> &v_spd,uint8_t v_num,float shoot_delay,uint8_t fps)
{
    static uint8_t init_signal = 0;
    static float  start_time = 0.0;
    uint start_num = get_start_section_num(v_spd,v_num,10);
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
uint get_start_section_num(vector<float> &v_spd,uint8_t v_num,uint8_t spd_diff)
{
    if(v_spd.size() == v_num)
    {
        float mid_value = (v_spd[v_num-5]+v_spd[v_num-4]+v_spd[v_num-3]+v_spd[v_num-2]+v_spd[v_num-1])/5.0f;
        for(size_t i=0;i<v_num;i++)
        {
            if(v_spd[i] == 0.0f)
                return 0;
            if(i > v_num-6)
            {
                float delta = fabs(v_spd[i] - mid_value);
                if(delta > spd_diff)
                    return 0;
            }
        }
        return get_the_seclector_by_test(v_spd,v_num);
    }
}
uint get_the_seclector_by_test(vector<float> &v_spd,uint8_t v_num)
{
    static uint sec = 0;
    static uint last_sec = 0;
    static float spd_set = 75;//75
    /************** select the block(1-16),i devide the sin curve to 16 section *************/
    float spd_now =  fabs((v_spd[v_num-5]+v_spd[v_num-4]+v_spd[v_num-3]+v_spd[v_num-2]+v_spd[v_num-1])/5.0f);
    float spd_compare = (fabs(spd_now) - spd_set)/fabs(45.0f)*90/65.0f;//set the model is 1
    float spd_diff = fabs(v_spd[v_num-5]+v_spd[v_num-4]+v_spd[v_num-3]+v_spd[v_num-2]+v_spd[v_num-1]) -\
                     fabs(v_spd[v_num-10]+v_spd[v_num-9]+v_spd[v_num-8]+v_spd[v_num-7]+v_spd[v_num-6]);
    static float sin_value[16];
    get_the_sin_value_by_section(sin_value);
//    cout<<"spd 0: "<<v_spd[v_num-5]<<endl;
//    cout<<"spd 1: "<<v_spd[v_num-4]<<endl;
//    cout<<"spd 2: "<<v_spd[v_num-3]<<endl;
//    cout<<"spd 3: "<<v_spd[v_num-2]<<endl;
//    cout<<"spd 4: "<<v_spd[v_num-1]<<endl;
//    cout<<"spd_now: "<<spd_now<<endl;
//    cout<<"spd_compare: "<<spd_compare<<endl;
//    cout<<"spd_diff: "<<spd_diff<<endl;
//    for(int i =0;i<16;i++)
//        cout<<"sin"<<i<<": "<< sin_value[i]<<endl;
    last_sec = sec;
    if(spd_compare >= 0)
    {//section 1-8
        if(spd_compare >= sin_value[2])
        {//section 3456
            if(spd_compare >= sin_value[3])
            {//section 4,5
               if(spd_diff > 0.0f)
                   sec = 4;
               else
                   sec = 5;
            }
            else
            {//section 3,6
                if(spd_diff > 0.0f)
                    sec = 3;
                else
                    sec = 6;
            }
        }
        else
        {//section 1278
            if(spd_compare >= sin_value[1])
            {//section 2,7
                if(spd_diff > 0.0f)
                    sec = 2;
                else
                    sec = 7;
            }
            else
            {//section 1,8
                if(spd_diff > 0.0f)
                    sec = 1;
                else
                    sec = 8;
            }
        }
    }
    else
    {//section 9-16
        if(spd_compare <= sin_value[10])
        {//section 11,12,13,14
            if(spd_compare <= sin_value[11])
            {//section 12,13
                if(spd_diff > 0.0f)
                    sec = 13;
                else
                    sec = 12;
            }
            else
            {//section 11,14
                if(spd_diff > 0.0f)
                    sec = 14;
                else
                    sec = 11;
            }
        }
        else
        {//section 9,10,15,16
            if(spd_compare <= sin_value[9])
            {//section 10,15
                if(spd_diff > 0.0f)
                    sec = 15;
                else
                    sec = 10;
            }
            else
            {//section 9,16
                if(spd_diff > 0.0f)
                    sec = 16;
                else
                    sec = 9;
            }
        }
    }
    if(sec < last_sec)
    {
        if(sec != 1)
            sec = last_sec;
    }
    cout<<"sec: "<<sec<<endl;
    return sec;
}
float get_the_sin_value_by_section(float sin_val[16])
{
    for(int i=0;i<16;i++)
        sin_val[i] = sinf(2*PI/16.0f*i);
}
