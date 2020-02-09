#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "algorithm/include/usr_math.h"
#include "fanwheel/fan_param.h"
/* @Des:this function
 * @param:
 * @retval:
 */
fan::fan()
{
    fan_param_init();
    kalman1_init(&m_angdiff_kal,0, 200, 5, 50);
}
fan::~fan(){};
/* @Des:this function is the run of find fanwheel
 * @param:Mat src--raw image
 * @retval:
 */
void fan::run(Mat &src, color_ENUM color,GetLightBarMethod_ENUM method,bool is_show_img)
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
    adjust_param(1,method);
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
    detect_state();
    get_fan_spd();
    get_predict_point(0.4f);
    if(m_is_show_img)
    {
        namedWindow("debug", WINDOW_AUTOSIZE);
        imshow("debug", m_debug_img);
    }
}
