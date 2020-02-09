
#ifndef FANWHEEL_HPP
#define FANWHEEL_HPP

#include <iostream>
#include "opencv2/opencv.hpp"
#include "math.h"
#include "main/global.h"

using namespace std;
using namespace cv;
typedef struct
{
    RotatedRect           FirstFilter_Armors;
    float                 distance;
}PairDist_Armors;
static inline bool dist_sort(PairDist_Armors  a1,PairDist_Armors a2)
{
    return a1.distance > a2.distance;
};

class Fan_Shooter
{
public:
                             Fan_Shooter();
   void                      get_fanwheel_center(Mat &src, bool is_show_img, uint8_t fanwheel_dir, color_ENUM color);
private:
   float                     get_points_dist(Point2f p1, Point2f p2);
   void                      Find_intersection(Point2f point_A, Point2f point_B, Point2f point_C, vector<Point2f> &intersection);
   void                      Draw_FanWheel_Path(Mat &img, Point2f Center, float radium);
   void                      Find_Target_Center(Point2f center, Point2f pos_now, float angle_increase, Point2f& target_center, uint8_t dir);
   void                      get_binary_img(color_ENUM color,Mat& img);
   void                      get_seed_point(Mat& img);
   void                      do_floodfill(void);
   void                      get_firstfilter_armor(void);
   void                      deal_muti_armor(void);
   void                      get_center(void);
public:
   vector<vector<Point>>           contour, flood_contour, center_contour;
   Mat                             binary, flood_img,  center_img, canny_img, mask_img;
   RotatedRect                     Temp_Ellipse, last_final_armor, Final_armor, Center_Ellipse;
   vector <Point2f>                seed_points;
   Point2f                         final_target;
   int                             adjust_value;
   Rect                            roi_rect;
   Mat                             roi_img;
   bool                            m_is_show_img;
   PairDist_Armors                 s_PairDist_Armors;
   vector<PairDist_Armors>         V_PairDist_Armors;
   vector<RotatedRect>             FirstFilter_Armors;
   vector<float>                   distance;
};
void trackbar_callback(int, void*);

#endif // FANWHEEL_HPP
