#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "algorithm/include/usr_math.h"
#include "fanwheel/fan_param.h"
/* @Des:this function is detect the mode of fanwheel find(catch,lose,change)
 * @param:
 * @retval:state:1--lose traget 2--target change 3-- catch target
 */
int fan::detect_state()
{
    static uint8_t cnt;
    if(!is_find_R)
    {
      cnt = 0;
      m_state = 1;
    }
    else
    {
        if(pointDistance(m_target_point,m_last_target_point) > 50)
        {
           cnt = 0;
           m_state = 2;
        }
        else
        {
            if(cnt++ > 5)
            {
                cnt = 0;
                m_state = 3;
            }
        }
    }
    return m_state;
}
/* @Des:this function is get the speed of fanwheel
 * @param:
 * @retval:
 */
float fan::get_fan_spd()
{
    static uint8_t cnt = 0;
    if(m_state == 3)
    {
        s_fan_spd.polar_angle_now = atan2(m_target_point.y - m_center_point.y,m_target_point.x - m_center_point.x)*RAD_TO_ANGLE;
        if(cnt++ >= 1)//40ms
        {
            cnt = 0;
            float ang_diff = s_fan_spd.polar_angle_now - s_fan_spd.polar_angle_last;
            ang_diff = loop_float_constrain(ang_diff,-180.0f,180.0f);
            s_fan_spd.spd = (ang_diff)/0.04f;
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
        s_fan_spd.polar_angle_last = atan2(m_target_point.y - m_center_point.y,m_target_point.x - m_center_point.x)*RAD_TO_ANGLE;;
        s_fan_spd.spd = 0.0f;
        s_fan_spd.spd_kal1 = 0.0f;
        s_fan_spd.spd_kal2 = 0.0f;
        s_fan_spd.spd_kal3 = 0.0f;
    }
    return s_fan_spd.spd;
}
