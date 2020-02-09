#ifndef HIKCAMERA_H
#define HIKCAMERA_H
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include<linux/videodev2.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#include "opencv2/opencv.hpp"
#include "get_frame/camera/include/MvCameraControl.h"
#include "main/global.h"
#include <mutex>
#include <thread>

#define GET_IMAGE_BY_CALLBACK

using namespace cv;

class debug_Settings;

class HIKCamera
{
public:
    HIKCamera(void);
    ~HIKCamera();
    void SettingParas(void);
    bool getFrame();
    bool getState();
    //HKVision
    bool HIKsetFrameRate(void *handle, float value); //采集帧率
    bool HIKsetBalance(void *handle, unsigned int value);  //自动白平衡
    bool HIKsetBrightness(void *handle, unsigned int value); //图像亮度
    bool HIKsetExposureMode(void *handle, unsigned int value); //自动曝光
    bool HIKsetExposure(void *handle, unsigned int value); //自动曝光
    bool HIKsetGainMode(void *handle, unsigned int value); //增益模式
    bool HIKsetGain(void *handle, float value);  //增益值
    bool HIKgetFrameRate();
    bool Hik_Adjust_Resolution(void *handle,  unsigned int height, unsigned int width );
    bool state_;
    debug_Settings* psettings_;
    cv::VideoWriter videowriter_;
    int device_;

    //HIKVision
    int HIKnRet_;
    void* HIKhandle_;
    MV_CC_DEVICE_INFO_LIST HIKstDeviceList_;
    unsigned int HIKnIndex_;
    MV_CC_DEVICE_INFO* HIKpDeviceInfo_;
    float HIKfValue_ ;
    unsigned int HIKOffsetX;
    unsigned int HIKOffsetY;
    unsigned int HIKwidth_ ;
    unsigned int HIKheight_ ;
    MV_FRAME_OUT_INFO_EX HIKstImageInfo_;
    unsigned char* HIKpData_;
    unsigned int HIKnDataSize_;
    unsigned char *pDataForBayer = NULL;
    MVCC_FLOATVALUE*  frame_test;
    MVCC_ENUMVALUE*   PixelEnum;
    unsigned char *pDataForBGR = NULL;
    unsigned char *pDataForSaveImage = NULL;

    Mat raw_image;
private:
    cv::FileStorage mfs;
};

//extern MVCC_ENUMVALUE*   PixelEnum;
#endif // HIKCAMERA_H
