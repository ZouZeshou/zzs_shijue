#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "algorithm/include/usr_math.h"
#include "fanwheel/fan_param.h"
#include "algorithm/include/fan_spd_filter.h"
/* @Des:this function
 * @param:
 * @retval:
 */
fan::fan()
{
    init();
}
fan::~fan(){};
void fan::init()
{
    set_fps(50);
    set_init_mark(true);
    member_init();
    fan_param_init();
    fan_filter_init(&m_fan_filter1,0, 15, 0.1f, 5);
    fan_filter_init(&m_fan_filter2,0, 15, 0.5f, 5);
    kalman1_init(&m_spd_kal1,0, 200, 1,50);
    kalman1_init(&m_spd_kal2,0, 200, 1,50);
    kalman1_init(&m_spd_kal3,0, 200, 1,50);
    kalman1_init(&m_ang_kal1,0, 60, 1, 20);
    kalman1_init(&m_ang_kal2,0, 60, 1, 20);
    kalman1_init(&m_ang_kal3,0, 60, 1, 20);
    kalman1_init(&m_slope_kal1,0, 20, 1, 5);
    kalman1_init(&m_slope_kal2,0, 450, 1, 150);
    kalman1_init(&m_slope_kal3,0, 450, 1, 150);
    kalman1_init(&m_final_kal1,0, 2000, 1, 100);
}
void fan::member_init()
{
    memset(&s_fan_spd,0,sizeof(fan_spd_t));
    m_predict_angle = 0;
    m_predict_kal1 = 0;
    m_shoot_angle = 0;
    m_test_angle1 = 0;
    m_test_angle2 = 0;
    m_angle_last = 0;
    m_angle_now = 0;
    m_shoot_point = {0,0};
    m_target_point = {0,0};
    m_state = 0;//1--lose traget 2--target change 3-- catch target
    m_last_state = 0;
    m_last_sec = 0;
    m_direction = CW;//the rotate direction of fanwheel
    m_is_show_img = false;//is show image
    m_is_first_catch = false;//is first catch the target
    m_state_changing = false;
    m_last_target_point = {0,0};
    m_center_point = {0,0};//the center point of fanwheel
    is_find_R = false;//the signal to judge the mode
    m_armors.clear();//风车上所有装甲
    m_flow_strip_fans.clear();//流动条区域候选区
    m_target_armors.clear();//目标装甲板
    m_flow_strips.clear();//the candidate strip
    v_spd_kal1.clear();
    v_spd_kal3.clear();
}
