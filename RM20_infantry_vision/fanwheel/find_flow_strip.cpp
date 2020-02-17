#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "algorithm/include/usr_math.h"
#include "fanwheel/fan_param.h"
/* @Des:this function is to find flow strip
 * @param:Mat src--raw image
 * @retval:find or not
 */
bool fan::find_flow_strip(const Mat &src)
{
    Mat src_bin;
    src_bin = src.clone();
    std::vector<vector<Point>> flow_strip_contours;
    m_flow_strips.clear();
    /************** delete the target armor for find flow strip *************/
    RotatedRect candidate_armor = m_target_armor;
    Point2f vertices[4];
    candidate_armor.points(vertices);   //计算矩形的4个顶点
    for (int i = 0; i < 4; i++)
    {
        line(src_bin, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 0), 20);
    }
    /************* find the flow strip ****************************************/
    flow_strip_erode_dilate(src_bin);//图像膨胀，防止图像断开并更方便寻找
    findContours(src_bin, flow_strip_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    for (auto candidate_flow_strip_fan : m_flow_strip_fans)
    {
        for (auto &flow_strip_contour : flow_strip_contours)
        {
            if (!isValidFlowStripContour(flow_strip_contour))
            {
                continue;
            }
            std::vector<cv::Point2f> intersection;
            RotatedRect cur_rect = minAreaRect(flow_strip_contour);
            if (rotatedRectangleIntersection(cur_rect, candidate_flow_strip_fan, intersection) == 0)
            {
                continue;
            }
            else if (contourArea(intersection) > fan_param.FLOW_STRIP_CONTOUR_INTERSETION_AREA_MIN)
            {
                m_flow_strips.push_back(minAreaRect(flow_strip_contour));
                //   cout << "intersection: " << contourArea(intersection) << '\t' << cur_rect.center << endl;
            }
            else
            {
                continue;
            }
        }
    }
    if(m_is_show_img)
    {
        for(auto &flow_strip:m_flow_strips)
            ellipse(m_debug_img, flow_strip, CV_RGB(255,255 , 255), 1, 8);
//        namedWindow("flow_strip", WINDOW_NORMAL);
//        imshow("flow_strip", src_bin);
    }
    if (m_flow_strips.empty()|| m_flow_strips.size() > 1)
        return false;
    else
    {
        m_flow_strip = m_flow_strips[0];
        return true;
    }
}
