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
       get_spd_vector(s_fan_spd.spd,m_rotate_spd,10);
    }
    if(mode == small_fan)
        m_predict_angle = 60.0f*shoot_delay;
    else
    {
        m_predict_angle = get_predict_angle_by_predict(m_rotate_spd,10,shoot_delay,50);
        //m_predict_angle = get_predict_angle(m_rotate_spd,10,shoot_delay);
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
}
void get_spd_vector(float spd,vector<float> &v_spd,uint8_t v_num)
{
    /**************** get the speed in previous 0.2s *****************/
    if(v_spd.size() == v_num)
    {
        for(size_t i = 0;i<v_num -1;i++)
            swap(v_spd[i],v_spd[i+1]);
        v_spd.pop_back();
    }
    v_spd.push_back(spd);

    /***************** continue the speed (because the speed we get is discrete and many speed is zero)******/
}

float get_predict_angle(vector<float> &v_spd,uint8_t v_num,float shoot_delay)
{
    uint start_num = get_start_section_num(v_spd,v_num,10);
    if(start_num)
    {
        float t0 = (start_num + 0.5f) * 3.33f/16.0f;
        float t1 = t0 + shoot_delay ;//the period is 3.33s
        float f0 = -0.785f/1.884f*cosf(1.884f*t0) + 1.305f*t0;
        float f1 = -0.785f/1.884f*cosf(1.884f*t1) + 1.305f*t1;
        return (f1-f0)*RAD_TO_ANGLE;
    }
    else
        return 0.0f;
}
