/*this file is some function of hik_camera*/

#include "get_frame/camera/include/hikcamera.h"

Mat Call_back_image;

HIKCamera::HIKCamera()
{
    cout << "HIK_camera is ready !!!" << endl;
}

HIKCamera::~HIKCamera()
{
    // end grab image
    HIKnRet_ = MV_CC_StopGrabbing(HIKhandle_);
    if (MV_OK != HIKnRet_)
    {
        printf("MV_CC_StopGrabbing fail! nRet [%x]\n", HIKnRet_);
        state_ = false;
        return;
    }

    // destroy handle
    HIKnRet_ = MV_CC_DestroyHandle(HIKhandle_);
    if (MV_OK != HIKnRet_)
    {
        printf("MV_CC_DestroyHandle fail! nRet [%x]\n", HIKnRet_);
        state_ = false;
        return;
    }
    cout<<"~HIKCamera ok!"<<endl;
}
void __stdcall ImageCallBackEx(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{
    static int i = 0;
    if (pFrameInfo)
    {
        Call_back_image=Mat(pFrameInfo->nHeight,pFrameInfo->nWidth,CV_8U,pData);
//        printf("GetOneFrame, Width[%d], Height[%d], nFrameNum[%d]\n",
//            pFrameInfo->nWidth, pFrameInfo->nHeight, pFrameInfo->nFrameNum);
    }
}
/*adjust resolution function begin*/
bool HIKCamera::Hik_Adjust_Resolution(void *handle,  unsigned int height, unsigned int width )
{
    HIKnRet_ = MV_CC_StopGrabbing(HIKhandle_);
    if (MV_OK != HIKnRet_)
    {
        printf("MV_CC_StopGrabbing fail! nRet [%x]\n", HIKnRet_);
        state_ = false;
        return 0;
    }
    HIKnRet_ = MV_CC_DestroyHandle(HIKhandle_);
    if (MV_OK != HIKnRet_)
    {
        printf("MV_CC_DestroyHandle fail! nRet [%x]\n", HIKnRet_);
        state_ = false;
        return 0;
    }

    //open camera and get streaming
    HIKnRet_ = MV_OK;
    HIKhandle_ = NULL;
    memset(&HIKstDeviceList_, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    // enum device
    HIKnRet_ = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &HIKstDeviceList_);
    if (MV_OK != HIKnRet_)
    {
        printf("MV_CC_EnumDevices fail! HIKnRet_ [%x]\n", HIKnRet_);
        state_= false;
        return 0;
    }
    if (HIKstDeviceList_.nDeviceNum > 0)
    {
        for (int i = 0; i < HIKstDeviceList_.nDeviceNum; i++)
        {
            printf("[device %d]:\n", i);
            MV_CC_DEVICE_INFO* HIKpDeviceInfo_ = HIKstDeviceList_.pDeviceInfo[i];
            if (NULL == HIKpDeviceInfo_)
            {
                cout<<"NULL == HIKpDeviceInfo_"<<endl;
                state_= false;
                return 0;
            }
        }
    }
    else
    {
        printf("Find No Devices!\n");
        state_= false;
        return 0;
    }

    HIKnIndex_ = 0;

    // select device and create HIKhandle_
    HIKnRet_ = MV_CC_CreateHandle(&HIKhandle_, HIKstDeviceList_.pDeviceInfo[HIKnIndex_]);
    if (MV_OK != HIKnRet_){
        printf("MV_CC_CreateHandle fail! HIKnRet_ [%x]\n", HIKnRet_);
        state_= false;
        return 0;
    }

    // open device
    HIKnRet_ = MV_CC_OpenDevice(HIKhandle_);
    if (MV_OK != HIKnRet_){
        printf("MV_CC_OpenDevice fail! HIKnRet_ [%x]\n", HIKnRet_);
        state_= false;
        return 0;
    }
    HIKfValue_=240;
    HIKnRet_=MV_CC_SetFrameRate(HIKhandle_,HIKfValue_);
    HIKOffsetX=720-width/2;
    HIKOffsetY=540-height/2;
    MV_CC_SetWidth(HIKhandle_,width);
    MV_CC_SetHeight(HIKhandle_,height);
    MV_CC_SetAOIoffsetX(HIKhandle_,HIKOffsetX);
    MV_CC_SetAOIoffsetY(HIKhandle_,HIKOffsetY);
    //set Balance
    HIKsetBalance(HIKhandle_,1);
    //setExpose
    HIKsetExposureMode(HIKhandle_, 0);
    HIKsetExposure(HIKhandle_,1000);      //sf:5000       bf:10000
    //setGain
    HIKsetGainMode(HIKhandle_,0);
    HIKsetGain(HIKhandle_,0);
    //Set Pixel Format
    MV_CC_SetPixelFormat(HIKhandle_, PixelType_Gvsp_BayerRG8);
    //MV_CC_SetBrightness(HIKhandle_,60.0f);
//    MV_CC_SetGammaSelector(HIKhandle_,1);
//    MV_CC_SetGamma(HIKhandle_, 50.0f);
    HIKnRet_ = MV_CC_SetEnumValue(HIKhandle_, "TriggerMode", 0);
    if (MV_OK != HIKnRet_)
    {
        printf("MV_CC_SetTriggerMode fail! HIKnRet_ [%x]\n", HIKnRet_);
        state_= false;
        return 0;
    }

#ifdef GET_IMAGE_BY_CALLBACK
    // 注册抓图回调
    // register image callback
    HIKnRet_ = MV_CC_RegisterImageCallBackEx(HIKhandle_, ImageCallBackEx, HIKhandle_);
    if (MV_OK != HIKnRet_)
    {
        printf("MV_CC_RegisterImageCallBackEx fail! nRet [%x]\n", HIKnRet_);
        return 0;
    }
#endif

    // start grab image
    HIKnRet_ = MV_CC_StartGrabbing(HIKhandle_);
    if (MV_OK != HIKnRet_)
    {
        printf("MV_CC_StartGrabbing fail! HIKnRet_ [%x]\n", HIKnRet_);
        state_= false;
        return 0;
    }

    MV_FRAME_OUT_INFO_EX HIKstImageInfo_ = {0};
    memset(&HIKstImageInfo_, 0, sizeof(MV_FRAME_OUT_INFO_EX));
    HIKpData_ = (unsigned char *)malloc(sizeof(unsigned char) * MAX_IMAGE_DATA_SIZE);
    HIKnDataSize_ = MAX_IMAGE_DATA_SIZE;
    return 1;
}
/*adjust resolution function end*/



void HIKCamera::SettingParas(void)
{
    //open camera and get streaming
    HIKnRet_ = MV_OK;
    HIKhandle_ = NULL;
    memset(&HIKstDeviceList_, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    // enum device
    HIKnRet_ = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &HIKstDeviceList_);
    if (MV_OK != HIKnRet_)
    {
        printf("MV_CC_EnumDevices fail! HIKnRet_ [%x]\n", HIKnRet_);
        state_= false;
        return;
    }
    if (HIKstDeviceList_.nDeviceNum > 0)
    {
        for (int i = 0; i < HIKstDeviceList_.nDeviceNum; i++)
        {
            printf("[device %d]:\n", i);
            MV_CC_DEVICE_INFO* HIKpDeviceInfo_ = HIKstDeviceList_.pDeviceInfo[i];
            if (NULL == HIKpDeviceInfo_)
            {
                cout<<"NULL == HIKpDeviceInfo_"<<endl;
                state_= false;
                return;
            }
        }
    }
    else
    {
        printf("Find No Devices!\n");
        state_= false;
        return;
    }

    HIKnIndex_ = 0;

    // select device and create HIKhandle_
    HIKnRet_ = MV_CC_CreateHandle(&HIKhandle_, HIKstDeviceList_.pDeviceInfo[HIKnIndex_]);
    if (MV_OK != HIKnRet_)
    {
        printf("MV_CC_CreateHandle fail! HIKnRet_ [%x]\n", HIKnRet_);
        state_= false;
        return;
    }

    // open device
    HIKnRet_ = MV_CC_OpenDevice(HIKhandle_);
    if (MV_OK != HIKnRet_)
    {
        printf("MV_CC_OpenDevice fail! HIKnRet_ [%x]\n", HIKnRet_);
        state_= false;
        return;
    }

    HIKfValue_=300;
    HIKnRet_=MV_CC_SetFrameRate(HIKhandle_,HIKfValue_);
    HIKwidth_=pixelWidth;
    HIKheight_=pixelHeight;
    HIKOffsetX=720-pixelWidth/2;
    HIKOffsetY=540-pixelHeight/2;
    MV_CC_SetWidth(HIKhandle_,HIKwidth_);
    MV_CC_SetHeight(HIKhandle_,HIKheight_);
    MV_CC_SetAOIoffsetX(HIKhandle_,HIKOffsetX);
    MV_CC_SetAOIoffsetY(HIKhandle_,HIKOffsetY);
    //set Balance
    HIKsetBalance(HIKhandle_,1);
    //setExpose
    HIKsetExposureMode(HIKhandle_, 0);
    HIKsetExposure(HIKhandle_,1000);      //sf:5000       bf:10000
    //setGain
    HIKsetGainMode(HIKhandle_,0);
    HIKsetGain(HIKhandle_,0);
    //Set Pixel Format
    MV_CC_SetPixelFormat(HIKhandle_, PixelType_Gvsp_BayerRG8);
    //MV_CC_SetBrightness(HIKhandle_,60.0f);
//    MV_CC_SetGammaSelector(HIKhandle_,1);
//    MV_CC_SetGamma(HIKhandle_, 50.0f);
    HIKnRet_ = MV_CC_SetEnumValue(HIKhandle_, "TriggerMode", 0);
    if (MV_OK != HIKnRet_){
        printf("MV_CC_SetTriggerMode fail! HIKnRet_ [%x]\n", HIKnRet_);
        state_= false;
        return;
    }
#ifdef GET_IMAGE_BY_CALLBACK
    // 注册抓图回调
    // register image callback
    HIKnRet_ = MV_CC_RegisterImageCallBackEx(HIKhandle_, ImageCallBackEx, HIKhandle_);
    if (MV_OK != HIKnRet_)
    {
        printf("MV_CC_RegisterImageCallBackEx fail! nRet [%x]\n", HIKnRet_);
        return;
    }
#endif

    // start grab image
    HIKnRet_ = MV_CC_StartGrabbing(HIKhandle_);
    if (MV_OK != HIKnRet_){
        printf("MV_CC_StartGrabbing fail! HIKnRet_ [%x]\n", HIKnRet_);
        state_= false;
        return;
    }
    MV_FRAME_OUT_INFO_EX HIKstImageInfo_ = {0};
    memset(&HIKstImageInfo_, 0, sizeof(MV_FRAME_OUT_INFO_EX));
    HIKpData_ = (unsigned char *)malloc(sizeof(unsigned char) * MAX_IMAGE_DATA_SIZE);
    HIKnDataSize_ = MAX_IMAGE_DATA_SIZE;



}

/* the returns value is to judge the camera is offline */
bool HIKCamera::getFrame()
{
#ifndef GET_IMAGE_BY_CALLBACK
    /* the 100000000 value can not be smaller ! */
    HIKnRet_ = MV_CC_GetImageForBGR(HIKhandle_, HIKpData_, HIKnDataSize_, &HIKstImageInfo_,100000000);
    if(MV_OK != HIKnRet_)
    {
        printf("MV_CC_GetImageForBGR fail! nRet [%x]\n", HIKnRet_);
        exit(1);
        return 1;
    }
    else
    {
        raw_image=Mat(HIKstImageInfo_.nHeight,HIKstImageInfo_.nWidth,CV_8UC3,HIKpData_);
        //imshow("Call_back_image", raw_image);
        return 0;
    }

#else
    if(Call_back_image.size().height != 0) {
        cvtColor(Call_back_image, raw_image, CV_BayerBG2BGR);
//        imshow("Call_back_image", raw_image);
    }
#endif
}

bool HIKCamera::getState()
{
    return state_;
}

//set cameraparam
bool HIKCamera::HIKsetFrameRate(void *handle, float value)
{
    int nRet;
    nRet = MV_CC_SetFrameRate(handle, value);
    if(nRet != 0)
    {
        return false;
    }
    return true;
}

bool HIKCamera::HIKsetBalance(void *handle, unsigned int value)
{
    /*
      value = 0 �ر�
      value = 1 ����
      value = 2 һ��
    */
    int nRet;
    nRet = MV_CC_SetBalanceWhiteAuto(handle, value);
    if(nRet != 0){
        return false;
    }
    return true;
}

bool HIKCamera::HIKsetBrightness(void *handle, unsigned int value)
{
    int nRet;
    nRet = MV_CC_SetBrightness(handle, value);
    if(nRet != 0){
        return false;
    }
    return true;
}

bool HIKCamera::HIKsetExposureMode(void *handle, unsigned int value)
{
    /*
      value = 0 �ر�
      value = 1 һ��
      value = 2 ����
    */
    int nRet;
    nRet = MV_CC_SetExposureAutoMode(handle, value);
    if(nRet != 0){
        return false;
    }
    return true;
}

bool HIKCamera::HIKsetExposure(void *handle, unsigned int value)
{
    int nRet;
    nRet = MV_CC_SetExposureTime(handle,value);
    if(nRet != 0){
        return false;
    }
    return true;
}

bool HIKCamera::HIKsetGainMode(void *handle, unsigned int value)
{
    /*
      value = 0 �ر�
      value = 1 һ��
      value = 2 ����
    */
    int nRet;
    nRet = MV_CC_SetGainMode(handle, value);
    if(nRet != 0){
        return false;
    }
    return true;
}

bool HIKCamera::HIKsetGain(void *handle, float value)
{
    int nRet;
    nRet = MV_CC_SetGain(handle, value);
    if(nRet != 0){
        return false;
    }
    return true;
}
    //bool HIKgetFrameRate();
bool HIKCamera::HIKgetFrameRate()
{
    int nRet;
    nRet = MV_CC_GetPixelFormat(HIKhandle_, PixelEnum);
    if(nRet != 0){
        return false;
    }
    //MV_CC_SetPixelFormat(HIKhandle_, PixelType_Gvsp_BayerBG8);
//    if( MV_CC_GetPixelFormat(HIKhandle_, PixelEnum))
    //printf( "fcurvalue:%d " ,&PixelEnum->nCurValue);

    return true;

}

