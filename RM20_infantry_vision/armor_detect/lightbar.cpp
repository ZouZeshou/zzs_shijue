#include "armor_detect/armor_detect.h"
#include "armor_detect/armor_param.h"
#include "algorithm/include/usr_math.h"
/* @Des:this function is to get the  lightbar from binary image
 * @param:src--the image to show ellipse
 * @retval: binary image
 */
bool armor_detect::get_lightbar(Mat src)
{
    /* find the contours and find the useful rects, then save them */
    vector<vector<Point>>   contours;
    Mat src_bin;
    src_bin = src.clone();
    contours.clear();
    m_lightbars.clear();
    lightbar_erode_dilate(src_bin);//erode and dilate
    findContours(src_bin, contours, RETR_EXTERNAL , CHAIN_APPROX_NONE);
    for(auto &contour:contours)
    {
        if (!isValidLightbarContour(contour))
        {
            continue;
        }
        m_lightbars.push_back(fitEllipse(contour));
    }
    if(m_is_show_img)
    {
        for(auto &lightbar:m_lightbars)
            ellipse(m_debug_img, lightbar, CV_RGB(255, 0, 0), 2, 8);
        namedWindow("lightbar", WINDOW_NORMAL);
        imshow("lightbar", src_bin);
    }
    if(m_lightbars.size() < 2)
        return false;
    else
        return true;
}
