#include "fanwheel/fan_middleware.h"
#include "fanwheel/fan_param.h"
/* @Des:this function is erode and dilate the image
 * @param:Mat src--raw image
 * @retval:
 */
void armor_erode_dilate(Mat & src)
{
	Mat element_dilate_1 = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat element_erode_1 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_dilate_2 = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat element_erode_2 = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat element_dilate_3 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_erode_3 = getStructuringElement(MORPH_RECT, Size(1, 1));

    erode(src, src, element_erode_3);
	dilate(src, src, element_dilate_1);
    erode(src, src, element_erode_1);
}
void flow_fan_erode_dilate(Mat & src)
{
	Mat element_dilate_1 = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat element_erode_1 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_dilate_2 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_erode_2 = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat element_dilate_3 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_erode_3 = getStructuringElement(MORPH_RECT, Size(1, 1));

    dilate(src, src, element_dilate_1);
    erode(src, src, element_erode_1);
}
void flow_strip_erode_dilate(Mat & src)
{
	Mat element_dilate_1 = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat element_erode_1 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_dilate_2 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_erode_2 = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat element_dilate_3 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_erode_3 = getStructuringElement(MORPH_RECT, Size(1, 1));

    dilate(src, src, element_dilate_1);
    erode(src, src, element_erode_1);
}
void R_erode_dilate(Mat &src) 
{
	Mat element_dilate_1 = getStructuringElement(MORPH_RECT, Size(4, 4));
	Mat element_erode_1 = getStructuringElement(MORPH_RECT, Size(2, 1));
	Mat element_dilate_2 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_erode_2 = getStructuringElement(MORPH_RECT, Size(4, 4));

	erode(src, src, element_erode_1);
	dilate(src, src, element_dilate_1);
}
/* @Des:this function is judge the contour is a armor
 * @param:const vector<Point> &armor_contour -- contour
 * @retval:
 */
bool isValidArmorContour(const vector<Point> &armor_contour) 
{
	double cur_contour_area = contourArea(armor_contour);
    if (cur_contour_area > fan_param.ARMOR_CONTOUR_AREA_MAX ||
        cur_contour_area < fan_param.ARMOR_CONTOUR_AREA_MIN)
    {
        //cout<<cur_contour_area<<" "<<energy_fan_param_.CONTOUR_AREA_MIN<<" "<<energy_fan_param_.CONTOUR_AREA_MAX<<endl;
        //cout<<"area fail."<<endl;
        return false;
        //选区面积大小不合适
    }
	RotatedRect cur_rect = minAreaRect(armor_contour);//得到包覆轮廓的最小斜矩形
	Size2f cur_size = cur_rect.size;
	float length = cur_size.height > cur_size.width ? cur_size.height : cur_size.width;//将矩形的长边设置为长
	float width = cur_size.height < cur_size.width ? cur_size.height : cur_size.width;//将矩形的短边设置为宽
    if (length < fan_param.ARMOR_CONTOUR_LENGTH_MIN || width < fan_param.ARMOR_CONTOUR_WIDTH_MIN ||
        length > fan_param.ARMOR_CONTOUR_LENGTH_MAX || width > fan_param.ARMOR_CONTOUR_WIDTH_MAX)
    {
        //cout<<"length width fail."<<endl;
//        cout << "length: " << length << '\t' << "width: " << width << '\t' << cur_rect.center << endl;
        return false;
        //矩形边长不合适
    }

	float length_width_ratio = length / width;//计算矩形长宽比
    if (length_width_ratio > fan_param.ARMOR_CONTOUR_HW_RATIO_MAX/100.0f ||
        length_width_ratio < fan_param.ARMOR_CONTOUR_HW_RATIO_MIN/100.0f)
    {
        //cout<<"length width ratio fail."<<endl;
//        cout << "HW: " << length_width_ratio << '\t' << cur_rect.center << endl;
        return false;
        //长宽比不合适
    }
    if (cur_contour_area / cur_size.area() < fan_param.ARMOR_CONTOUR_AREA_RATIO_MIN/100.0f)
    {
        //        cout << "area ratio: " << cur_contour_area / cur_size.area() << '\t' << cur_rect.center << endl;
        return false;//轮廓对矩形的面积占有率不合适
    }
	return true;
}
/* @Des:this function is to judge if the contour is the flow fan
 * @param:Mat src--raw image
 * @retval:find or not
 */
bool isValidFlowStripFanContour(const vector<Point> &flow_strip_fan_contour) 
{
	double cur_contour_area = contourArea(flow_strip_fan_contour);
	if (cur_contour_area > fan_param.FLOW_STRIP_FAN_CONTOUR_AREA_MAX ||
		cur_contour_area < fan_param.FLOW_STRIP_FAN_CONTOUR_AREA_MIN) 
	{
		//cout<<"area fail."<<endl;
//        cout << "area: " << cur_contour_area << '\t' << endl;
		return false;
		//选区面积大小不合适
	}
	//    cout << "area: " << cur_contour_area << endl;
	RotatedRect cur_rect = minAreaRect(flow_strip_fan_contour);
	Size2f cur_size = cur_rect.size;
	float length = cur_size.height > cur_size.width ? cur_size.height : cur_size.width;//将矩形的长边设置为长
	float width = cur_size.height < cur_size.width ? cur_size.height : cur_size.width;//将矩形的短边设置为宽
	if (length < fan_param.FLOW_STRIP_FAN_CONTOUR_LENGTH_MIN
		|| width < fan_param.FLOW_STRIP_FAN_CONTOUR_WIDTH_MIN
		|| length > fan_param.FLOW_STRIP_FAN_CONTOUR_LENGTH_MAX
		|| width > fan_param.FLOW_STRIP_FAN_CONTOUR_WIDTH_MAX) 
	{
		//        cout<<"length width fail."<<endl;
		//        cout << "length: " << length << '\t' << "width: " << width << '\t' << cur_rect.center << endl;
		return false;
		//矩形边长不合适
	}
	float length_width_ratio = length / width;//计算矩形长宽比
    if (length_width_ratio > fan_param.FLOW_STRIP_FAN_CONTOUR_HW_RATIO_MAX /100.0f||
        length_width_ratio < fan_param.FLOW_STRIP_FAN_CONTOUR_HW_RATIO_MIN/100.0f)
	{
		//cout<<"length width ratio fail."<<endl;
//        cout << "HW: " << length_width_ratio << '\t' << cur_rect.center << endl;
		return false;
		//长宽比不合适
	}
	//    cout << cur_contour_area / cur_size.area() << endl;
    if (cur_contour_area / cur_size.area() < fan_param.FLOW_STRIP_FAN_CONTOUR_AREA_RATIO_MIN/100.0f
        || cur_contour_area / cur_size.area() > fan_param.FLOW_STRIP_FAN_CONTOUR_AREA_RATIO_MAX/100.0f)
	{
		//        cout << "area ratio: " << cur_contour_area / cur_size.area() << '\t' << cur_rect.center << endl;
		return false;//    轮廓对矩形的面积占有率不合适
	}
	return true;
}
/* @Des:this function is to judge if the contour is the flow strip
 * @param:Mat src--raw image
 * @retval:find or not
 */
bool isValidFlowStripContour(const vector<Point> &flow_strip_contour) 
{
	double cur_contour_area = contourArea(flow_strip_contour);
	if (cur_contour_area > fan_param.FLOW_STRIP_CONTOUR_AREA_MAX ||
		cur_contour_area < fan_param.FLOW_STRIP_CONTOUR_AREA_MIN) {
		//        cout<<"area fail."<<endl;
		return false;
		//选区面积大小不合适
	}
	RotatedRect cur_rect = minAreaRect(flow_strip_contour);
	Size2f cur_size = cur_rect.size;
	float length = cur_size.height > cur_size.width ? cur_size.height : cur_size.width;//将矩形的长边设置为长
	float width = cur_size.height < cur_size.width ? cur_size.height : cur_size.width;//将矩形的短边设置为宽
	if (length < fan_param.FLOW_STRIP_CONTOUR_LENGTH_MIN ||
		width < fan_param.FLOW_STRIP_CONTOUR_WIDTH_MIN ||
		length > fan_param.FLOW_STRIP_CONTOUR_LENGTH_MAX ||
		width > fan_param.FLOW_STRIP_CONTOUR_WIDTH_MAX) 
	{
		//        cout<<"length width fail."<<endl;
		//        if(length>30)cout << "length: " << length << '\t' << "width: " << width << '\t' << cur_rect.center << endl;
		return false;
		//矩形边长不合适
	}
	//    cout << length << '\t' << width << endl;
	float length_width_ratio = length / width;//计算矩形长宽比
    if (length_width_ratio > fan_param.FLOW_STRIP_CONTOUR_HW_RATIO_MAX/100.0f ||
        length_width_ratio < fan_param.FLOW_STRIP_CONTOUR_HW_RATIO_MIN/100.0f)
	{
		//        cout<<"hw fail."<<endl;
		//        if(length_width_ratio>3.5)cout << "HW: " << length_width_ratio << '\t' << cur_rect.center << endl;
		return false;
		//长宽比不合适
	}
    if (cur_contour_area / cur_size.area() < fan_param.FLOW_STRIP_CONTOUR_AREA_RATIO_MIN/100.0f)
	{
		//        if(cur_contour_area / cur_size.area()>0.5)cout << "area ratio: " << cur_contour_area / cur_size.area() << '\t' << cur_rect.center << endl;
		return false;//轮廓对矩形的面积占有率不合适
	}
	return true;
}
/* @Des:this function is to judge if the contour is R
 * @param:Mat src--raw image
 * @retval:find or not
 */
bool isValidCenterRContour(const vector<Point> &center_R_contour,RotatedRect center_ROI)
{
	double cur_contour_area = contourArea(center_R_contour);
	if (cur_contour_area > fan_param.CENTER_R_CONTOUR_AREA_MAX ||
		cur_contour_area < fan_param.CENTER_R_CONTOUR_AREA_MIN) 
	{
		//cout<<cur_contour_area<<" "<<energy_fan_param_.CONTOUR_AREA_MIN<<" "<<energy_fan_param_.CONTOUR_AREA_MAX<<endl;
		//cout<<"area fail."<<endl;
		return false;
		//选区面积大小不合适
	}
	RotatedRect cur_rect = minAreaRect(center_R_contour);
	Size2f cur_size = cur_rect.size;
	float length = cur_size.height > cur_size.width ? cur_size.height : cur_size.width;//将矩形的长边设置为长
	float width = cur_size.height < cur_size.width ? cur_size.height : cur_size.width;//将矩形的短边设置为宽
	if (length < fan_param.CENTER_R_CONTOUR_LENGTH_MIN || width < fan_param.CENTER_R_CONTOUR_WIDTH_MIN
		|| length > fan_param.CENTER_R_CONTOUR_LENGTH_MAX ||
		width > fan_param.CENTER_R_CONTOUR_WIDTH_MAX) 
	{
		//cout<<"length width fail."<<endl;
//        cout << "length: " << length << '\t' << "width: " << width << '\t' << cur_rect.center << endl;
		return false;
		//矩形边长不合适
	}
	float length_width_ratio = length / width;//计算矩形长宽比
    if (length_width_ratio > fan_param.CENTER_R_CONTOUR_HW_RATIO_MAX/100.0f ||
        length_width_ratio < fan_param.CENTER_R_CONTOUR_HW_RATIO_MIN/100.0f)
	{
		//cout<<"length width ratio fail."<<endl;
//        cout << "HW: " << length_width_ratio << '\t' << cur_rect.center << endl;
		return false;
		//长宽比不合适
	}
    if (cur_contour_area / cur_size.area() < fan_param.CENTER_R_CONTOUR_AREA_RATIO_MIN/100.0f)
	{
		//        cout << "area ratio: " << cur_contour_area / cur_size.area() << '\t' << cur_rect.center << endl;
		return false;//轮廓对矩形的面积占有率不合适
	}
    vector<Point2f> intersection;
	if (rotatedRectangleIntersection(cur_rect, center_ROI, intersection) == 0) 
	{
		return false;
	}
	else if (contourArea(intersection) < fan_param.CENTER_R_CONTOUR_INTERSETION_AREA_MIN) 
	{
		//        cout << "R intersection: " << contourArea(intersection) << '\t' << cur_rect.center << endl;
		return false;
	}
	return true;
}

