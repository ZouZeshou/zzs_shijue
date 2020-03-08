/* this file is to deal with the picture from video, aim to find the lightbar clearly
the second part
*/
#include "armor_detect/armor_detect.h"
#include "armor_detect/armor_param.h"
#include "algorithm/include/usr_math.h"
void gray1_bar_callback(int, void*){};
/* @Des:this function is to get the target center of armor
 * @param:img--raw image
 * @retval:
 */
bool armor_detect::run(Mat &src,color_ENUM color,GetLightBarMethod_ENUM method,bool is_show_image)
{
    bool is_find_target = false;
    m_is_show_img = is_show_image;
    m_roi_rect.x = 0;
    m_roi_rect.y = 0;
    m_roi_rect.width = 1440;//1440
    m_roi_rect.height = 840;//480
    m_roi_img = src(m_roi_rect);
    m_debug_img = m_roi_img.clone();
    adjust_param(0,method);
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
        if(IS_CREATE_WINDOW)
        {
            namedWindow("debug_img",WINDOW_NORMAL);
            imshow("debug_img",m_debug_img);
        }
    }
    return is_find_target;
}
