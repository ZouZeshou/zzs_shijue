#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "algorithm/include/usr_math.h"
#include "fanwheel/fan_param.h"
#include "algorithm/include/fan_spd_filter.h"
/* @Des:this function is get the speed of fanwheel
 * @param:
 * @retval:
 */
float fan::get_fan_spd()
{
    static uint8_t cnt = 0;
    static uint8_t cnt1 = 0;
    if(m_state == 3)
    {
        s_fan_spd.polar_angle_now = atan2(m_target_point.y - m_center_point.y,m_target_point.x - m_center_point.x)*RAD_TO_ANGLE;
        if(cnt++ >= (40*m_fps/1000.0f-1))//40ms
        {
            cnt = 0;
            float ang_diff = s_fan_spd.polar_angle_now - s_fan_spd.polar_angle_last;
            ang_diff = loop_float_constrain(ang_diff,-180.0f,180.0f);
            s_fan_spd.spd = (ang_diff)/0.04f;
//            s_fan_spd.spd_kal1 = -fan_filter_update(&m_fan_filter1,-s_fan_spd.spd,m_last_sec,50);
//            s_fan_spd.spd_kal1 = -fan_filter_update(&m_fan_filter2,-s_fan_spd.spd_kal1,m_last_sec,50);
            s_fan_spd.spd_kal1 = kalman1_filter(&m_spd_kal1,s_fan_spd.spd);
            s_fan_spd.spd_kal2 = kalman1_filter(&m_spd_kal2,s_fan_spd.spd);
            s_fan_spd.spd_kal3 = kalman1_filter(&m_spd_kal3,s_fan_spd.spd_kal2);
//                plot_y1 = static_cast<double>(ang_diff);
            if(ang_diff > 0)
                m_direction = CW;
            else if(ang_diff < 0)
                m_direction = CCW;
//            cout<<"print***********************************************"<<endl;
//            cout<<"rotate_speed: "<<s_fan_spd.spd<<endl;
//            cout<<"m_direction: "<<m_direction<<endl;
//            cout<<"ang_now: "<<s_fan_spd.polar_angle_now<<endl;
//            cout<<"anf_last: "<<s_fan_spd.polar_angle_last<<endl;
            s_fan_spd.polar_angle_last = s_fan_spd.polar_angle_now;
        }
    }
    else
    {
        cnt = 0;
        s_fan_spd.polar_angle_last = atan2(m_target_point.y - m_center_point.y,m_target_point.x - m_center_point.x)*RAD_TO_ANGLE;
    }
    if(cnt1++ >= (40*m_fps/1000.0f-1))//40ms
    {
       cnt1 = 0;
       get_spd_vector(s_fan_spd.spd_kal3,v_spd_kal3,5);
       get_spd_vector(s_fan_spd.spd_kal1,v_spd_kal1,5);
    }
    static float t = 0;
    s_fan_spd.stard_spd = (0.785f*sin(1.884f*(t++)*0.02f)+1.305f)*RAD_TO_ANGLE;
    return s_fan_spd.spd;
}
void fan::get_spd_vector(float spd,vector<float> &v_spd,uint8_t v_num)
{
    if(v_spd.size() == v_num)
    {
        for(size_t i = 0;i<v_num -1;i++)
            swap(v_spd[i],v_spd[i+1]);
        v_spd.pop_back();
    }
    v_spd.push_back(spd);
}
