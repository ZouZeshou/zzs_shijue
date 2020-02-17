#include "armor_detect/armor_detect.h"
#include "armor_detect/armor_param.h"
#include "algorithm/include/usr_math.h"

/* @Des:this function is to get the binary image from raw image
 * @param:initial_img--raw image
 * @param:color--the color to detect(red,blue,green)
 * @param:method--the method(BGR,HSV,YUV)
 * @param:is_debug--debug or not(if debug,reset parameter )
 * @retval: binary image
 */
void armor_detect::get_binary_img(Mat &src,color_ENUM color,GetLightBarMethod_ENUM method)
{
    Mat gray_img,subtract_img;
    switch (method)
    {
        case BGR_METHOD:
        {
            Mat bgr_img[3];
            cvtColor(src,gray_img,COLOR_BGR2GRAY);
            threshold(gray_img,gray_img,armor_param.GRAY_THRESHOLD,255,THRESH_BINARY);
            split(src,bgr_img);
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
            threshold(subtract_img,subtract_img,armor_param.BGR_THRESHOLD,255,THRESH_BINARY);
            break;
        }
        case HSV_METHOD:
        {
            cvtColor(src, src, CV_BGR2HSV);
            if(color == RED)
            {
                inRange(src, Scalar(armor_param.RED_H_MIN, armor_param.RED_S_MIN, armor_param.RED_V_MIN),
                        Scalar(armor_param.RED_H_MAX, armor_param.RED_S_MAX, armor_param.RED_V_MAX), src);
            }
            else
            {
                inRange(src, Scalar(armor_param.BLUE_H_MIN, armor_param.BLUE_S_MIN, armor_param.BLUE_V_MIN),
                        Scalar(armor_param.BLUE_H_MAX, armor_param.BLUE_S_MAX, armor_param.BLUE_V_MAX), src);
            }
            break;
        }
        case YUV_METHOD:
        {
            cvtColor(src, src, CV_BGR2YCrCb);
            if(color == RED)
            {
                inRange(src, Scalar(armor_param.RED_Y_MIN, armor_param.RED_CR_MIN, armor_param.RED_CB_MIN),
                        Scalar(armor_param.RED_Y_MAX, armor_param.RED_CR_MAX, armor_param.RED_CB_MAX), src);
            }
            else
            {
                inRange(src, Scalar(armor_param.BLUE_Y_MIN, armor_param.BLUE_CR_MIN, armor_param.BLUE_CB_MIN),
                        Scalar(armor_param.BLUE_Y_MAX, armor_param.BLUE_CR_MAX, armor_param.BLUE_CB_MAX), src);
            }
            break;
        }
    }
    if (m_is_show_img)
    {
        namedWindow("binary", WINDOW_NORMAL);
        imshow("binary", src);
    }
}
