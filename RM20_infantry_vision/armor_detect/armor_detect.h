#ifndef ARMOR_DETECT_H
#define ARMOR_DETECT_H
#include "main/global.h"
#include "algorithm/include/kalman_filter.hpp"
#include "plot/mainwindow.h"
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
    bool run(Mat &src,color_ENUM color,GetLightBarMethod_ENUM method,bool is_show_image);
    void get_binary_img(Mat &src,color_ENUM color,GetLightBarMethod_ENUM method);
    bool get_lightbar(Mat src);
    uint get_armors(void);
    bool get_target_armor(void);
    void get_armor_depth(color_ENUM color,GetLightBarMethod_ENUM method);
    void adjust_param(uint8_t type,GetLightBarMethod_ENUM method);
public:
    TargetInfo                              m_targetinfo;
    Point3f                                 m_target_center;
private:
    int                                     m_is_debug;
    vector<RotatedRect>                     m_lightbars;
    vector<RotatedRect>                     m_useful_armors;
    RotatedRect                             m_useful_lightbars[25];/*the 25 is the random number ,it also can be 30\40, but not too small */
    Rect                                    m_roi_rect;
    Mat                                     m_roi_img;
    Mat                                     m_debug_img;
    bool                                    m_is_show_img;

};


extern kalman1_state                        g_angle_projection_kf;
extern kalman1_state                        g_angle_cos_kf;
void gray1_bar_callback(int, void*);
void lightbar_erode_dilate(Mat & src);
bool isValidLightbarContour(const vector<Point> &armor_contour);

#endif
