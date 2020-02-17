#include "algorithm/include/usr_math.h"
/****** this function is for contrain a value to a interval ***************/
float loop_float_constrain(float input,float min_value,float max_value)
{
    if(max_value < min_value)
    {
        return input;
    }
    if(input > max_value)
    {
        float len = max_value - min_value;
        while(input > max_value)
        {
            input -= len;
        }
    }
    else if(input < min_value)
    {
        float len = max_value - min_value;
        while(input < min_value)
        {
            input += len;
        }
    }
    return input;
}
/* @Des:this function is to get the distance of two point
 * @param:
 * @retval:
 */
double pointDistance(Point point_1, Point point_2)
{
    double distance = 0;
    distance = sqrt(pow(static_cast<double>(point_1.x - point_2.x), 2)
                    + pow(static_cast<double>(point_1.y - point_2.y), 2));
    return distance;
}
/* @Des:this function is to get the midpoint of two point
 * @param:
 * @retval:
 */
Point2f mid_point(Point2f point_1, Point2f point_2)
{
    Point2f mid_point;
    mid_point.x = (point_1.x + point_2.x)/2.0f;
    mid_point.y = (point_1.y + point_2.y)/2.0f;
    return mid_point;
}
/* @Des:this function is to get the absolute angle difference of two points
 * @param:
 * @retval:
 */
float abs_angle_diff(Point2f point_1, Point2f point_2)
{
    float angle;
    if(point_1.x - point_2.x != 0.0f )
    {
        angle = atan2f(point_2.y - point_1.y,point_2.x - point_1.x) * RAD_TO_ANGLE;
        angle = loop_float_constrain(angle,-90,90);
        return fabs(angle);
    }
    else
        return 0.0f;
}
/* @Des:this function is to find the intersection of two line(p1p2 and p3p4)
 * @param:
 * @retval:
 */
bool find_crossPoint(Point2f p1, Point2f p2, Point2f p3, Point2f p4, Point2f &crossPoint)
{
    //****************************************************************************************
    //  求二条直线的交点的公式
    //  有如下方程 (x-x1)/(y-y1) = (x2-x1)/(y2-y1) ==> a1*x+b1*y=c1
    //            (x-x3)/(y-y3) = (x4-x3)/(y4-y3) ==> a2*x+b2*y=c2
    //  则交点为
    //                x= | c1 b1|  / | a1 b1 |      y= | a1 c1| / | a1 b1 |
    //                   | c2 b2|  / | a2 b2 |         | a2 c2| / | a2 b2 |
    //
    //   a1= y2-y1
    //   b1= x1-x2
    //   c1= x1*y2-x2*y1
    //   a2= y4-y3
    //   b2= x3-x4
    //   c2= x3*y4-x4*y3

    float a1 = p2.y - p1.y;
    float b1 = p1.x - p2.x;
    float c1 = p1.x*p2.y - p2.x*p1.y;
    float a2 = p4.y - p3.y;
    float b2 = p3.x - p4.x;
    float c2 = p3.x*p4.y - p4.x*p3.y;
    float det= a1*b2 - a2*b1;

    if(det == 0.0f) return false;

    crossPoint.x = (c1*b2 - c2*b1)/det;
    crossPoint.y = (a1*c2 - a2*c1)/det;

    // Now this is cross point of lines
    // Do we need the cross Point of segments(need to judge x,y within 4 endpoints)
    // 是否要判断线段相交
    if((abs(crossPoint.x -(p1.x+p2.x)/2) <= abs(p2.x-p1.x)/2) &&
       (abs(crossPoint.y -(p1.y+p2.y)/2) <= abs(p2.y-p1.y)/2) &&
       (abs(crossPoint.x -(p3.x+p4.x)/2) <= abs(p4.x-p3.x)/2) &&
       (abs(crossPoint.y -(p3.y+p4.y)/2) <= abs(p4.y-p3.y)/2))
    {
        return true;
    }
    return false;
}
/* @Des:this function is to find the circle center if we know three points(A,B,C),
 *      A,B in circle and line AC through the circle center
 * @param:
 * @retval:the center point
 */
Point2f get_circle_center1(Point2f A, Point2f B, Point2f C)
{
    Point2f D,E,center;
    float k1,k2;
    D.x = (A.x + B.x)/2.0f;
    D.y = (A.y + B.y)/2.0f;//get the midpoint of line AB
    k1 = (A.y - B.y)/(A.x - B.x);// get the slope of AB
    k2 = -1.0f/k1;//get the slope of DE,DE is the perpendicular bisector of AB.
    E.x = D.x +  50.0f;//E is a point in line DE
    E.y = D.y + k2 * 50.0f;
    find_crossPoint(A,C,D,E,center);
    cout<<"D.x"<<D.x<<endl;
    cout<<"D.y"<<D.y<<endl;
    cout<<"E.x"<<E.x<<endl;
    cout<<"E.y"<<E.y<<endl;
    cout<<"K1 "<<k1<<endl;
    cout<<"k2 "<<k2<<endl;
    return center;
}
/* @Des:this function is to find the circle center if we know three points(A,B,C),
 *      A,B,C are all in the circle
 * @param:
 * @retval:the center point
 */
Point2f get_circle_center2(Point2f A, Point2f B, Point2f C)
{
    Point2f D,E,F,G,center;
    float k1,k2,k3,k4;
    D.x = (A.x + B.x)/2.0f;
    D.y = (A.y + B.y)/2.0f;//get the midpoint of line AB
    k1 = (A.y - B.y)/(A.x - B.x);// get the slope of AB
    k2 = -1.0f/k1;//get the slope of DE,DE is the perpendicular bisector of AB.
    E.x = D.x +  50.0f;//E is a point in line DE
    E.y = D.y + k2 * 50.0f;

    F.x = (A.x + C.x)/2.0f;
    F.y = (A.y + C.y)/2.0f;//get the midpoint of line AC
    k3 = (A.y - C.y)/(A.x - C.x);// get the slope of AC
    k4 = -1.0f/k3;//get the slope of FG,FG is the perpendicular bisector of AC.
    G.x = F.x +  50.0f;//G is a point in line FG
    G.y = F.y + k4 * 50.0f;

    find_crossPoint(D,E,F,G,center);
    return center;
}
