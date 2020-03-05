#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "algorithm/include/usr_math.h"
#include "fanwheel/fan_param.h"
#include "algorithm/include/fan_spd_filter.h"
/* @Des:this function is the run of find fanwheel
 * @param:Mat src--raw image
 * @retval:
 */
bool fan::run(Mat &src,\
              e_vision_mode_t mode,\
              color_ENUM color,\
              GetLightBarMethod_ENUM method,\
              bool is_show_img)
{
	m_is_show_img = is_show_img;
	//find roi
    m_roi_rect.x = 240;
    m_roi_rect.y = 0;
    m_roi_rect.width = 960;
    m_roi_rect.height = 840;
    m_roi_img = src(m_roi_rect);
    resize(m_roi_img, m_roi_img, Size(m_roi_img.cols*0.5, m_roi_img.rows*0.5));
    m_debug_img = m_roi_img.clone();
    adjust_param(0,method);
    get_binary(m_roi_img,color,method);
    if(find_armors(m_roi_img) > 0)
    {
        if (find_flow_fans(m_roi_img))
        {
            if(find_armor_in_flow_area())
            {
                if(find_flow_strip(m_roi_img))
                {
                    find_roi_of_R();
                    is_find_R = find_R_by_contour(m_roi_img);
                }
            }
        }
    }
    get_state();
    get_fan_spd();
    get_predict_point(mode,0.4f);
    if(m_is_show_img)
    {
        if(IS_CREATE_WINDOW)
        {
            namedWindow("debug", WINDOW_AUTOSIZE);
            imshow("debug", m_debug_img);
        }
    }
    if(m_state == 2||m_state == 3)
        return true;
    else
        return false;
}
