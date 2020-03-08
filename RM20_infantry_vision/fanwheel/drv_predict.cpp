#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "fanwheel/fan_param.h"
#include "algorithm/include/usr_math.h"
float fan::get_the_seclector_by_test(float spd_compare,float spd_diff)
{
    float sec = 0.0f;
    /************** select the block(1-16),i devide the sin curve to 16 section *************/
    static float sin_value[16];
    get_the_sin_value_by_section(sin_value);
//    cout<<"spd 0: "<<v_spd[v_num-5]<<endl;
//    cout<<"spd 1: "<<v_spd[v_num-4]<<endl;
//    cout<<"spd 2: "<<v_spd[v_num-3]<<endl;
//    cout<<"spd 3: "<<v_spd[v_num-2]<<endl;
//    cout<<"spd 4: "<<v_spd[v_num-1]<<endl;
//    cout<<"spd_now: "<<spd_now<<endl;
//    cout<<"spd_compare: "<<spd_compare<<endl;
//    for(int i =0;i<16;i++)
//        cout<<"sin"<<i<<": "<< sin_value[i]<<endl;
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
    //cout<<"sec: "<<sec<<endl;
    return sec;
}
float get_the_sin_value_by_section(float sin_val[16])
{
    for(int i=0;i<16;i++)
        sin_val[i] = sinf(2*PI/16.0f*i);
    return 0.0f;
}
