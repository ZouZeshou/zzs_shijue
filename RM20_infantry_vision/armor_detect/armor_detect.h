#ifndef ARMOR_DETECT_H
#define ARMOR_DETECT_H
#include "main/global.h"
#include "algorithm/include/kalman_filter.hpp"
#define LIGHTBAR_REALHEIGHT 60//mm

static inline float CalRectCenterDis(RotatedRect a)
{
    return sqrt((a.center.x - pixelCenterX)*(a.center.x - pixelCenterX) +\
                (a.center.y - pixelCenterY)*(a.center.y - pixelCenterY));
}

static inline bool CenterDisSort(RotatedRect a1,RotatedRect a2)
{
    return CalRectCenterDis(a1) < CalRectCenterDis(a2);
}
static inline bool AreaSort(RotatedRect a1,RotatedRect a2)
{
    return a1.size.area() > a2.size.area();
}

class armor_detect
{

public:
    armor_detect();
    ~armor_detect();
    void get_armor_center(Mat img,color_ENUM color,GetLightBarMethod_ENUM method,bool is_show_image);
    Mat get_binary_img(Mat initial_img, color_ENUM color,GetLightBarMethod_ENUM method,bool is_debug);
    void get_lightbar(Mat initial_img);
    void get_armors(void);
    void deal_muti_armors(void);
    void get_armor_depth(Mat img,color_ENUM color,GetLightBarMethod_ENUM method);
public:
    TargetInfo                              m_targetinfo;
    Point3f                                 m_target_center;
private:
    int                                     m_is_debug;
    Mat                                     m_raw_img;
    Mat                                     m_binary_img;
    vector<RotatedRect>                     m_lightbars;
    vector<RotatedRect>                     m_useful_armors;
    RotatedRect                             m_useful_lightbars[25];/*the 25 is the random number ,it also can be 30\40, but not too small */
    Rect                                    m_roi_rect;
    Mat                                     m_roi_img;
    bool                                    m_show_image;

};


extern kalman1_state                        g_angle_projection_kf;
extern kalman1_state                        g_angle_cos_kf;
void gray1_bar_callback(int, void*);

#endif
