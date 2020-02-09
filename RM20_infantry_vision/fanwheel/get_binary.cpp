#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
#include "algorithm/include/usr_math.h"
#include "fanwheel/fan_param.h"
/* @Des:this function is transfer 3 channel image to 1 single channel image
 * @param:Mat src--raw image
 * @retval:
 */
Mat fan::get_binary(Mat &src, color_ENUM color,GetLightBarMethod_ENUM method)
{
    switch(method)
    {
        case BGR_METHOD:
        {
           /* Y = 0.299R + 0.587G + 0.114B
            U = -0.147R - 0.289G + 0.436B
            V = 0.615R - 0.515G - 0.100B */
//            Mat bgr_img[3],subtract_img,yuv_img[3],yuv_imgU3;
//            split(src,bgr_img);
//            yuv_img[0] = 0.299*bgr_img[2] + 0.587*bgr_img[1] + 0.114*bgr_img[0];
//            yuv_img[1] = -0.147*bgr_img[2] - 0.289*bgr_img[1] + 0.436*bgr_img[0];
//            yuv_img[2] = 0.615*bgr_img[2] - 0.515*bgr_img[1] - 0.100*bgr_img[0];
//            merge(yuv_img,3,yuv_imgU3);
//            imshow("merge_img",yuv_imgU3);
//            switch(color)
//            {
//                case RED:
//                {

//                    inRange(yuv_imgU3, Scalar(fan_param.RED_Y_MIN, fan_param.RED_CR_MIN, fan_param.RED_CB_MIN),
//                        Scalar(fan_param.RED_Y_MAX, fan_param.RED_CR_MAX, fan_param.RED_CB_MAX), src);
//                    break;
//                }
//                case BLUE:
//                {
//                    inRange(yuv_imgU3, Scalar(fan_param.BLUE_Y_MIN, fan_param.BLUE_CR_MIN, fan_param.BLUE_CB_MIN),
//                        Scalar(fan_param.BLUE_Y_MAX, fan_param.BLUE_CR_MAX, fan_param.BLUE_CB_MAX), src);
//                    break;
//                }
//                default:
//                    break;
//            }
//            threshold(subtract_img,subtract_img,fan_param.GRAY_THRESHOLD,255,THRESH_BINARY);
            cvtColor(src, src, CV_BGR2GRAY);
            threshold(src,src,fan_param.GRAY_THRESHOLD,255,THRESH_BINARY);
            break;
        }
        case HSV_METHOD:
        {
            cvtColor(src, src, CV_BGR2HSV);
            imshow("merge_img",src);
            if(color == RED)
            {
                inRange(src, Scalar(fan_param.RED_H_MIN, fan_param.RED_S_MIN, fan_param.RED_V_MIN),
                        Scalar(fan_param.RED_H_MAX, fan_param.RED_S_MAX, fan_param.RED_V_MAX), src);
            }
            else
            {
                inRange(src, Scalar(fan_param.BLUE_H_MIN, fan_param.BLUE_S_MIN, fan_param.BLUE_V_MIN),
                        Scalar(fan_param.BLUE_H_MAX, fan_param.BLUE_S_MAX, fan_param.BLUE_V_MAX), src);
            }
            break;
        }
        case YUV_METHOD:
        {
            cvtColor(src, src, CV_BGR2YCrCb);
            if(color == RED)
            {
                inRange(src, Scalar(fan_param.RED_Y_MIN, fan_param.RED_CR_MIN, fan_param.RED_CB_MIN),
                        Scalar(fan_param.RED_Y_MAX, fan_param.RED_CR_MAX, fan_param.RED_CB_MAX), src);
            }
            else
            {
                inRange(src, Scalar(fan_param.BLUE_Y_MIN, fan_param.BLUE_CR_MIN, fan_param.BLUE_CB_MIN),
                        Scalar(fan_param.BLUE_Y_MAX, fan_param.BLUE_CR_MAX, fan_param.BLUE_CB_MAX), src);
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
