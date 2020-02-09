#ifndef USR_MATH_H
#define USR_MATH_H
#include "main/global.h"
#define ANGLE_TO_RAD 0.0174532f
#define RAD_TO_ANGLE 57.295779f
#define PI 3.1415926f
float loop_float_constrain(float input,float min_value,float max_value);
double pointDistance(Point point_1, Point point_2) ;
bool find_crossPoint(Point2f p1, Point2f p2, Point2f p3, Point2f p4, Point2f &crossPoint);
Point2f get_circle_center1(Point2f A, Point2f B, Point2f C);
Point2f get_circle_center2(Point2f A, Point2f B, Point2f C);
#endif // USR_MATH_H
