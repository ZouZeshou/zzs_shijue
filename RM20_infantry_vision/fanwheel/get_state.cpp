#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "algorithm/include/usr_math.h"
#include "fanwheel/fan_param.h"
/* @Des:this function is detect the mode of fanwheel find(catch,lose,change)
 * @param:
 * @retval:state:1--lose traget 2--target change 3-- catch target
 */
int fan::get_state()
{
    static uint8_t cnt;
    m_angle_last = m_angle_now;
    m_angle_now = atan2(m_target_point.y - m_center_point.y,m_target_point.x - m_center_point.x)*RAD_TO_ANGLE;
    m_last_state = m_state;
    if(!is_find_R)
    {
      cnt = 0;
      m_state = 1;
    }
    else
    {
        if(fabs(loop_float_constrain(m_angle_last-m_angle_now,-180.0f,180.0f)) > fan_param.TARGET_CHANGE_ANGLE)
        {
           cnt = 0;
           m_state = 2;
        }
        else
        {
            if(cnt++ > (100*m_fps/1000.0f-1))//100ms
            {
                cnt = 0;
                m_state = 3;
            }
        }
//        if(m_test_angle1>130&&m_test_angle1<180)
//            m_state = 2;
    }
    return m_state;
}
/* @Des:this function is change the value of m_is_first_catch
 * @param:mark(true--is first step)
 */
void fan::set_init_mark(bool mark)
{
   m_is_first_catch = mark;
}
void fan::set_fps(float fps)
{
   m_fps = fps;
}
