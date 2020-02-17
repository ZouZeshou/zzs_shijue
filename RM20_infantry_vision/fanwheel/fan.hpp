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
	void          param_init();
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
    int           detect_state();
    float         get_fan_spd();
    fan_spd_t                      s_fan_spd;//the struct of calculate fanwheel speed
    float                          m_predict_angle;//the angle is decided by shoot delay
    float                          m_shoot_angle;//the angle final to shoot
    float                          m_test_angle1;//
    float                          m_test_angle2;//
    Point2f                        m_shoot_point;//the point send to stm32
    Point2f                        m_target_point;//the center point of final armor
    Mat                            m_debug_img;//the image to show debug message
private:
    bool                           m_is_show_img;//is show image
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
    e_rotation_t                   m_direction;//the rotate direction of fanwheel
    kalman1_state                  m_spd_kal1;
    kalman1_state                  m_spd_kal2;
    kalman1_state                  m_spd_kal3;
    bool                           is_find_R;//the signal to judge the mode
    int                            m_state;//1--lose traget 2--target change 3-- catch target
    vector<float>                  m_rotate_spd;
};

void get_spd_vector(float spd,vector<float> &v_spd,uint8_t v_num);
uint get_the_seclector_by_test(vector<float> &v_spd,uint8_t v_num);
float get_the_sin_value_by_section(float sin_val[16]);
float get_predict_angle(vector<float> &v_spd,uint8_t v_num,float shoot_delay);
float get_predict_angle_by_predict(vector<float> &v_spd,uint8_t v_num,float shoot_delay,uint8_t fps);
uint get_start_section_num(vector<float> &v_spd,uint8_t v_num,uint8_t spd_diff);

#endif // FAN_CPP

