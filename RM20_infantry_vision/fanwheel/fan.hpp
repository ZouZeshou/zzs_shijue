#ifndef FAN_CPP
#define FAN_CPP
#include "math.h"
#include "main/global.h"
#include "algorithm/include/kalman_filter.hpp"
#include "plot/mainwindow.h"
class fan
{
public:
    fan();
    ~fan();
    void          init();
    void          member_init();
    bool          run(Mat &src, e_vision_mode_t mode, color_ENUM color, GetLightBarMethod_ENUM method, bool is_show_img);
    void          adjust_param(uint8_t type,GetLightBarMethod_ENUM method);
    Mat           get_binary(Mat &src, color_ENUM color,GetLightBarMethod_ENUM method);
    int           find_armors(const Mat &src);
    bool          find_flow_fans(const Mat &src);
    bool          find_armor_in_flow_area();
    bool          find_flow_strip(const Mat &src);
    bool          find_roi_of_R();
    bool          find_R_by_contour(const Mat &src);
    bool          find_R_by_crosspoint();
    bool          get_predict_point(e_vision_mode_t mode,float shoot_delay);
    int           get_state();
    float         get_fan_spd();
    float         get_sec_by_test(float *last_sec);
    float         get_sec_by_predict(float *last_sec);
    float         get_predict_angle(float sec,float shoot_delay);
    void          get_spd_vector(float spd,vector<float> &v_spd,uint8_t v_num);
    float         get_the_seclector_by_test(float spd_now,float spd_diff);
    float         get_sec_by_time(float sec_last,float fps);
    float         get_sec_by_kal3();
    float         get_sec_by_kal1();
    float         get_sec_by_slope();
    float         continue_sec_1(float sec);
    float         continue_sec_2(float sec);
    float         continue_sec_3(float sec);
    float         continue_sec_4(float sec);
    float         get_merge_sec(float last_sec,float k1,float k2,float k3,float p1,float p2,float p3);
    float         get_merge_sec2(float last_sec,float k1,float k2,float p1,float p2);
    void          set_init_mark(bool mark);
    void          set_fps(float fps);
    fan_spd_t                      s_fan_spd;//the struct of calculate fanwheel speed
    float                          m_angle_now;
    float                          m_angle_last;
    float                          m_predict_angle;//the angle is decided by shoot delay
    float                          m_predict_kal1;
    float                          m_shoot_angle;//the angle final to shoot
    float                          m_test_angle1;//
    float                          m_test_angle2;//
    Point2f                        m_shoot_point;//the point send to stm32
    Point2f                        m_target_point;//the center point of final armor
    Mat                            m_debug_img;//the image to show debug message
    int                            m_state;//1--lose traget 2--target change 3-- catch target
    int                            m_last_state;
    float                          m_last_sec;
    e_rotation_t                   m_direction;//the rotate direction of fanwheel
private:
    bool                           m_is_show_img;//is show image
    bool                           m_is_first_catch;//is first catch the target
    bool                           m_state_changing;
    Rect                           m_roi_rect;
    RotatedRect                    m_target_armor;//the final armor in flow strip fan
    RotatedRect                    m_flow_strip;//the final flow strip
    RotatedRect                    m_center_R;//the center of fanwheel
    vector<RotatedRect>            m_armors;//风车上所有装甲
    vector<RotatedRect>            m_flow_strip_fans;//流动条区域候选区
    vector<RotatedRect>            m_target_armors;//目标装甲板
    vector<RotatedRect>            m_flow_strips;//the candidate strip
    Mat                            m_roi_img;//roi image
    Point2f                        m_last_target_point;
    RotatedRect                    m_center_ROI;//能量机关中心候选区
    Point2f                        m_center_point;//the center point of fanwheel
    kalman1_state                  m_fan_filter1;
    kalman1_state                  m_fan_filter2;
    kalman1_state                  m_spd_kal1;
    kalman1_state                  m_spd_kal2;
    kalman1_state                  m_spd_kal3;
    kalman1_state                  m_ang_kal1;
    kalman1_state                  m_ang_kal2;
    kalman1_state                  m_ang_kal3;
    kalman1_state                  m_slope_kal1;
    kalman1_state                  m_slope_kal2;
    kalman1_state                  m_slope_kal3;
    kalman1_state                  m_final_kal1;
    bool                           is_find_R;//the signal to judge the mode
    vector<float>                  v_spd_kal1;
    vector<float>                  v_spd_kal3;
    float                          m_fps;
};
float get_the_sin_value_by_section(float sin_val[16]);


#endif // FAN_CPP

