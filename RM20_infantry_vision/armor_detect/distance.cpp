#include "armor_detect/armor_detect.h"
#include "armor_detect/armor_param.h"
#include "algorithm/include/usr_math.h"
/* @Des:this function is to get the depth of armor
 * @param: img--the image to show
 * @retval:
 */
void armor_detect::get_armor_depth(color_ENUM color,GetLightBarMethod_ENUM method)
{
    static int small_area,big_area;
    switch(method)
    {
        case BGR_METHOD:
        {
            if(color == RED)
            {
                small_area = armor_param.DEPTH_BGR_RED_AREA_SMALL;
                big_area = armor_param.DEPTH_BGR_RED_AREA_BIG;
            }
            else
            {
                small_area = armor_param.DEPTH_BGR_BLUE_AREA_SMALL;
                big_area = armor_param.DEPTH_BGR_BLUE_AREA_BIG;
            }
            break;
        }
        case HSV_METHOD:
        {
            if(color == RED)
            {
                small_area = armor_param.DEPTH_HSV_RED_AREA_SMALL;
                big_area = armor_param.DEPTH_HSV_RED_AREA_BIG;
            }
            else
            {
                small_area = armor_param.DEPTH_HSV_BLUE_AREA_SMALL;
                big_area = armor_param.DEPTH_HSV_BLUE_AREA_BIG;
            }
            break;
        }
        case YUV_METHOD:
        {
            if(color == RED)
            {
                small_area = armor_param.DEPTH_YUV_RED_AREA_SMALL;
                big_area = armor_param.DEPTH_YUV_RED_AREA_BIG;
            }
            else
            {
                small_area = armor_param.DEPTH_YUV_BLUE_AREA_SMALL;
                big_area = armor_param.DEPTH_YUV_BLUE_AREA_BIG;
            }
            break;
        }
    }
    float project_ang = 100.0f * (m_useful_lightbars[1].size.height -\
                                    m_useful_lightbars[0].size.height) / m_useful_armors[0].size.height;
    project_ang = kalman1_filter(&g_angle_projection_kf, project_ang);
    float projection_out = 0.0411f*(fabs(project_ang))*(fabs(project_ang)) +\
                            14.0f * (fabs(project_ang)) + 8.6f;
    /* calculate the width/height to find the depth and whether it is small armor */
    float wh_ratio = m_useful_armors[0].size.width / m_useful_armors[0].size.height;
    if(wh_ratio < armor_param.DEPTH_SMALL_ARMOR_HW_RATIO_MAX/100.0f)
    {
        m_targetinfo.TargetType = SmallArmor;
        m_target_center.z = -projection_out + static_cast<float>((small_area*97.f/\
                            (sqrt(m_useful_armors[0].size.area()))));
    }
    else
    {
        m_targetinfo.TargetType = BigArmor;
        if(project_ang > 0)
            m_target_center.z = project_ang * 4 - projection_out +\
                                static_cast<float>((big_area*97.f/(sqrt(m_useful_armors[0].size.area()))));
        else
            m_target_center.z = -projection_out +\
                                static_cast<float>((big_area*97.f/(sqrt(m_useful_armors[0].size.area()))));
    }
    m_targetinfo.Trans_ratio = armor_param.LIGHTBAR_REALHEIGHT / (( m_useful_lightbars[0].size.height +\
                               m_useful_lightbars[1].size.height)/2.0f);
    if(m_is_show_img)
    {
        ellipse(m_debug_img, m_useful_armors[0], CV_RGB(255,87,64), 2, 8);//show ellipse
        ellipse(m_debug_img, m_useful_lightbars[0], CV_RGB(0, 0, 255), 2, 8);//show ellipse
        ellipse(m_debug_img, m_useful_lightbars[1], CV_RGB(0, 255, 0), 2, 8);//show ellipse
        circle(m_debug_img, m_useful_armors[0].center, 3, Scalar(120, 167, 61), 4, 8);
    }
}
