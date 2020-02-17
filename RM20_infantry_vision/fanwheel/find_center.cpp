#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "algorithm/include/usr_math.h"
#include "fanwheel/fan_param.h"
/* @Des:this function is to find roi of fan center
 * @param:Mat src--raw image
 * @retval:find or not
 */
bool fan::find_roi_of_R()
{
    float length = static_cast<float>(pointDistance(m_flow_strip.center,m_target_point));//get the distance of strip and armor
    Point2f p2p(m_flow_strip.center.x - m_target_point.x,
        m_flow_strip.center.y - m_target_point.y);//calculate the relative coordinate of flowstrip to targetpoint
    p2p = p2p / pointDistance(m_flow_strip.center, m_target_point);//单位化(the model is 1)
    m_center_ROI = RotatedRect(Point2f(m_flow_strip.center + p2p * length * 1.1f),
                 Size2f(length * 0.8f, length * 0.8f), -90);
    if(m_is_show_img)
    {
       ellipse(m_debug_img, m_center_ROI, CV_RGB(0, 255, 0), 1, 8);
    }
    return true;
}
/* @Des:this function is to find R
 * @param:Mat src--raw image
 * @retval:find or not
 */
bool fan::find_R_by_contour(const Mat &src)
{
    static Mat src_bin;
    vector<vector<Point>> center_R_contours;

    src_bin = src.clone();
    R_erode_dilate(src_bin);
    findContours(src_bin, center_R_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    for (auto &center_R_contour : center_R_contours)
    {
        if (!isValidCenterRContour(center_R_contour,m_center_ROI))
        {
            continue;
        }
        m_center_R = cv::minAreaRect(center_R_contour);
        float target_length = static_cast<float>(pointDistance(m_flow_strip.center,m_target_point));
        m_center_point = m_center_R.center;
        m_center_point.y += target_length/20.0f;//实际最小二乘得到的中心在R的下方
//        cout<<"center_x:"<<m_center_point.x<<endl;
//        cout<<"center_y:"<<m_center_point.y<<endl;
        if(m_is_show_img)
        {
           circle(m_debug_img, m_center_point, 2, Scalar(50, 50, 255), 2, 8);
//           namedWindow("R", WINDOW_NORMAL);
//           imshow("R", src_bin);
           //cout<<"m_armors.size() :"<<m_armors.size() <<endl;
           //cout<<"center_x:"<<m_center_point.x<<endl;
           //cout<<"center_y:"<<m_center_point.y<<endl;
        }
        return true;
    }
    return false;

}
/* @Des:this function is to find R
 * @param:Mat src--raw image
 * @retval:find or not
 */
bool fan::find_R_by_crosspoint()
{
    if(m_armors.size() > 2)
       m_center_point = get_circle_center2(m_target_armor.center, m_armors[1].center,m_armors[2].center);
    else
       m_center_point = get_circle_center1(m_target_armor.center, m_armors[1].center,m_flow_strip.center);
    float target_length =
        m_target_armor.size.height > m_target_armor.size.width ? m_target_armor.size.height : m_target_armor.size.width;
    m_center_point.x -= target_length/20.0f;
    m_center_point.y -= target_length/15.0f;//实际最小二乘得到的中心在R的下方
    if(m_is_show_img)
    {
       circle(m_debug_img, m_target_armor.center, 2, Scalar(255, 50, 50), 2, 8);
       circle(m_debug_img, m_armors[1].center, 2, Scalar(255, 50, 50), 2, 8);
       circle(m_debug_img, m_armors[2].center, 2, Scalar(255, 50, 50), 2, 8);
       circle(m_debug_img, m_center_point, 2, Scalar(50, 50, 255), 2, 8);
       cout<<"m_armors.size() :"<<m_armors.size() <<endl;
       cout<<"center_x:"<<m_center_point.x<<endl;
       cout<<"center_y:"<<m_center_point.y<<endl;
    }
}
