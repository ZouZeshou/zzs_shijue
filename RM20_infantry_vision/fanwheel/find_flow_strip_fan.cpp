#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "algorithm/include/usr_math.h"
#include "fanwheel/fan_param.h"
/* @Des:this function is to find the flow fan
 * @param:Mat src--raw image
 * @retval:find or not
 */
bool fan::find_flow_fans(const Mat &src)
{
    static Mat src_bin;
    vector<vector<Point>> flow_strip_fan_contours;
    vector<RotatedRect>   candidate_flow_strip_fans;

    m_flow_strip_fans.clear();
    src_bin = src.clone();
    flow_fan_erode_dilate(src_bin);//图像膨胀，防止图像断开并更方便寻找
    findContours(src_bin, flow_strip_fan_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//        for(auto &armor:flow_strip_fan_contours)
//            ellipse(m_debug_img, minAreaRect(armor), CV_RGB(255, 0, 0), 2, 8);
//        cout<<"armor_contours.size()"<<flow_strip_fan_contours.size()<<endl;
    for (auto &flow_strip_fan_contour : flow_strip_fan_contours)
    {
        if (!isValidFlowStripFanContour(flow_strip_fan_contour))
        {
            continue;
        }
        m_flow_strip_fans.push_back(minAreaRect(flow_strip_fan_contour));
    }
    //cout<<"m_flow_strip_fans.size()"<<m_flow_strip_fans.size()<<endl;
    if(m_is_show_img)
    {
        for(auto &flow_fan:m_flow_strip_fans)
            ellipse(m_debug_img, flow_fan, CV_RGB(0, 0,255 ), 1, 8);
        namedWindow("flow_fan", WINDOW_NORMAL);
        imshow("flow_fan", src_bin);
    }
    if (m_flow_strip_fans.empty())
        return false;
    return true;
}
/* @Des:this function is to find armor in flow fan
 * @param:Mat src--raw image
 * @retval:find or not
 */
bool fan::find_armor_in_flow_area()
{
    static size_t target_armor_num;
    m_target_armors.clear();
    m_last_target_point = m_target_point;
    for (auto &candidate_flow_strip_fan : m_flow_strip_fans)
    {
        for (size_t i = 0; i < m_armors.size(); ++i)
        {
            vector<Point2f> intersection;
            if (rotatedRectangleIntersection(m_armors.at(i), candidate_flow_strip_fan, intersection) == 0)
                continue;//返回0表示没有重合面积
            double cur_contour_area = contourArea(intersection);
            //        cout<<cur_contour_area<<endl;
            if (cur_contour_area > fan_param.ARMOR_INTERSETION_AREA_MIN)
            {
                m_target_armors.push_back(m_armors.at(i));
                target_armor_num = i;
            }
        }
    }
    if(m_armors.size() > 1 && target_armor_num != 0)
        swap(m_armors[target_armor_num],m_armors[0]);
    if(m_target_armors.size()==1)
    {
        m_target_armor = m_target_armors[0];
        m_target_point = m_target_armor.center;
        if(m_is_show_img)
        {
            ellipse(m_debug_img, m_target_armor, CV_RGB(255, 0, 0), 1, 8);
            circle(m_debug_img, m_target_point,3, CV_RGB(255, 0, 0), 1, 8);
            //cout<<"armor_x:"<<m_target_point.x<<endl;
            //cout<<"armor_y:"<<m_target_point.y<<endl;
        }
        return true;
    }
    else
        return false;
}
