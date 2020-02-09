#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "fanwheel/fan_param.h"
#include "algorithm/include/usr_math.h"
/* @Des:this function is to get the predict point
 * @param:Mat src--raw image
 * @retval:find or not
 */
bool fan::get_predict_point(float shoot_delay)
{
    static uint8_t cnt = 0;
    if(cnt++ >= 10)
    {
       cnt = 0;
       get_spd_vector(s_fan_spd.spd,m_rotate_spd);
       for(int i=0;i<3;i++)
            cout<<"spd_array"<<i<< ": "<<m_rotate_spd[i]<<endl;
    }
    uint section_number = get_the_seclector(m_rotate_spd);
    m_predict_angle = get_predict_angle(section_number,shoot_delay);
    cout<<"m_predict_angle:"<<m_predict_angle<<endl;
    float radius = static_cast<float>(pointDistance(m_center_point,m_target_point));
    if(m_direction == CCW)
    {
        m_shoot_point.x = m_center_point.x + radius * cosf((s_fan_spd.polar_angle_now-m_predict_angle)*ANGLE_TO_RAD);
        m_shoot_point.y = m_center_point.y + radius * sinf((s_fan_spd.polar_angle_now-m_predict_angle)*ANGLE_TO_RAD);
    }
    else
    {
        m_shoot_point.x = m_center_point.x + radius * cosf((s_fan_spd.polar_angle_now+m_predict_angle)*ANGLE_TO_RAD);
        m_shoot_point.y = m_center_point.y + radius * sinf((s_fan_spd.polar_angle_now+m_predict_angle)*ANGLE_TO_RAD);
    }
    if(m_is_show_img)
    {
       circle(m_debug_img, m_shoot_point, 2, Scalar(255, 255, 255), 2, 8);
       circle(m_debug_img, m_center_point, static_cast<int>(radius), Scalar(0,255, 0), 1, 8);
       cout<<"section_number :"<<section_number<<endl;
    }
}
void get_spd_vector(float spd,float spd_vector[])
{
    /**************** get the speed in previous 3.3s *****************/
    static uint index = 0;

    if(index == 3)
    {
        for(int i = 0;i < 2;i++)
            spd_vector[i] = spd_vector[i+1];
    }
    else
        index++;
    spd_vector[index - 1] = spd;
    cout<<"index:"<<index<<endl;
    /***************** continue the speed (because the speed we get is discrete and many speed is zero)******/
    if(index == 3)
    {
       for(int i = 2; i >= 0;i--)
       {
           if(spd_vector[i]==0.0f)
           {
               for(int j = i; j >= 0;j--)
               {
                   if(spd_vector[j]!=0.0f)
                   {
                     spd_vector[i] =  spd_vector[j];
                     break;
                   }
               }
           }
       }
    }

}
uint get_the_seclector(float spd_vector[])
{
    static float spd_set = 75;//75
    /************** select the block(1-16),i devide the sin curve to 16 section *************/
    float spd_now =  fabs((spd_vector[0]+spd_vector[1]+spd_vector[2])/3.0f);
    float spd_compare = (fabs(spd_now) - spd_set)/fabs(45.0f);//set the model is 1
    float spd_diff = fabs(spd_vector[2])- fabs(spd_vector[0]);
    static float sin_value[16];
    get_the_sin_value_by_section(sin_value);
    cout<<"spd 0: "<<spd_vector[0]<<endl;
    cout<<"spd_now: "<<spd_now<<endl;
    cout<<"spd_compare: "<<spd_compare<<endl;
    cout<<"spd_diff: "<<spd_diff<<endl;
//    for(int i =0;i<16;i++)
//        cout<<"sin"<<i<<": "<< sin_value[i]<<endl;
    if(spd_compare >= 0)
    {//section 1-8
        if(spd_compare >= sin_value[2])
        {//section 3456
            if(spd_compare >= sin_value[3])
            {//section 4,5
               if(spd_diff > 0.0f)
                   return 4;
               else
                   return 5;
            }
            else
            {//section 3,6
                if(spd_diff > 0.0f)
                    return 3;
                else
                    return 6;
            }
        }
        else
        {//section 1278
            if(spd_compare >= sin_value[1])
            {//section 2,7
                if(spd_diff > 0.0f)
                    return 2;
                else
                    return 7;
            }
            else
            {//section 1,8
                if(spd_diff > 0.0f)
                    return 1;
                else
                    return 8;
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
                    return 13;
                else
                    return 12;
            }
            else
            {//section 11,14
                if(spd_diff > 0.0f)
                    return 14;
                else
                    return 11;
            }
        }
        else
        {//section 9,10,15,16
            if(spd_compare <= sin_value[9])
            {//section 10,15
                if(spd_diff > 0.0f)
                    return 15;
                else
                    return 10;
            }
            else
            {//section 9,16
                if(spd_diff > 0.0f)
                    return 16;
                else
                    return 9;
            }
        }
    }
}
float get_the_sin_value_by_section(float sin_val[16])
{
    for(int i=0;i<16;i++)
        sin_val[i] = sinf(2*PI/16.0f*i);
}
float get_predict_angle(uint sec_num,float shoot_delay)
{
    float t0 = (sec_num -0.5f) * 3.33f/16.0f;
    float t1 = t0 + shoot_delay ;//the period is 3.33s
    float f0 = -0.785f/1.884f*cosf(1.884f*t0) + 1.305f*t0;
    float f1 = -0.785f/1.884f*cosf(1.884f*t1) + 1.305f*t1;
    return (f1-f0)*RAD_TO_ANGLE;
}
