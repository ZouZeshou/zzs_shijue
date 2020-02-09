#ifndef FAN_MIDDLEWARE
#define FAN_MIDDLEWARE
#include <iostream>
#include "opencv2/opencv.hpp"
#include "math.h"
#include "main/global.h"


void armor_erode_dilate(Mat & src);
void flow_fan_erode_dilate(Mat & src);
void flow_strip_erode_dilate(Mat & src);
void R_erode_dilate(Mat &src) ;
bool isValidArmorContour(const vector<Point> &armor_contour) ;
bool isValidFlowStripFanContour(const vector<Point> &flow_strip_fan_contour) ;
bool isValidFlowStripContour(const vector<Point> &flow_strip_contour) ;
bool isValidCenterRContour(const vector<Point> &center_R_contour,RotatedRect center_ROI) ;

#endif
