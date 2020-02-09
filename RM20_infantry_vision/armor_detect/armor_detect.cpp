/* this file is to deal with the picture from video, aim to find the lightbar clearly
the second part
*/
#include "armor_detect/armor_detect.h"
#include "algorithm/include/usr_math.h"
kalman1_state                     g_angle_projection_kf;
kalman1_state                     g_angle_cos_kf;
void gray1_bar_callback(int, void*){};


armor_detect::armor_detect(){
    m_targetinfo.TargetTrackerStatus = LoseTarget;
    kalman1_init(&g_angle_projection_kf, 0, 200, 5, 50);
};
armor_detect::~armor_detect(){};
/* @Des:this function is to get the target center of armor
 * @param:img--raw image
 * @retval:
 */
void armor_detect::get_armor_center(Mat img,color_ENUM color,GetLightBarMethod_ENUM method,bool is_show_image)
{
    m_roi_rect.x = 0;
    m_roi_rect.y = 0;
    m_roi_rect.width = 1440;//1440
    m_roi_rect.height = 480;//480
    m_roi_img = img(m_roi_rect);
    m_show_image = is_show_image;
    get_binary_img(m_roi_img,color,method,is_show_image);/*true--debug*/
    get_lightbar(m_roi_img);
    get_armors();
    deal_muti_armors();
    get_armor_depth(m_roi_img,color,method);
}
/* @Des:this function is to get the binary image from raw image
 * @param:initial_img--raw image
 * @param:color--the color to detect(red,blue,green)
 * @param:method--the method(BGR,HSV,YUV)
 * @param:is_debug--debug or not(if debug,reset parameter )
 * @retval: binary image
 */
Mat armor_detect::get_binary_img(Mat initial_img,color_ENUM color,\
                                 GetLightBarMethod_ENUM method,bool is_debug)
{

    Mat gray_img,subtract_img,sub_Thre_img,final_img;

    m_is_debug = is_debug;
    m_raw_img = initial_img;
    switch (method)
    {
        case BGR_METHOD:
        {
            static int gray1_bar_value = 30;
            static int bgr_thre_value = 100;
            Mat bgr_img[3];

            cvtColor(initial_img,gray_img,COLOR_BGR2GRAY);
            threshold(gray_img,gray_img,gray1_bar_value,255,THRESH_BINARY);
            split(initial_img,bgr_img);
            switch(color)
            {
                case RED:
                {
                    subtract(bgr_img[2],bgr_img[0],subtract_img);
                    break;
                }
                case BLUE:
                {
                    subtract(bgr_img[0],bgr_img[2],subtract_img);
                    break;
                }
                case GREEN:
                {

                    break;
                }

            }
            threshold(subtract_img,sub_Thre_img,bgr_thre_value,255,THRESH_BINARY);
            erode(subtract_img, final_img, getStructuringElement(MORPH_RECT, Size(3,3)));
            morphologyEx(final_img, final_img, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(3,3)));
            if(m_is_debug)
            {
//                createTrackbar("gray1_bar","gray",&gray1_bar_value,255,gray1_bar_callback);
//                namedWindow("gray",WINDOW_AUTOSIZE);
//                imshow("gray",gray_img);
                //cout<<"gray1_bar_value"<<gray1_bar_value<<endl;

//                namedWindow("subtract_img",WINDOW_AUTOSIZE);
//                imshow("subtract_img",subtract_img);

                createTrackbar("bgr_thre","sub_Thre_img",&bgr_thre_value,255,gray1_bar_callback);
                namedWindow("sub_Thre_img",WINDOW_AUTOSIZE);
                imshow("sub_Thre_img",sub_Thre_img);

                namedWindow("final_img",WINDOW_AUTOSIZE);
                imshow("final_img",sub_Thre_img);
            }
            m_binary_img = final_img;
            break;
        }
        case HSV_METHOD:
        {
            static int H_Min = 0;
            static int H_Max = 36;
            static int S_Min = 164;
            static int S_Max = 255;
            static int V_Min = 40;
            static int V_Max = 255;
            Mat hsv_img;
            if(!m_is_debug)
            {
                switch(color)
                {
                    case RED:
                    {
                        H_Min = 0;
                        H_Max = 36;
                        S_Min = 164;
                        S_Max = 255;
                        V_Min = 40;
                        V_Max = 255;
                        break;
                    }
                    case BLUE:
                    {
                        H_Min = 80;
                        H_Max = 120;
                        S_Min = 180;
                        S_Max = 255;
                        V_Min = 80;
                        V_Max = 255;
                        break;
                    }
                    case GREEN:
                    {
                        H_Min = 80;
                        H_Max = 120;
                        S_Min = 180;
                        S_Max = 255;
                        V_Min = 80;
                        V_Max = 255;
                        break;
                    }
                }
            }
            cvtColor(initial_img, hsv_img, CV_BGR2HSV);
            inRange(hsv_img, Scalar(H_Min,S_Min,V_Min),Scalar(H_Max,S_Max,V_Max),hsv_img);
            erode(hsv_img, final_img, getStructuringElement(MORPH_RECT, Size(3,3)));
            morphologyEx(final_img, final_img, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(3,3)));
            if(m_is_debug)
            {
                namedWindow("hsv_img",WINDOW_NORMAL);
                imshow("hsv_img",hsv_img);
                createTrackbar("H_Min","hsv_img",&H_Min,255,gray1_bar_callback);
                createTrackbar("H_Max","hsv_img",&H_Max,255,gray1_bar_callback);
                createTrackbar("S_Min","hsv_img",&S_Min,255,gray1_bar_callback);
                createTrackbar("S_Max","hsv_img",&S_Max,255,gray1_bar_callback);
                createTrackbar("V_Min","hsv_img",&V_Min,255,gray1_bar_callback);
                createTrackbar("V_Max","hsv_img",&V_Max,255,gray1_bar_callback);
            }
            m_binary_img = hsv_img;
            break;

        }
        case YUV_METHOD:
        {
            break;
        }
    }

    return m_binary_img;
}
/* @Des:this function is to get the  lightbar from binary image
 * @param:initial_img--the image to show ellipse
 * @retval: binary image
 */
void armor_detect::get_lightbar(Mat initial_img)
{
    /* find the contours and find the useful rects, then save them */
    vector<vector<Point>>   contour;
    RotatedRect             Temp_Ellipse;
    float rectArea=0.0f;
    float rectProp=0.0f;
    int rectAngle=60;

    contour.clear();
    m_lightbars.clear();
    findContours(m_binary_img, contour, RETR_CCOMP , CHAIN_APPROX_NONE);
    for( size_t ii = 0; ii < contour.size() ; ++ii )
    {
        if((contour[ii].size() > 20))//5
        {
            Temp_Ellipse = fitEllipse(contour[ii]);
        }
        rectArea=Temp_Ellipse.size.area();
        if(Temp_Ellipse.size.width != 0.0f)
        {
            rectProp=Temp_Ellipse.size.height/Temp_Ellipse.size.width;
        }
        else
            continue;
        if(m_show_image)
        {
             ellipse(initial_img, Temp_Ellipse, CV_RGB(0, 0, 200), 2, 8);//show ellipse
        }

        rectAngle = abs((int)(Temp_Ellipse.angle) % 180);
        if(rectAngle > 90)
        {
            rectAngle += -180;
        }
        //cout<<"angle= "<<rectAngle<<"    area= "<<rectArea<<"   rectProp= "<<rectProp<<endl;
        if((rectAngle < 20 || rectAngle > -20) && \
            rectArea > 20  && rectArea < 15000 && \
            rectProp > 1.4f && rectProp < 20.0f )
        {
            m_lightbars.push_back(Temp_Ellipse);
            //cout << "size  " << m_lightbars.size() << endl;
            if(m_show_image)
                ellipse(initial_img, Temp_Ellipse, CV_RGB(190, 12, 0), 2, 8);//show ellipse
        }
        else
            continue;
    }
}
/* @Des:this function is to get the  armors from lightbars
 * @retval:
 */
void armor_detect::get_armors(void)
{
    /* the RotatedRects which are represented as the armors can be find in the following steps
     * 1. filter the m_lightbars RotatedRects by using the angle_diff, width_diff, height_diff
     *    *(1)the angel_diff decides the recognization of the different armors, if the angle threshold
     *    *is too big, it can not recognize different armors.
     *    *(2)width_diff, height_diff, This two thresholds will decide the max angle of armors, if they are too small, the
     *    *system can not get the armors when the armors have angles.
     * 2. filter the m_lightbars RotatedRects by using the angle of the armor from the horizontal axis.
     * 3. filter the m_lightbars RotatedRects by using the area. area can not be zero!
     * 4. filter the m_lightbars RotatedRects by using the width/height ratio. And using this value to judge whether is big armor!
     */
    static float angle_diff = 0.0f;
    static float angle_sum = 0.0f;
    static float width_diff = 0.0f;
    static float height_diff = 0.0f;
    static float width_sum = 0.0f;
    static float height_sum = 0.0f;
    static float WH_prop = 0.0f; // width/height
    static float temparmor_height = 0.0f;
    static float temparmor_width = 0.0f;
    static float CenterAngle = 0.0f;
    static float temparmor_area = 0.0f;
    RotatedRect  _temp_rect;

    static float angle_thred = 5.9f;
    static float width_thred = 0.57f;
    static float height_thred = 0.36f;
    static float wh_ratio_thred = 6.6f;

    m_useful_armors.clear();

    if(m_lightbars.size() < 2)
        return;
    //cout << m_lightbars.size() << endl;

/******************** enter the cycle to compare every two lightbar RotatedRects ******************/
    for( size_t i = 0; i < m_lightbars.size() - 1; ++i)
    {
        for( size_t j = i +1; j < m_lightbars.size(); ++j)
        {
            /***********************************************************************************/
            /************* use the angle, width_ratio, height_ratio to do the first filter *****/
            /***********************************************************************************/
            angle_sum = fabs(m_lightbars[i].angle + m_lightbars[j].angle );
            angle_diff = fabs(m_lightbars[i].angle - m_lightbars[j].angle);
            angle_diff = loop_float_constrain(angle_diff,-90,90);
            width_diff = fabs(m_lightbars[i].size.width - m_lightbars[j].size.width );
            height_diff = fabs(m_lightbars[i].size.height - m_lightbars[j].size.height);
            width_sum = fabs(m_lightbars[i].size.width + m_lightbars[j].size.width);
            height_sum = fabs(m_lightbars[i].size.height + m_lightbars[j].size.height);
            //cout << "angle :" << angle_diff  << "  width" << width_diff<< "  height" << height_diff << endl;
            if((fabs(angle_diff) < angle_thred ) &&\
               (width_diff < width_sum * width_thred ) &&\
               (height_diff < height_sum * height_thred ))
            {
                _temp_rect.center.x = (m_lightbars[i].center.x + m_lightbars[j].center.x)/2;
                _temp_rect.center.y = (m_lightbars[i].center.y + m_lightbars[j].center.y)/2;

                //ellipse center offset
                if(m_lightbars[i].center.x - m_lightbars[j].center.x != 0.0f )
                {
                        CenterAngle = sqrt((m_lightbars[i].center.y - m_lightbars[j].center.y)*\
                                                 (m_lightbars[i].center.y - m_lightbars[j].center.y) / \
                                                 ((m_lightbars[i].center.x - m_lightbars[j].center.x)*\
                                                 (m_lightbars[i].center.x - m_lightbars[j].center.x)));
                }
                if(CenterAngle > 0.2670f)  //15du
                     continue;
                //cout << "_temp_rect.angle    = " << CenterAngle << endl;

                temparmor_height = (m_lightbars[i].size.height + m_lightbars[j].size.height) / 2.0f ;
                temparmor_width  = sqrt((m_lightbars[i].center.x - m_lightbars[j].center.x) * \
                                        (m_lightbars[i].center.x - m_lightbars[j].center.x) + \
                                        ((m_lightbars[i].center.y - m_lightbars[j].center.y) * \
                                         (m_lightbars[i].center.y - m_lightbars[j].center.y)));
                temparmor_area = temparmor_width * temparmor_height;
//                cout << "area   " << temparmor_area << endl;
                if(temparmor_area <= 400.0f || temparmor_area > 370000)
                    continue;
                /***********************************************************************************/
                /********************* use the width/height ratio to do the second filter **********/
                /***********************************************************************************/
                WH_prop = temparmor_width / temparmor_height;
                if( WH_prop <  wh_ratio_thred)
                {
                    //calculate the correct armor angle
                    if(m_lightbars[i].center.x - m_lightbars[j].center.x > 0.0f)
                    {
                        if(m_lightbars[i].center.y - m_lightbars[j].center.y < 0.0f)
                            _temp_rect.angle = CenterAngle * 180.0f / 3.1415926f;
                        else if(m_lightbars[i].center.y - m_lightbars[j].center.y > 0.0f)
                            _temp_rect.angle = -CenterAngle * 180.0f / 3.1415926f;
                        /* left lightbar is 0 2 4 6, so the number is 2*(lightbarIndex++)
                         * right lightbar is 1 3 5 7 , so the number is 2*lightbarIndex-1 */
                        m_useful_lightbars[0] = (m_lightbars[j]);
                        m_useful_lightbars[1]  = (m_lightbars[i]);
                    }
                    else if(m_lightbars[i].center.x - m_lightbars[j].center.x < 0.0f)
                    {
                        if(m_lightbars[i].center.y - m_lightbars[j].center.y > 0.0f)
                            _temp_rect.angle = -CenterAngle * 180.0f / 3.1415926f;
                        else if(m_lightbars[i].center.y - m_lightbars[j].center.y < 0.0f)
                            _temp_rect.angle = CenterAngle * 180.0f / 3.1415926f;
                        m_useful_lightbars[0] = (m_lightbars[i]);
                        m_useful_lightbars[1] = (m_lightbars[j]);
                    }
                    _temp_rect.size.width = temparmor_width;
                    _temp_rect.size.height = temparmor_height;

                    //cout << "angle :" << angle_diff  << "  width" << width_diff<< "  height" << height_diff << endl;
                    /* judge whether there is the same armor in the vector */
                    m_useful_armors.push_back(_temp_rect);
                }
                else
                  continue;
//                cout << "WH_prop   "  << WH_prop << endl;
            }
        }
    }
}
/* @Des:this function is to deal with many armors
 * @param: img--the image to show
 * @retval:
 */
void armor_detect::deal_muti_armors(void)
{

    if (m_useful_armors.size() == 0 )
    {
        m_target_center.x = m_target_center.y = 0;
        m_target_center.z = 0.0f;
        m_targetinfo.TargetType = NoneArmor;
        m_targetinfo.TargetTrackerStatus = LoseTarget;
    }
    else if(m_useful_armors.size() >= 1)
    {
        if(m_useful_armors.size() >= 2)
        {
            /* sort by the distance between the armor center and screen center  */
            sort(m_useful_armors.begin(),m_useful_armors.end(),CenterDisSort);/*small to big*/
            //cout << "area   "  << m_pair_armor_lightbars[0].first.size.area() << endl;
        }
         /* give the final pair to the g_tragetinfo */
         m_target_center.x = m_useful_armors[0].center.x;
         m_target_center.y = m_useful_armors[0].center.y;
         //m_targetinfo.TargetPair = m_useful_armors[0];
         m_targetinfo.TargetTrackerStatus = FindTarget;
    }
}
/* @Des:this function is to get the depth of armor
 * @param: img--the image to show
 * @retval:
 */
void armor_detect::get_armor_depth(Mat img,color_ENUM color,GetLightBarMethod_ENUM method)
{
    /* calculate the area projection angle */
    /* This part is for the case that the depth is not correct when the target rotate with some angle */
    /* area_projection_angle : this value is the height difference divided by the armor height , and times a
     *                         specifical params. It means the projection angle
     * projection_out        : this value means the compensation of the depth by using the projection angle
     *  !!! attention: the big armor has some cases not fit the curve , so it need to do some changes */
    float wh_ratio = 0.0f;
    float area_projection_angle = 0.0f;
    float projection_out = 0.0f;
    static float BigOrSmall_thred = 3.2f;
    static int depthAreaSmallArmor_Ratio,depthAreaBigArmor_Ratio;
    switch(method)
    {
        case BGR_METHOD:
        {
            if(color == RED)
            {
                depthAreaSmallArmor_Ratio = 2600;//1120
                depthAreaBigArmor_Ratio = 3280;
            }
            else
            {
                depthAreaSmallArmor_Ratio = 2600;//1120
                depthAreaBigArmor_Ratio = 3500;
            }
            break;
        }
        case HSV_METHOD:
        {
            if(color == RED)
            {
                depthAreaSmallArmor_Ratio = 1436;//1535
                depthAreaBigArmor_Ratio = 2000;
            }
            else
            {
                depthAreaSmallArmor_Ratio = 1485;
                depthAreaBigArmor_Ratio = 2000;
            }
            break;
        }
        case YUV_METHOD:
        {
            break;
        }
    }
    if(m_useful_armors.size() == 1)
    {
        area_projection_angle = 100.0f * (m_useful_lightbars[1].size.height -\
                                         m_useful_lightbars[0].size.height) / m_useful_armors[0].size.height;
        area_projection_angle = kalman1_filter(&g_angle_projection_kf, area_projection_angle);
        //        cout << "                height_ratio  " << area_projection_angle << endl;// (g_tragetinfo.TargetPair.first.size.area()/g_tragetinfo.TargetPair.first.size.width)<< endl;
        //        cout << "pts   " << m_target_center << "flag   "<< IsSmallArmor << endl;
        projection_out = 0.0411f*(fabs(area_projection_angle))*(fabs(area_projection_angle)) + 14.0f * (fabs(area_projection_angle)) + 8.6f;
        //        cout << "  compasation " << projection_out << endl;

        /* calculate the width/height to find the depth and whether it is small armor */
        wh_ratio = m_useful_armors[0].size.width / m_useful_armors[0].size.height;
        if(wh_ratio < BigOrSmall_thred)
        {
            m_targetinfo.TargetType = SmallArmor;
            m_target_center.z = -projection_out + (float)(depthAreaSmallArmor_Ratio*97.f/(sqrt(m_useful_armors[0].size.area())));
        }
        else
        {
           m_targetinfo.TargetType = BigArmor;
            if(area_projection_angle > 0)
                m_target_center.z = area_projection_angle * 4 -projection_out + (float)(depthAreaBigArmor_Ratio*97.f/(sqrt(m_useful_armors[0].size.area())));
            else
                m_target_center.z = -projection_out + (float)(depthAreaBigArmor_Ratio*97.f/(sqrt(m_useful_armors[0].size.area())));
        }
        //cout << "   cos_angle   "<< Kalman.kalman1_filter(&angle_cos_kf, cosf(fabs(area_projection_angle))) << endl;
        if(m_show_image)
        {
            //            Point2f armorpts[4];
            //            g_tragetinfo.TargetPair.first.points(armorpts);
            //            line(img, armorpts[0], armorpts[1], CV_RGB(0, 255, 0), 3, 8, 0);
            //            line(img, armorpts[1], armorpts[2], CV_RGB(0, 255, 0), 3, 8, 0);
            //            line(img, armorpts[2], armorpts[3], CV_RGB(0, 255, 0), 3, 8, 0);
            //            line(img, armorpts[3], armorpts[0], CV_RGB(0, 255, 0), 3, 8, 0);

            ellipse(img, m_useful_armors[0], CV_RGB(255,87,64), 2, 8);//show ellipse
            ellipse(img, m_useful_lightbars[1], CV_RGB(0, 255, 0), 2, 8);//show ellipse
            ellipse(img, m_useful_lightbars[0],  CV_RGB(0, 255, 0), 2, 8);//show ellipse
        }
        //        cout << "height   " << (g_tragetinfo.TargetPair.second[0].size.height + g_tragetinfo.TargetPair.second[1].size.height)/2.0f <<endl;
        m_targetinfo.Trans_ratio = LIGHTBAR_REALHEIGHT / (( m_useful_lightbars[0].size.height +  m_useful_lightbars[1].size.height)/2.0f);
          //        cout << "g_tragetinfo.Trans_ratio   " << g_tragetinfo.Trans_ratio <<endl;

        if(m_show_image)
           circle(img, m_useful_armors[0].center, 3, Scalar(120, 167, 61), 2, 8);
    }

}
