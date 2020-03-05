#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "algorithm/include/usr_math.h"
#include "fanwheel/fan_param.h"
fan_param_t    fan_param;
static void trackbar_callback1(int, void*){};
/* @Des:this function is initialise the parameter
 * @param:Mat src--raw image
 * @retval:
 */
void fan_param_init()
{
    fan_param.GRAY_THRESHOLD = 15;

    fan_param.RED_H_MAX = 50;
    fan_param.RED_H_MIN = 0;
    fan_param.RED_S_MAX = 255;
    fan_param.RED_S_MIN = 65;
    fan_param.RED_V_MAX = 255;
    fan_param.RED_V_MIN = 60;
    fan_param.BLUE_H_MAX = 110;
    fan_param.BLUE_H_MIN = 75;
    fan_param.BLUE_S_MAX = 255;
    fan_param.BLUE_S_MIN = 150;
    fan_param.BLUE_V_MAX = 255;
    fan_param.BLUE_V_MIN = 30;

    fan_param.RED_Y_MAX = 255;
    fan_param.RED_Y_MIN = 50;
    fan_param.RED_CR_MAX = 255;
    fan_param.RED_CR_MIN = 140;
    fan_param.RED_CB_MAX = 255;
    fan_param.RED_CB_MIN = 10;
    fan_param.BLUE_Y_MAX = 255;
    fan_param.BLUE_Y_MIN = 20;
    fan_param.BLUE_CR_MAX = 110;
    fan_param.BLUE_CR_MIN = 0;
    fan_param.BLUE_CB_MAX = 255;
    fan_param.BLUE_CB_MIN = 90;

    fan_param.ARMOR_CONTOUR_AREA_MAX = 700;
    fan_param.ARMOR_CONTOUR_AREA_MIN = 100;
    fan_param.ARMOR_CONTOUR_LENGTH_MAX = 50;
    fan_param.ARMOR_CONTOUR_LENGTH_MIN = 10;
    fan_param.ARMOR_CONTOUR_WIDTH_MAX = 20;
    fan_param.ARMOR_CONTOUR_WIDTH_MIN = 4;
    fan_param.ARMOR_CONTOUR_HW_RATIO_MAX = 300;
    fan_param.ARMOR_CONTOUR_HW_RATIO_MIN = 100;
    fan_param.ARMOR_CONTOUR_AREA_RATIO_MIN = 60;
    fan_param.ARMOR_INTERSETION_AREA_MIN = 100;

    fan_param.FLOW_STRIP_FAN_CONTOUR_AREA_MAX = 1800;
    fan_param.FLOW_STRIP_FAN_CONTOUR_AREA_MIN = 800;
    fan_param.FLOW_STRIP_FAN_CONTOUR_LENGTH_MAX = 90;
    fan_param.FLOW_STRIP_FAN_CONTOUR_LENGTH_MIN = 50;
    fan_param.FLOW_STRIP_FAN_CONTOUR_WIDTH_MAX = 45;
    fan_param.FLOW_STRIP_FAN_CONTOUR_WIDTH_MIN = 15;
    fan_param.FLOW_STRIP_FAN_CONTOUR_HW_RATIO_MAX = 250;
    fan_param.FLOW_STRIP_FAN_CONTOUR_HW_RATIO_MIN = 120;
    fan_param.FLOW_STRIP_FAN_CONTOUR_AREA_RATIO_MAX = 65;
    fan_param.FLOW_STRIP_FAN_CONTOUR_AREA_RATIO_MIN = 35;

    fan_param.FLOW_STRIP_CONTOUR_AREA_MAX = 600;
    fan_param.FLOW_STRIP_CONTOUR_AREA_MIN = 200;
    fan_param.FLOW_STRIP_CONTOUR_LENGTH_MAX = 70;
    fan_param.FLOW_STRIP_CONTOUR_LENGTH_MIN = 35;
    fan_param.FLOW_STRIP_CONTOUR_WIDTH_MAX = 20;
    fan_param.FLOW_STRIP_CONTOUR_WIDTH_MIN = 4;
    fan_param.FLOW_STRIP_CONTOUR_HW_RATIO_MAX = 700;
    fan_param.FLOW_STRIP_CONTOUR_HW_RATIO_MIN = 300;
    fan_param.FLOW_STRIP_CONTOUR_AREA_RATIO_MIN = 60;
    fan_param.FLOW_STRIP_CONTOUR_INTERSETION_AREA_MIN = 200;

    fan_param.CENTER_R_CONTOUR_AREA_MAX = 200;
    fan_param.CENTER_R_CONTOUR_AREA_MIN = 40;
    fan_param.CENTER_R_CONTOUR_LENGTH_MAX = 20;
    fan_param.CENTER_R_CONTOUR_LENGTH_MIN = 6;
    fan_param.CENTER_R_CONTOUR_WIDTH_MAX = 20;
    fan_param.CENTER_R_CONTOUR_WIDTH_MIN = 6;
    fan_param.CENTER_R_CONTOUR_HW_RATIO_MAX = 200;
    fan_param.CENTER_R_CONTOUR_HW_RATIO_MIN = 100;
    fan_param.CENTER_R_CONTOUR_AREA_RATIO_MIN = 65;
    fan_param.CENTER_R_CONTOUR_INTERSETION_AREA_MIN = 50;

    fan_param.PRE_SPEED_OFFSET = 75;
    fan_param.PRE_SPEED_RATIO = 150;
    fan_param.PRE_SLOPE_SACLE = 220;
    fan_param.PRE_SPEED_PROBABILITY = 40;
    fan_param.PRE_SLOPE_PROBABILITY = 20;
    fan_param.PRE_TIME_PROBABILITY = 40;
    fan_param.PRE_SLOPE_OFFSET = 200;
    fan_param.PRE_SECTION_OFFSET = 260;
}
/* @Des:this function is to adjust the parameter
 * @param:type:
 *         1--adjust the binary parameter
 *         2--adjust the armor find parameter
 *         3--adjust the flow fans parameter
 *         4--adjust the flow strip parameter
 *         5--adjust the R parameter
 *         6--adjust the predict parameter
 * @retval:
 */
void fan::adjust_param(uint8_t type,GetLightBarMethod_ENUM method)
{
    if(!IS_CREATE_WINDOW)
        return;
    switch(type)
    {
        case 1:
        {
            if(method == HSV_METHOD)
            {
                namedWindow("hsv_param", WINDOW_NORMAL);
                createTrackbar("RED_H_MAX", "hsv_param", &fan_param.RED_H_MAX, 255, trackbar_callback1);
                createTrackbar("RED_H_MIN", "hsv_param", &fan_param.RED_H_MIN, 255, trackbar_callback1);
                createTrackbar("RED_S_MAX", "hsv_param", &fan_param.RED_S_MAX, 255, trackbar_callback1);
                createTrackbar("RED_S_MIN", "hsv_param", &fan_param.RED_S_MIN, 255, trackbar_callback1);
                createTrackbar("RED_V_MAX", "hsv_param", &fan_param.RED_V_MAX, 255, trackbar_callback1);
                createTrackbar("RED_V_MIN", "hsv_param", &fan_param.RED_V_MIN, 255, trackbar_callback1);
                createTrackbar("BLUE_H_MAX", "hsv_param", &fan_param.BLUE_H_MAX, 255, trackbar_callback1);
                createTrackbar("BLUE_H_MIN", "hsv_param", &fan_param.BLUE_H_MIN, 255, trackbar_callback1);
                createTrackbar("BLUE_S_MAX", "hsv_param", &fan_param.BLUE_S_MAX, 255, trackbar_callback1);
                createTrackbar("BLUE_S_MIN", "hsv_param", &fan_param.BLUE_S_MIN, 255, trackbar_callback1);
                createTrackbar("BLUE_V_MAX", "hsv_param", &fan_param.BLUE_V_MAX, 255, trackbar_callback1);
                createTrackbar("BLUE_V_MIN", "hsv_param", &fan_param.BLUE_V_MIN, 255, trackbar_callback1);
            }
            else if(method == YUV_METHOD)
            {
                namedWindow("yuv_param", WINDOW_NORMAL);
                createTrackbar("RED_Y_MAX", "yuv_param", &fan_param.RED_Y_MAX, 255, trackbar_callback1);
                createTrackbar("RED_Y_MIN", "yuv_param", &fan_param.RED_Y_MIN, 255, trackbar_callback1);
                createTrackbar("RED_CR_MAX", "yuv_param", &fan_param.RED_CR_MAX, 255, trackbar_callback1);
                createTrackbar("RED_CR_MIN", "yuv_param", &fan_param.RED_CR_MIN, 255, trackbar_callback1);
                createTrackbar("RED_CB_MAX", "yuv_param", &fan_param.RED_CB_MAX, 255, trackbar_callback1);
                createTrackbar("RED_CB_MIN", "yuv_param", &fan_param.RED_CB_MIN, 255, trackbar_callback1);
                createTrackbar("BLUE_Y_MAX", "yuv_param", &fan_param.BLUE_Y_MAX, 255, trackbar_callback1);
                createTrackbar("BLUE_Y_MIN", "yuv_param", &fan_param.BLUE_Y_MIN, 255, trackbar_callback1);
                createTrackbar("BLUE_CR_MAX", "yuv_param", &fan_param.BLUE_CR_MAX, 255, trackbar_callback1);
                createTrackbar("BLUE_CR_MIN", "yuv_param", &fan_param.BLUE_CR_MIN, 255, trackbar_callback1);
                createTrackbar("BLUE_CB_MAX", "yuv_param", &fan_param.BLUE_CB_MAX, 255, trackbar_callback1);
                createTrackbar("BLUE_CB_MIN", "yuv_param", &fan_param.BLUE_CB_MIN, 255, trackbar_callback1);
            }
            else
            {
                namedWindow("gray_param", WINDOW_NORMAL);
                createTrackbar("GRAY_THRESHOLD", "gray_param", &fan_param.GRAY_THRESHOLD, 255, trackbar_callback1);
            }
            break;
         }
        case 2:
        {
            namedWindow("armor_param", WINDOW_NORMAL);
            createTrackbar("AREA_MAX", "armor_param", &fan_param.ARMOR_CONTOUR_AREA_MAX, 3000, trackbar_callback1);
            createTrackbar("AREA_MIN", "armor_param", &fan_param.ARMOR_CONTOUR_AREA_MIN, 3000, trackbar_callback1);
            createTrackbar("LENGTH_MAX", "armor_param", &fan_param.ARMOR_CONTOUR_LENGTH_MAX, 150, trackbar_callback1);
            createTrackbar("LENGTH_MIN", "armor_param", &fan_param.ARMOR_CONTOUR_LENGTH_MIN, 150, trackbar_callback1);
            createTrackbar("WIDTH_MAX", "armor_param", &fan_param.ARMOR_CONTOUR_WIDTH_MAX, 80, trackbar_callback1);
            createTrackbar("WIDTH_MIN", "armor_param", &fan_param.ARMOR_CONTOUR_WIDTH_MIN, 80, trackbar_callback1);
            createTrackbar("HW_RATIO_MAX", "armor_param", &fan_param.ARMOR_CONTOUR_HW_RATIO_MAX, 1000, trackbar_callback1);
            createTrackbar("HW_RATIO_MIN", "armor_param", &fan_param.ARMOR_CONTOUR_HW_RATIO_MIN, 1000, trackbar_callback1);
            createTrackbar("AREA_RATIO_MIN", "armor_param", &fan_param.ARMOR_CONTOUR_AREA_RATIO_MIN, 100, trackbar_callback1);
            createTrackbar("INTERSETION", "armor_param", &fan_param.ARMOR_INTERSETION_AREA_MIN, 500, trackbar_callback1);
            break;
         }
        case 3:
        {
            namedWindow("fan_param", WINDOW_NORMAL);
            createTrackbar("AREA_MAX", "fan_param", &fan_param.FLOW_STRIP_FAN_CONTOUR_AREA_MAX, 3000, trackbar_callback1);
            createTrackbar("AREA_MIN", "fan_param", &fan_param.FLOW_STRIP_FAN_CONTOUR_AREA_MIN, 3000, trackbar_callback1);
            createTrackbar("LENGTH_MAX", "fan_param", &fan_param.FLOW_STRIP_FAN_CONTOUR_LENGTH_MAX, 150, trackbar_callback1);
            createTrackbar("LENGTH_MIN", "fan_param", &fan_param.FLOW_STRIP_FAN_CONTOUR_LENGTH_MIN, 150, trackbar_callback1);
            createTrackbar("WIDTH_MAX", "fan_param", &fan_param.FLOW_STRIP_FAN_CONTOUR_WIDTH_MAX, 80, trackbar_callback1);
            createTrackbar("WIDTH_MIN", "fan_param", &fan_param.FLOW_STRIP_FAN_CONTOUR_WIDTH_MIN, 80, trackbar_callback1);
            createTrackbar("HW_RATIO_MAX", "fan_param", &fan_param.FLOW_STRIP_FAN_CONTOUR_HW_RATIO_MAX, 1000, trackbar_callback1);
            createTrackbar("HW_RATIO_MIN", "fan_param", &fan_param.FLOW_STRIP_FAN_CONTOUR_HW_RATIO_MIN, 1000, trackbar_callback1);
            createTrackbar("AREA_RATIO_MAX", "fan_param", &fan_param.FLOW_STRIP_FAN_CONTOUR_AREA_RATIO_MAX, 100, trackbar_callback1);
            createTrackbar("AREA_RATIO_MIN", "fan_param", &fan_param.FLOW_STRIP_FAN_CONTOUR_AREA_RATIO_MIN, 100, trackbar_callback1);
            break;
         }
        case 4:
        {
            namedWindow("strip_param", WINDOW_NORMAL);
            createTrackbar("AREA_MAX", "strip_param", &fan_param.FLOW_STRIP_CONTOUR_AREA_MAX, 3000, trackbar_callback1);
            createTrackbar("AREA_MIN", "strip_param", &fan_param.FLOW_STRIP_CONTOUR_AREA_MIN, 3000, trackbar_callback1);
            createTrackbar("LENGTH_MAX", "strip_param", &fan_param.FLOW_STRIP_CONTOUR_LENGTH_MAX, 150, trackbar_callback1);
            createTrackbar("LENGTH_MIN", "strip_param", &fan_param.FLOW_STRIP_CONTOUR_LENGTH_MIN, 150, trackbar_callback1);
            createTrackbar("WIDTH_MAX", "strip_param", &fan_param.FLOW_STRIP_CONTOUR_WIDTH_MAX, 80, trackbar_callback1);
            createTrackbar("WIDTH_MIN", "strip_param", &fan_param.FLOW_STRIP_CONTOUR_WIDTH_MIN, 80, trackbar_callback1);
            createTrackbar("HW_RATIO_MAX", "strip_param", &fan_param.FLOW_STRIP_CONTOUR_HW_RATIO_MAX, 1000, trackbar_callback1);
            createTrackbar("HW_RATIO_MIN", "strip_param", &fan_param.FLOW_STRIP_CONTOUR_HW_RATIO_MIN, 1000, trackbar_callback1);
            createTrackbar("AREA_RATIO_MIN", "strip_param", &fan_param.FLOW_STRIP_CONTOUR_AREA_RATIO_MIN, 100, trackbar_callback1);
            createTrackbar("INTERSETION", "strip_param", &fan_param.FLOW_STRIP_CONTOUR_INTERSETION_AREA_MIN, 500, trackbar_callback1);
            break;
         }
        case 5:
        {
            namedWindow("R_param", WINDOW_NORMAL);
            createTrackbar("AREA_MAX", "R_param", &fan_param.CENTER_R_CONTOUR_AREA_MAX, 3000, trackbar_callback1);
            createTrackbar("AREA_MIN", "R_param", &fan_param.CENTER_R_CONTOUR_AREA_MIN, 3000, trackbar_callback1);
            createTrackbar("LENGTH_MAX", "R_param", &fan_param.CENTER_R_CONTOUR_LENGTH_MAX, 150, trackbar_callback1);
            createTrackbar("LENGTH_MIN", "R_param", &fan_param.CENTER_R_CONTOUR_LENGTH_MIN, 150, trackbar_callback1);
            createTrackbar("WIDTH_MAX", "R_param", &fan_param.CENTER_R_CONTOUR_WIDTH_MAX, 80, trackbar_callback1);
            createTrackbar("WIDTH_MIN", "R_param", &fan_param.CENTER_R_CONTOUR_WIDTH_MIN, 80, trackbar_callback1);
            createTrackbar("HW_RATIO_MAX", "R_param", &fan_param.CENTER_R_CONTOUR_HW_RATIO_MAX, 1000, trackbar_callback1);
            createTrackbar("HW_RATIO_MIN", "R_param", &fan_param.CENTER_R_CONTOUR_HW_RATIO_MIN, 1000, trackbar_callback1);
            createTrackbar("AREA_RATIO_MIN", "R_param", &fan_param.CENTER_R_CONTOUR_AREA_RATIO_MIN, 100, trackbar_callback1);
            createTrackbar("INTERSETION", "R_param", &fan_param.CENTER_R_CONTOUR_INTERSETION_AREA_MIN, 300, trackbar_callback1);
            break;
         }
        case 6:
        {
            namedWindow("Predict_param", WINDOW_NORMAL);
            createTrackbar("SPEED_MID_VAL", "Predict_param", &fan_param.PRE_SPEED_OFFSET, 200, trackbar_callback1);
            createTrackbar("SPEED_RATIO", "Predict_param", &fan_param.PRE_SPEED_RATIO, 300, trackbar_callback1);
            createTrackbar("SLOPE_MAX_VAL", "Predict_param", &fan_param.PRE_SLOPE_SACLE, 300, trackbar_callback1);
            createTrackbar("SLOPE_OFFSET", "Predict_param", &fan_param.PRE_SLOPE_OFFSET, 500, trackbar_callback1);
            createTrackbar("SPEED_PROBABILITY", "Predict_param", &fan_param.PRE_SPEED_PROBABILITY, 100, trackbar_callback1);
            createTrackbar("SLOPE_PROBABILITY", "Predict_param", &fan_param.PRE_SLOPE_PROBABILITY, 100, trackbar_callback1);
            createTrackbar("TIME_PROBABILITY", "Predict_param", &fan_param.PRE_TIME_PROBABILITY, 100, trackbar_callback1);
            createTrackbar("SECTION_OFFSET", "Predict_param", &fan_param.PRE_SECTION_OFFSET, 500, trackbar_callback1);
            break;
         }
        default:
            break;

    }
}
