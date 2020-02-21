#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "fanwheel/fan_param.h"
#include "algorithm/include/usr_math.h"
/* @Des:this function is to get the predict point
 * @param:Mat src--raw image
 * @retval:find or not
 */
bool fan::get_predict_point(e_vision_mode_t mode,float shoot_delay)
{
    static uint8_t cnt = 0;
    if(cnt++ >= 1)
    {
       cnt = 0;
       get_spd_vector(s_fan_spd.spd_kal1,v_spd_kal1,15);
       get_spd_vector(s_fan_spd.spd_kal3,v_spd_kal3,5);
    }
    if(mode == small_fan)
        m_predict_angle = 60.0f*shoot_delay;
    else
    {
        m_predict_angle = get_predict_angle_by_predict(shoot_delay,50);
        //m_predict_angle = get_predict_angle_by_test(shoot_delay);
    }
    //cout<<"m_predict_angle:"<<m_predict_angle<<endl;
    float radius = static_cast<float>(pointDistance(m_center_point,m_target_point));
    if(m_direction == CCW)
    {
        m_shoot_angle = s_fan_spd.polar_angle_now - m_predict_angle;
    }
    else
    {
        m_shoot_angle = s_fan_spd.polar_angle_now + m_predict_angle;
    }
    m_shoot_angle = loop_float_constrain(m_shoot_angle,-180.0f,180.0f);
    m_shoot_point.x = m_center_point.x + radius * cosf(m_shoot_angle*ANGLE_TO_RAD);
    m_shoot_point.y = m_center_point.y + radius * sinf(m_shoot_angle*ANGLE_TO_RAD);
    m_test_angle1 = atan2(m_target_point.y - m_center_point.y,m_target_point.x - m_center_point.x)*RAD_TO_ANGLE;
    m_test_angle2 = atan2(m_shoot_point.y - m_center_point.y,m_shoot_point.x - m_center_point.x)*RAD_TO_ANGLE;
    if(m_is_show_img)
    {
       circle(m_debug_img, m_target_point,2, Scalar(255, 255, 255), 2, 8);
       circle(m_debug_img, m_shoot_point, 2, Scalar(255, 255, 255), 2, 8);
       circle(m_debug_img, m_center_point, static_cast<int>(radius), Scalar(0,255, 0), 1, 8);
       //cout<<"section_number :"<<section_number<<endl;
    }
    return true;
}
