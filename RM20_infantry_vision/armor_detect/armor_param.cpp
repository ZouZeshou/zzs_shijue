#include "armor_detect/armor_detect.h"
#include "algorithm/include/usr_math.h"
#include "armor_detect/armor_param.h"
armor_param_t armor_param;
static void trackbar_callback1(int, void*){};
/* @Des:this function is initialise the parameter
 * @param:Mat src--raw image
 * @retval:
 */
void armor_param_init()
{
    armor_param.GRAY_THRESHOLD = 15;
    armor_param.BGR_THRESHOLD = 100;

    armor_param.RED_H_MAX = 36;
    armor_param.RED_H_MIN = 0;
    armor_param.RED_S_MAX = 255;
    armor_param.RED_S_MIN = 164;
    armor_param.RED_V_MAX = 255;
    armor_param.RED_V_MIN = 40;
    armor_param.BLUE_H_MAX = 120;
    armor_param.BLUE_H_MIN = 80;
    armor_param.BLUE_S_MAX = 255;
    armor_param.BLUE_S_MIN = 180;
    armor_param.BLUE_V_MAX = 255;
    armor_param.BLUE_V_MIN = 80;

    armor_param.RED_Y_MAX = 255;
    armor_param.RED_Y_MIN = 50;
    armor_param.RED_CR_MAX = 255;
    armor_param.RED_CR_MIN = 140;
    armor_param.RED_CB_MAX = 255;
    armor_param.RED_CB_MIN = 10;
    armor_param.BLUE_Y_MAX = 255;
    armor_param.BLUE_Y_MIN = 20;
    armor_param.BLUE_CR_MAX = 110;
    armor_param.BLUE_CR_MIN = 0;
    armor_param.BLUE_CB_MAX = 255;
    armor_param.BLUE_CB_MIN = 90;

    armor_param.LIGHTBAR_CONTOUR_MIN_SIZE = 20;
    armor_param.LIGHTBAR_CONTOUR_AREA_MAX = 1500;
    armor_param.LIGHTBAR_CONTOUR_AREA_MIN = 20;
    armor_param.LIGHTBAR_CONTOUR_LENGTH_MAX = 300;
    armor_param.LIGHTBAR_CONTOUR_LENGTH_MIN = 10;
    armor_param.LIGHTBAR_CONTOUR_WIDTH_MAX = 80;
    armor_param.LIGHTBAR_CONTOUR_WIDTH_MIN = 1;
    armor_param.LIGHTBAR_CONTOUR_ANGLE_MAX = 20;
    armor_param.LIGHTBAR_CONTOUR_HW_RATIO_MAX = 2000;
    armor_param.LIGHTBAR_CONTOUR_HW_RATIO_MIN = 100;
    armor_param.LIGHTBAR_CONTOUR_AREA_RATIO_MIN = 50;

    armor_param.ARMOR_ANGLE_DIFF = 590;
    armor_param.ARMOR_LENGTH_DIFF_RATIO = 36;
    armor_param.ARMOR_WIDTH_DIFF_RATIO = 57;
    armor_param.ARMOR_MAX_HORIZONTAL_ANGLE = 15;
    armor_param.ARMOR_AREA_MAX = 2000;
    armor_param.ARMOR_AREA_MIN = 20;
    armor_param.ARMOR_HW_RATIO_MAX = 600;
    armor_param.ARMOR_HW_RATIO_MIN = 60;

    armor_param.DEPTH_BGR_RED_AREA_BIG = 3280;
    armor_param.DEPTH_BGR_RED_AREA_SMALL = 2600;
    armor_param.DEPTH_BGR_BLUE_AREA_BIG = 3500;
    armor_param.DEPTH_BGR_BLUE_AREA_SMALL = 2600;
    armor_param.DEPTH_HSV_RED_AREA_BIG = 2000;
    armor_param.DEPTH_HSV_RED_AREA_SMALL = 1436;
    armor_param.DEPTH_HSV_BLUE_AREA_BIG = 2000;
    armor_param.DEPTH_HSV_BLUE_AREA_SMALL = 1485;
    armor_param.DEPTH_YUV_RED_AREA_BIG = 2000;
    armor_param.DEPTH_YUV_RED_AREA_SMALL = 1450;
    armor_param.DEPTH_YUV_BLUE_AREA_BIG = 2000;
    armor_param.DEPTH_YUV_BLUE_AREA_SMALL = 1450;
    armor_param.DEPTH_SMALL_ARMOR_HW_RATIO_MAX = 320;

    armor_param.LIGHTBAR_REALHEIGHT = 60;
}
/* @Des:this function is to adjust the parameter
 * @param:type:
 *         1--adjust the binary parameter
 *         2--adjust the find lightbar parameter
 *         3--adjust the find armor parameter
 *         4--adjust the depth parameter
 * @retval:
 */
void armor_detect::adjust_param(uint8_t type,GetLightBarMethod_ENUM method)
{
    switch(type)
    {
        case 1:
        {
            if(method == HSV_METHOD)
            {
                namedWindow("hsv_param", WINDOW_NORMAL);
                createTrackbar("RED_H_MAX", "hsv_param", &armor_param.RED_H_MAX, 255, trackbar_callback1);
                createTrackbar("RED_H_MIN", "hsv_param", &armor_param.RED_H_MIN, 255, trackbar_callback1);
                createTrackbar("RED_S_MAX", "hsv_param", &armor_param.RED_S_MAX, 255, trackbar_callback1);
                createTrackbar("RED_S_MIN", "hsv_param", &armor_param.RED_S_MIN, 255, trackbar_callback1);
                createTrackbar("RED_V_MAX", "hsv_param", &armor_param.RED_V_MAX, 255, trackbar_callback1);
                createTrackbar("RED_V_MIN", "hsv_param", &armor_param.RED_V_MIN, 255, trackbar_callback1);
                createTrackbar("BLUE_H_MAX", "hsv_param", &armor_param.BLUE_H_MAX, 255, trackbar_callback1);
                createTrackbar("BLUE_H_MIN", "hsv_param", &armor_param.BLUE_H_MIN, 255, trackbar_callback1);
                createTrackbar("BLUE_S_MAX", "hsv_param", &armor_param.BLUE_S_MAX, 255, trackbar_callback1);
                createTrackbar("BLUE_S_MIN", "hsv_param", &armor_param.BLUE_S_MIN, 255, trackbar_callback1);
                createTrackbar("BLUE_V_MAX", "hsv_param", &armor_param.BLUE_V_MAX, 255, trackbar_callback1);
                createTrackbar("BLUE_V_MIN", "hsv_param", &armor_param.BLUE_V_MIN, 255, trackbar_callback1);
            }
            else if(method == YUV_METHOD)
            {
                namedWindow("yuv_param", WINDOW_NORMAL);
                createTrackbar("RED_Y_MAX", "yuv_param", &armor_param.RED_Y_MAX, 255, trackbar_callback1);
                createTrackbar("RED_Y_MIN", "yuv_param", &armor_param.RED_Y_MIN, 255, trackbar_callback1);
                createTrackbar("RED_CR_MAX", "yuv_param", &armor_param.RED_CR_MAX, 255, trackbar_callback1);
                createTrackbar("RED_CR_MIN", "yuv_param", &armor_param.RED_CR_MIN, 255, trackbar_callback1);
                createTrackbar("RED_CB_MAX", "yuv_param", &armor_param.RED_CB_MAX, 255, trackbar_callback1);
                createTrackbar("RED_CB_MIN", "yuv_param", &armor_param.RED_CB_MIN, 255, trackbar_callback1);
                createTrackbar("BLUE_Y_MAX", "yuv_param", &armor_param.BLUE_Y_MAX, 255, trackbar_callback1);
                createTrackbar("BLUE_Y_MIN", "yuv_param", &armor_param.BLUE_Y_MIN, 255, trackbar_callback1);
                createTrackbar("BLUE_CR_MAX", "yuv_param", &armor_param.BLUE_CR_MAX, 255, trackbar_callback1);
                createTrackbar("BLUE_CR_MIN", "yuv_param", &armor_param.BLUE_CR_MIN, 255, trackbar_callback1);
                createTrackbar("BLUE_CB_MAX", "yuv_param", &armor_param.BLUE_CB_MAX, 255, trackbar_callback1);
                createTrackbar("BLUE_CB_MIN", "yuv_param", &armor_param.BLUE_CB_MIN, 255, trackbar_callback1);
            }
            else
            {
                namedWindow("gray_param", WINDOW_NORMAL);
                createTrackbar("GRAY_THRESHOLD", "gray_param", &armor_param.GRAY_THRESHOLD, 255, trackbar_callback1);
                createTrackbar("BGR_THRESHOLD", "gray_param", &armor_param.BGR_THRESHOLD, 255, trackbar_callback1);
            }
            break;
         }
        case 2:
        {
            namedWindow("lightbar_param", WINDOW_NORMAL);
            createTrackbar("CONTOUR_MIN_SIZE", "lightbar_param", & armor_param.LIGHTBAR_CONTOUR_MIN_SIZE, 100, trackbar_callback1);
            createTrackbar("AREA_MAX", "lightbar_param", &armor_param.LIGHTBAR_CONTOUR_AREA_MAX, 3000, trackbar_callback1);
            createTrackbar("AREA_MIN", "lightbar_param", &armor_param.LIGHTBAR_CONTOUR_AREA_MIN, 500, trackbar_callback1);
            createTrackbar("LENGTH_MAX", "lightbar_param", &armor_param.LIGHTBAR_CONTOUR_LENGTH_MAX, 500, trackbar_callback1);
            createTrackbar("LENGTH_MIN", "lightbar_param", &armor_param.LIGHTBAR_CONTOUR_LENGTH_MIN, 200, trackbar_callback1);
            createTrackbar("WIDTH_MAX", "lightbar_param", &armor_param.LIGHTBAR_CONTOUR_WIDTH_MAX, 300, trackbar_callback1);
            createTrackbar("WIDTH_MIN", "lightbar_param", &armor_param.LIGHTBAR_CONTOUR_WIDTH_MIN, 100, trackbar_callback1);
            createTrackbar("ANGLE_MAX", "lightbar_param", &armor_param.LIGHTBAR_CONTOUR_ANGLE_MAX, 90, trackbar_callback1);
            createTrackbar("HW_RATIO_MAX", "lightbar_param", &armor_param.LIGHTBAR_CONTOUR_HW_RATIO_MAX, 5000, trackbar_callback1);
            createTrackbar("HW_RATIO_MIN", "lightbar_param", &armor_param.LIGHTBAR_CONTOUR_HW_RATIO_MIN, 1000, trackbar_callback1);
            createTrackbar("AREA_RATIO_MIN", "lightbar_param", &armor_param.LIGHTBAR_CONTOUR_AREA_RATIO_MIN, 100, trackbar_callback1);
            break;
         }
        case 3:
        {
            namedWindow("armor_param", WINDOW_NORMAL);
            createTrackbar("ANGLE_DIFF", "armor_param", &armor_param.ARMOR_ANGLE_DIFF, 1000, trackbar_callback1);
            createTrackbar("LENGTH_DIFF_RATIO", "armor_param", &armor_param.ARMOR_LENGTH_DIFF_RATIO, 200, trackbar_callback1);
            createTrackbar("WIDTH_DIFF_RATIO", "armor_param", &armor_param.ARMOR_WIDTH_DIFF_RATIO, 200, trackbar_callback1);
            createTrackbar("MAX_HORIZONTAL_ANGLE", "armor_param", &armor_param.ARMOR_MAX_HORIZONTAL_ANGLE, 90, trackbar_callback1);
            createTrackbar("ARMOR_AREA_MAX", "armor_param", &armor_param.ARMOR_AREA_MAX, 2000, trackbar_callback1);
            createTrackbar("ARMOR_AREA_MIN", "armor_param", &armor_param.ARMOR_AREA_MIN, 100, trackbar_callback1);
            createTrackbar("HW_RATIO_MAX", "armor_param", &armor_param.ARMOR_HW_RATIO_MAX, 1000, trackbar_callback1);
            createTrackbar("HW_RATIO_MIN", "armor_param", &armor_param.ARMOR_HW_RATIO_MIN, 1000, trackbar_callback1);
            break;
         }
        case 4:
        {
            namedWindow("depth_param", WINDOW_NORMAL);
            createTrackbar("DEPTH_SMALL_ARMOR_HW_RATIO_MAX", "depth_param", &armor_param.DEPTH_SMALL_ARMOR_HW_RATIO_MAX, 1000, trackbar_callback1);
            if(method == BGR_METHOD)
            {
                createTrackbar("DEPTH_BGR_RED_AREA_BIG", "depth_param", &armor_param.DEPTH_BGR_RED_AREA_BIG, 5000, trackbar_callback1);
                createTrackbar("DEPTH_BGR_RED_AREA_SMALL", "depth_param", &armor_param.DEPTH_BGR_RED_AREA_SMALL, 5000, trackbar_callback1);
                createTrackbar("DEPTH_BGR_BLUE_AREA_BIG", "depth_param", &armor_param.DEPTH_BGR_BLUE_AREA_BIG, 5000, trackbar_callback1);
                createTrackbar("DEPTH_BGR_BLUE_AREA_SMALL", "depth_param", &armor_param.DEPTH_BGR_BLUE_AREA_SMALL, 5000, trackbar_callback1);
            }
            else if(method == HSV_METHOD)
            {
                createTrackbar("DEPTH_HSV_RED_AREA_BIG", "depth_param", &armor_param.DEPTH_HSV_RED_AREA_BIG, 5000, trackbar_callback1);
                createTrackbar("DEPTH_HSV_RED_AREA_SMALL", "depth_param", &armor_param.DEPTH_HSV_RED_AREA_SMALL, 5000, trackbar_callback1);
                createTrackbar("DEPTH_HSV_BLUE_AREA_BIG", "depth_param", &armor_param.DEPTH_HSV_BLUE_AREA_BIG, 5000, trackbar_callback1);
                createTrackbar("DEPTH_HSV_BLUE_AREA_SMALL", "depth_param", &armor_param.DEPTH_HSV_BLUE_AREA_SMALL, 5000, trackbar_callback1);
            }
            else
            {
                createTrackbar("DEPTH_YUV_RED_AREA_BIG", "depth_param", &armor_param.DEPTH_YUV_RED_AREA_BIG, 5000, trackbar_callback1);
                createTrackbar("DEPTH_YUV_RED_AREA_SMALL", "depth_param", &armor_param.DEPTH_YUV_RED_AREA_SMALL, 5000, trackbar_callback1);
                createTrackbar("DEPTH_YUV_BLUE_AREA_BIG", "depth_param", &armor_param.DEPTH_YUV_BLUE_AREA_BIG, 5000, trackbar_callback1);
                createTrackbar("DEPTH_YUV_BLUE_AREA_SMALL", "depth_param", &armor_param.DEPTH_YUV_BLUE_AREA_SMALL, 5000, trackbar_callback1);
            }

            break;
         }
        default:
            break;

    }
}
