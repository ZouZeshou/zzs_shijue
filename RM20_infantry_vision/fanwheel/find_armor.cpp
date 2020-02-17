#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "algorithm/include/usr_math.h"
#include "fanwheel/fan_param.h"
/* @Des:this function is to find the armor
 * @param:Mat src--raw image
 * @retval:armor size
 */
int fan::find_armors(const Mat &src)
{
    Mat src_bin;
    src_bin = src.clone();
    vector<vector<Point>> armor_contours;
    vector<vector<Point>> armor_contours_external;//用总轮廓减去外轮廓，只保留内轮廓，除去流动条的影响。
    m_armors.clear();
    armor_erode_dilate(src_bin);
    findContours(src_bin, armor_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    findContours(src_bin, armor_contours_external, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    for (size_t i = 0; i < armor_contours_external.size(); i++)//去除外轮廓
    {
        uint64_t external_contour_size = armor_contours_external[i].size();
        double external_area = contourArea(armor_contours_external[i]);
        for (size_t j = 0; j < armor_contours.size(); j++)
        {
            uint64_t list_contour_size = armor_contours[j].size();
            double list_area = contourArea(armor_contours[j]);
            if (external_contour_size == list_contour_size && external_area==list_area)
            {
                swap(armor_contours[j], armor_contours[armor_contours.size() - 1]);//将外轮廓换至容器最后
                armor_contours.pop_back();//删除容器最后一项，即外轮廓
                break;
            }
        }
    }
//    for(auto &armor:armor_contours)
//        ellipse(m_debug_img, minAreaRect(armor), CV_RGB(255, 0, 0), 2, 8);
//    cout<<"armor_contours.size()"<<armor_contours.size()<<endl;
    for (auto &armor_contour : armor_contours)
    {
        if (!isValidArmorContour(armor_contour))
        {
            continue;
        }
        m_armors.push_back(minAreaRect(armor_contour));
    }
    if(m_is_show_img)
    {
        for(auto &armor:m_armors)
            ellipse(m_debug_img, armor, CV_RGB(0, 255, 0), 2, 8);
//        namedWindow("armor", WINDOW_NORMAL);
//        imshow("armor", src_bin);
    }
    return static_cast<int>(m_armors.size());
}
