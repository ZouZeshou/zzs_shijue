/* this file is to deal with the picture from video, aim to find the lightbar clearly
the second part
*/
#include "armor_detect/armor_detect.h"
#include "armor_detect/armor_param.h"
#include "algorithm/include/usr_math.h"
kalman1_state                     g_angle_projection_kf;
kalman1_state                     g_angle_cos_kf;
void gray1_bar_callback(int, void*){};


armor_detect::armor_detect(){
    kalman1_init(&g_angle_projection_kf, 0, 200, 5, 50);
    armor_param_init();
};
armor_detect::~armor_detect(){};
/* @Des:this function is to get the target center of armor
 * @param:img--raw image
 * @retval:
 */
bool armor_detect::run(Mat &src,color_ENUM color,GetLightBarMethod_ENUM method,bool is_show_image)
{
    bool is_find_target = false;
    m_is_show_img = is_show_image;
    m_roi_rect.x = 200;
    m_roi_rect.y = 200;
    m_roi_rect.width = 720;//1440
    m_roi_rect.height = 420;//480
    m_roi_img = src(m_roi_rect);
    //resize(m_roi_img, m_roi_img, Size(m_roi_img.cols*0.5, m_roi_img.rows*0.5));
    m_debug_img = m_roi_img.clone();
    adjust_param(3,method);
    get_binary_img(m_roi_img,color,method);/*true--debug*/
    if(get_lightbar(m_roi_img))
    {
        get_armors();
        if(get_target_armor())
        {
            get_armor_depth(color,method);
            is_find_target = true;
        }
    }
    if(m_is_show_img)
    {
        namedWindow("debug_img",WINDOW_NORMAL);
        imshow("debug_img",m_debug_img);
    }
    return is_find_target;
}
