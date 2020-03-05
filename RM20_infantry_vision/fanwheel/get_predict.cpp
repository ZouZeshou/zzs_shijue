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
    static uint8_t cnt =0;
    if(mode == small_fan)
        m_predict_angle = 60.0f*shoot_delay;
    else
    {//big fan
        float sec=0;
        if(m_state == 3)
        {
            if(m_last_state==2)
            {
                m_state_changing = true;
                cnt = 0;
            }
            if(m_is_first_catch)
            {
               cnt = static_cast<uint8_t>((100*m_fps/1000.0f-1));
               m_is_first_catch = false;
            }
            if(m_state_changing == true && (cnt++ > (200*m_fps/1000.0f-1)))//200ms
            {
                m_state_changing =false;
                cnt = 0;
            }
            float temp_sec = get_sec_by_test(&m_last_sec);
            if(m_state_changing ==false)
            {
                sec = temp_sec;
            }
            else
                sec = get_sec_by_predict(&m_last_sec);
 //           plot_y1 = static_cast<double>(sec);
//            plot_y3 = static_cast<double>(sec);
        }
        else if(m_state == 2)
        {
            sec = get_sec_by_predict(&m_last_sec);
        }
//        plot_y2 = static_cast<double>(sec);
        m_predict_angle = get_predict_angle(sec,shoot_delay);
    }
    //m_predict_angle = kalman1_filter(&m_final_kal1,m_predict_angle)*1.05f;
    float radius = static_cast<float>(pointDistance(m_center_point,m_target_point));
    if(m_state == 3)
    {
        if(m_direction == CCW)
        {
            m_shoot_angle = s_fan_spd.polar_angle_now - m_predict_angle;
        }
        else
        {
            m_shoot_angle = s_fan_spd.polar_angle_now + m_predict_angle;
        }
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
