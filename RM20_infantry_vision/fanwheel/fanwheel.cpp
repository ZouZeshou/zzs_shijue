#include "fanwheel/fanwheel.hpp"
#include "main/global.h"
void trackbar_callback(int, void*){};
Fan_Shooter::Fan_Shooter(){
}
/* this function is main control function*/
void Fan_Shooter::get_fanwheel_center(Mat &src, bool is_show_img, uint8_t fanwheel_dir, color_ENUM color)
{
    m_is_show_img = is_show_img;
    seed_points.clear();
    V_PairDist_Armors.clear();
    FirstFilter_Armors.clear();
    distance.clear();
    final_target = Point2f(0.0f, 0.0f);

    //find roi
    roi_rect.x = 240;
    roi_rect.y = 0;
    roi_rect.width = 960;
    roi_rect.height = 840;
    roi_img = src(roi_rect);
    resize(roi_img,roi_img,Size(roi_img.cols*0.5, roi_img.rows*0.5));

    roi_img.copyTo(binary);
    get_binary_img(color,binary);
    binary.copyTo(center_img);
    get_seed_point(binary);
    do_floodfill();
    get_firstfilter_armor();
    deal_muti_armor();
    get_center();

    if(m_is_show_img)
    {
        namedWindow("roi",WINDOW_NORMAL);
        imshow("roi",roi_img);
    }
}
void Fan_Shooter::get_binary_img(color_ENUM color ,Mat& img)
{
    static int h_min = 0;
    static int h_max = 50;
    static int s_min = 65;
    static int s_max = 255;
    static int v_min = 20;
    static int v_max = 255;
    if(!m_is_show_img)
    {
        if(color == RED)
        {
            h_min = 0;
            h_max = 50;
            s_min = 65;//200
            s_max = 255;
            v_min = 20;//102
            v_max = 255;

        }
        else if(color == BLUE)
        {
            h_min = 75;
            h_max = 110;
            s_min = 78;
            s_max = 255;
            v_min = 15;//26
            v_max = 255;
        }
    }
    cvtColor(img, img, CV_BGR2HSV);
    inRange(img, Scalar(h_min,s_min,v_min), Scalar(h_max,s_max,v_max),img);
    threshold(img, img, 20, 255, THRESH_BINARY);

    morphologyEx(img, img, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(2,2)));
    dilate(img, img, getStructuringElement(MORPH_RECT, Size(4,4)));
    if(m_is_show_img)
    {
        namedWindow("binary",WINDOW_NORMAL);
        imshow("binary",img);
        createTrackbar("H_Min","binary",&h_min,255,trackbar_callback);
        createTrackbar("H_Max","binary",&h_max,255,trackbar_callback);
        createTrackbar("S_Min","binary",&s_min,255,trackbar_callback);
        createTrackbar("S_Max","binary",&s_max,255,trackbar_callback);
        createTrackbar("V_Min","binary",&v_min,255,trackbar_callback);
        createTrackbar("V_Max","binary",&v_max,255,trackbar_callback);
    }

}
void Fan_Shooter::get_seed_point(Mat& img)
{
    findContours(img, contour, RETR_CCOMP , CHAIN_APPROX_NONE);
    for( size_t ii = 0; ii < contour.size() ; ++ii )
    {
        if((contour[ii].size() > 5))
            Temp_Ellipse = fitEllipse(contour[ii]);
        float wh_ratio = Temp_Ellipse.size.height / Temp_Ellipse.size.width;
        //ellipse(roi_img, Temp_Ellipse, CV_RGB(0, 0, 255), 2, 8);//show ellipse

        if(Temp_Ellipse.size.area() < 1000 && \
                Temp_Ellipse.size.area() > 300 && \
                wh_ratio < 9.0f && \
                wh_ratio > 1.2f)
        {
//            if(Temp_Ellipse.center.x < 850 && Temp_Ellipse.center.x > 150 &&\
//               Temp_Ellipse.center.y < 750 && Temp_Ellipse.center.y > 150)
            seed_points.push_back(Temp_Ellipse.center);
            //FirstFilter_Armors.push_back(Temp_Ellipse);
           ellipse(roi_img, Temp_Ellipse, CV_RGB(0, 255, 0), 2, 8);//show ellipse
//                cout << "area" << Temp_Ellipse.size.area() << "    wh_ratio   "  << wh_ratio <<endl;
        }
    }
}
void Fan_Shooter::do_floodfill(void)
{
    binary.copyTo(flood_img);
    flood_img.copyTo(mask_img);
    mask_img.rows = mask_img.rows + 2;
    mask_img.cols = mask_img.cols + 2;
    /* I do not know why the canny does not have any difference when i change the 30, 90 to other values */
    Canny(mask_img, canny_img, 3, 9);
    if(m_is_show_img)
        imshow("canny", canny_img);
    for(size_t i = 0; i < seed_points.size() ; i++)
    {
        if(seed_points[i].x > 0 && seed_points[i].y > 0)
            floodFill(flood_img, canny_img, seed_points[i], Scalar(255), 0, Scalar(20), Scalar(20), 4);
            //floodFill(flood_img, canny_img, seed_points[i], Scalar(255), 0,FLOODFILL_FIXED_RANGE);
    }

    binary = flood_img - binary;
    if(m_is_show_img)
        imshow("floodFill", binary);
}
void Fan_Shooter::get_firstfilter_armor(void)
{
    findContours(binary, flood_contour,  RETR_CCOMP, CHAIN_APPROX_NONE);
    for( size_t ii = 0; ii < flood_contour.size() ; ++ii )
    {
        if((flood_contour[ii].size() > 5))
            Temp_Ellipse = fitEllipse(flood_contour[ii]);
        float wh_ratio = Temp_Ellipse.size.height / Temp_Ellipse.size.width;
        //ellipse(roi_img, Temp_Ellipse, CV_RGB(0, 0, 255), 2, 8);//show ellipse

        if(Temp_Ellipse.size.area() < 800 && \
                Temp_Ellipse.size.area() > 40
               /* wh_ratio < 6.0f && \
                wh_ratio > 1.7f*/){
            ellipse(roi_img, Temp_Ellipse, CV_RGB(0, 255, 0), 2, 8);//show ellipse
            //circle(src, Temp_Ellipse.center, 3, Scalar(0, 255, 0), 2);
            // area
            // 2300-2600   3400-3600
            // wh_ratio
            // 1.75-2.05    4.2-4.5
            //cout << "area" << Temp_Ellipse.size.area() << "    wh_ratio   "  << wh_ratio <<endl;
            FirstFilter_Armors.push_back(Temp_Ellipse);
        }
    }
}
void Fan_Shooter::deal_muti_armor(void)
{
    if(FirstFilter_Armors.size() > 0)
    {
        if(FirstFilter_Armors.size() == 1)
        {
            Final_armor = FirstFilter_Armors[0];
            distance.clear();
        }
        else
        {
            static size_t last_armor_size = 0;
            for(size_t i = 0; i < FirstFilter_Armors.size(); i++)
            {
                distance.clear();
                for(size_t j = 0; j < FirstFilter_Armors.size(); j++)
                {
                    Point2f p1 = FirstFilter_Armors[i].center;
                    Point2f p2 = FirstFilter_Armors[j].center;
                    if(i != j &&  (FirstFilter_Armors[i].center.x != FirstFilter_Armors[j].center.x))
                    {
                        distance.push_back(get_points_dist(p1, p2));
                    }
                }
                sort(distance.begin(), distance.end(), less<float>());
//                cout << " " << distance[0] << " " << distance[1] << " " << distance[2] << " " << distance[3] <<endl;
                if(!distance.empty())
                    s_PairDist_Armors.distance = distance[0];
                s_PairDist_Armors.FirstFilter_Armors = FirstFilter_Armors[i];
                V_PairDist_Armors.push_back(s_PairDist_Armors);
            }
            sort(V_PairDist_Armors.begin(), V_PairDist_Armors.end(), dist_sort);
//                cout << PairDist_Armors[0].first << endl;
            /* small value 150 - 165     big value 245-260 */
            if(50 < V_PairDist_Armors[0].distance && 200 > V_PairDist_Armors[0].distance)
                Final_armor = V_PairDist_Armors[0].FirstFilter_Armors;
            else
                Final_armor.center = Point2f(0, 0);
            ellipse(roi_img, Final_armor, CV_RGB(0, 255, 0), 2, 8);//show ellipse*/

            /* to limit the shoot influence to the target */
//            if(FirstFilter_Armors.size() <= last_armor_size){
//                if(fabs(Final_armor.center.x - last_final_armor.center.x) > 20)
//                    Final_armor = last_final_armor;
//            }
//            last_final_armor = Final_armor;
//            last_armor_size = FirstFilter_Armors.size();
        }
        //circle(roi_img, Final_armor.center, 3, Scalar(0, 0, 255), 2);
    }
}
void Fan_Shooter::get_center(void)
{
    static vector<Point2f> center;
    static Point2f         final_center;
    float                  radiums = 0.0f;
    findContours(center_img, center_contour, RETR_CCOMP , CHAIN_APPROX_NONE);
//        cout << "size "<< center_contour.size() << endl;

    for( size_t ii = 0; ii < center_contour.size() ; ++ii )
    {
        if((center_contour[ii].size() > 5)){
            Center_Ellipse = fitEllipse(center_contour[ii]);
            float WH_ratio = Center_Ellipse.size.height / Center_Ellipse.size.width;

            if( (Center_Ellipse.size.area() < 250) && \
                (Center_Ellipse.size.area() > 100) && \
                (WH_ratio < 2.0f) &&\
                (get_points_dist(Final_armor.center, Center_Ellipse.center) < 130.0f))
            {
//                    cout << "area" << Center_Ellipse.size.area() << "    wh_ratio   "  << WH_ratio <<endl;

                ellipse(roi_img, Center_Ellipse, CV_RGB(255, 0, 0), 2, 8);//show ellipse
                circle(roi_img, Center_Ellipse.center, 3, Scalar(0, 255, 0), 2);
                //cout << "area" << Center_Ellipse.size.area() << "    wh_ratio   "  << WH_ratio <<endl;
                // area 1400 - 1600
                if(Final_armor.center.x != 0)
                {
                    Draw_FanWheel_Path(roi_img, Center_Ellipse.center,  /*55.0f + */get_points_dist(Final_armor.center, Center_Ellipse.center));
                    Find_Target_Center(Center_Ellipse.center, Final_armor.center, 0.45f, final_target, 0);
//                    cout << final_target << endl;
                    circle(roi_img, final_target, 3, Scalar(50, 50, 255), 2, 8);
                }
                static Point2f last_center;
                if(last_center.x == final_target.x)
                {
                    //final_target = {0,0};
                    continue;
                }
                last_center = final_target;
            }
        }
    }
//    static float last_centerx = 0.0f;
//    static uint16_t  cnt = 0;
//    if(last_centerx == final_target.x )
//        cnt++;
//    if(cnt>100)
//        final_target = {0.0, 0.0};
    if(m_is_show_img)
    {
        //imshow("src", src);
        imshow("roi", roi_img);
    }
}
float Fan_Shooter::get_points_dist(Point2f p1, Point2f p2)
{
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

void Fan_Shooter::Find_intersection(Point2f point_A, Point2f point_B, Point2f point_C, vector<Point2f> &intersection){
    static float    k1 = 0.0f, \
                    k2 = 0.0f, \
                    t1 = 0.0f, \
                    t2 = 0.0f, \
                    c1 = 0.0f, \
                    c2 = 0.0f;
    static Point2f result;
    t1 = (point_B.x - point_A.x)/(point_B.y - point_A.y);
    t2 = (point_B.x - point_C.x)/(point_B.y - point_C.y);
    k1 = 1 / t1;
    k2 = 1 / t2;
    c1 = (point_A.x + point_B.x)/(2*k1) + (point_A.y + point_B.y) / 2.0f;
    c2 = (point_C.x + point_B.x)/(2*k2) + (point_C.y + point_B.y) / 2.0f;
    result.x = -(c1 - c2) / (t2 - t1);
    result.y = (c1*t2 - c2*t1) / (t2 - t1);
    if(result.x > 0 && result.y > 0)
        intersection.push_back(result);
}

void Fan_Shooter::Draw_FanWheel_Path(Mat &img, Point2f Center, float radium){
    circle(img, , radium, Scalar(50, 255, 150), 2, 8);
    circle(img, Center, 2, Scalar(50, 255, 150), 2, 8);
}

void Fan_Shooter::Find_Target_Center(Point2f center, Point2f pos_now, float angle_increase, Point2f& target_center, uint8_t dir){
    static float angle_now = 0.0f, angle_last = 0.0f, angle_final = 0.0f, \
                 _angle_first = 0.0f, _angle_second = 0.0f, angle_diff;
    float radium = 0.0f;
    static int   round = 0;
    if(pos_now.y != center.y){
        angle_now = std::atan2((pos_now.y - center.y), (pos_now.x - center.x));
        angle_now *= 180.0f/3.14f;
        if(angle_last - angle_now < -300.0f){
            round--;
        }
        else if(angle_last - angle_now > 300.0f){
            round++;
        }
        angle_final = round * 360.0f + angle_now;
        angle_final = angle_final - (int)angle_final/360 * 360.0f;
        angle_last = angle_now;
    }
//        cout << "angle " << angle_final << endl;
    radium = get_points_dist(center, pos_now);
    angle_now *= 3.14/180.0f;
    if(dir == 2){
        target_center.x = radium * cosf(angle_now + angle_increase) + center.x;
        target_center.y = radium * sinf(angle_now + angle_increase) + center.y;
    }
    else if(dir == 1){
        target_center.x = radium * cosf(angle_now - angle_increase) + center.x;
        target_center.y = radium * sinf(angle_now - angle_increase) + center.y;
    }
    else if(dir == 0){
        target_center.x = radium * cosf(angle_now) + center.x;
        target_center.y = radium * sinf(angle_now) + center.y;
    }
//    cout << "angle_diff" << angle_diff <<endl;
//    cout << is_got_dir << "dir is  " << direction << endl;
}
