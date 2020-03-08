#include "armor_detect/armor_detect.h"
#include "armor_detect/armor_param.h"
#include "algorithm/include/usr_math.h"
/* @Des:this function is to get the  armors from lightbars
 * @retval:
 */
uint armor_detect::get_armors(void)
{
    m_useful_armors.clear();
    for( size_t i = 0; i < m_lightbars.size() - 1; ++i)
    {
        for( size_t j = i +1; j < m_lightbars.size(); ++j)
        {
            /******* first filter****************************************************/
            float angle_diff = fabs(m_lightbars[i].angle - m_lightbars[j].angle);
            angle_diff = loop_float_constrain(angle_diff,-90,90);
            float width_diff = fabs(m_lightbars[i].size.width - m_lightbars[j].size.width );
            float height_diff = fabs(m_lightbars[i].size.height - m_lightbars[j].size.height);
            float width_sum = fabs(m_lightbars[i].size.width + m_lightbars[j].size.width);
            float height_sum = fabs(m_lightbars[i].size.height + m_lightbars[j].size.height);
            if((fabs(angle_diff) > armor_param.ARMOR_ANGLE_DIFF/100.0f ) ||\
                    (width_diff/width_sum > armor_param.ARMOR_WIDTH_DIFF_RATIO/100.0f ) ||\
                    (height_diff/height_sum > armor_param.ARMOR_LENGTH_DIFF_RATIO/100.0f ))
            {
                continue;
            }
            /*************second filter ********************************************/
            float center_angle = abs_angle_diff(m_lightbars[i].center,m_lightbars[j].center);
            if(center_angle > armor_param.ARMOR_MAX_HORIZONTAL_ANGLE)
            {
                continue;
            }
            /************third filter ***********************************************/
            float temparmor_height = (m_lightbars[i].size.height + m_lightbars[j].size.height) / 2.0f ;
            float temparmor_width  = static_cast<float>(pointDistance(m_lightbars[i].center,m_lightbars[j].center));
            float temparmor_area = temparmor_width * temparmor_height;
            float WH_prop = temparmor_width / temparmor_height;
            if(temparmor_area > armor_param.ARMOR_AREA_MAX*100.0f||
               temparmor_area < armor_param.ARMOR_AREA_MIN*100.0f||
               WH_prop > armor_param.ARMOR_HW_RATIO_MAX/100.0f||
               WH_prop < armor_param.ARMOR_HW_RATIO_MIN/100.0f)
            {
                continue;
            }
            /************ get the rect *********************************************/
            RotatedRect  _temp_rect;
            if(m_lightbars[i].center.x - m_lightbars[j].center.x > 0.0f)
            {
                if(m_lightbars[i].center.y - m_lightbars[j].center.y > 0.0f)
                    _temp_rect.angle = -center_angle;
                m_useful_lightbars[0] = (m_lightbars[j]);
                m_useful_lightbars[1]  = (m_lightbars[i]);
            }
            else if(m_lightbars[i].center.x - m_lightbars[j].center.x < 0.0f)
            {
                if(m_lightbars[i].center.y - m_lightbars[j].center.y > 0.0f)
                    _temp_rect.angle = -center_angle;
                m_useful_lightbars[0] = (m_lightbars[i]);
                m_useful_lightbars[1] = (m_lightbars[j]);
            }
            _temp_rect.size.width = temparmor_width;
            _temp_rect.size.height = temparmor_height;
            _temp_rect.center = mid_point(m_lightbars[i].center,m_lightbars[j].center);
            m_useful_armors.push_back(_temp_rect);
        }
    }
    if(m_is_show_img)
    {
        for(auto &armor:m_useful_armors)
            ellipse(m_debug_img, armor, CV_RGB(0, 255, 0), 2, 8);
    }
    return static_cast<uint>(m_useful_armors.size());
}

/* @Des:this function is to deal with many armors
 * @param: img--the image to show
 * @retval:
 */
bool armor_detect::get_target_armor(void)
{
    if(m_useful_armors.size() >= 1)
    {
        if(m_useful_armors.size() >= 2)
        {
            /* sort by the distance between the armor center and screen center  */
            sort(m_useful_armors.begin(),m_useful_armors.end(),CenterDisSort);/*close to far*/
            //cout << "area   "  << m_pair_armor_lightbars[0].first.size.area() << endl;
        }
         /* give the final pair to the g_tragetinfo */
        m_target_center.x = m_useful_armors[0].center.x;
        m_target_center.y = m_useful_armors[0].center.y;
        return true;
    }
    return false;
}
