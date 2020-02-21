#include "armor_detect/armor_detect.h"
#include "armor_detect/armor_param.h"
#include "algorithm/include/usr_math.h"
/* @Des:this function is erode and dilate the image
 * @param:Mat src--raw image
 * @retval:
 */
void lightbar_erode_dilate(Mat & src)
{
    erode(src, src, getStructuringElement(MORPH_RECT, Size(3,3)));
    morphologyEx(src, src, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(3,3)));
}
/* @Des:this function is judge the contour is a armor
 * @param:const vector<Point> &armor_contour -- contour
 * @retval:
 */
bool isValidLightbarContour(const vector<Point> &contour)
{
    double cur_contour_area = contourArea(contour);
    if (cur_contour_area > armor_param.LIGHTBAR_CONTOUR_AREA_MAX ||
        cur_contour_area < armor_param.LIGHTBAR_CONTOUR_AREA_MIN)
    {
        return false;
        //选区面积大小不合适
    }
    RotatedRect cur_rect = fitEllipse(contour);
    Size2f cur_size = cur_rect.size;
    float length = cur_size.height > cur_size.width ? cur_size.height : cur_size.width;//将矩形的长边设置为长
    float width = cur_size.height < cur_size.width ? cur_size.height : cur_size.width;//将矩形的短边设置为宽
    if (length < armor_param.LIGHTBAR_CONTOUR_LENGTH_MIN || width < armor_param.LIGHTBAR_CONTOUR_WIDTH_MIN ||
        length > armor_param.LIGHTBAR_CONTOUR_LENGTH_MAX || width > armor_param.LIGHTBAR_CONTOUR_WIDTH_MAX)
    {
        return false;
        //矩形边长不合适
    }
    if(contour.size() <armor_param.LIGHTBAR_CONTOUR_MIN_SIZE)
    {
        return false;//contour size is too few
    }
    float rectAngle = cur_rect.angle;//get the angle of rect
    rectAngle = loop_float_constrain(rectAngle,-90.0f,90.0f);
    if(rectAngle > armor_param.LIGHTBAR_CONTOUR_ANGLE_MAX||
       rectAngle < -armor_param.LIGHTBAR_CONTOUR_ANGLE_MAX)
    {
        return false;// the angle is not in range
    }
    float length_width_ratio = length / width;//计算矩形长宽比
    if (length_width_ratio > armor_param.LIGHTBAR_CONTOUR_HW_RATIO_MAX/100.0f ||
        length_width_ratio < armor_param.LIGHTBAR_CONTOUR_HW_RATIO_MIN/100.0f)
    {
        return false;
        //长宽比不合适
    }
    if (cur_contour_area / cur_size.area() < armor_param.LIGHTBAR_CONTOUR_AREA_RATIO_MIN/100.0f)
    {
        //        cout << "area ratio: " << cur_contour_area / cur_size.area() << '\t' << cur_rect.center << endl;
        return false;//轮廓对矩形的面积占有率不合适
    }
    return true;
}
