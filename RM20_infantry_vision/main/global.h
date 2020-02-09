#ifndef GLOBAL_H
#define GLOBAL_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

/*hik-camera*/
#define MAX_IMAGE_DATA_SIZE   (40*1024*1024)
#define pixelWidth 1440
#define pixelHeight 840
/*save video*/
#define PICTURE_PATH     "/home/zzs/ZZS_vision/RM20_infantry_vision/picture_save/d12.PNG"
#define PICTURE_SAVE_PATH  "/home/zzs/ZZS_vision/RM20_infantry_vision/picture_save/d"
#define VIDEO_READ_PATH  "/media/zzs/225C-F971/机器人/videosave1/orange_energy.avi"
#define VIDEO_WRITE_PATH  "/home/ubuntu/Desktop/ZZS-visiondata/zzs_get_vedio/video_save/a"
#define VIDEO_SAVE_COLUMN   1440
#define VIDEO_SAVE_ROW      840
/*armor detext*/
#define pixelCenterX       720
#define pixelCenterY       480

typedef struct
{
    double previousTime;
    double currentTime;
    int fps;
    int valid_fps;
    int send_fps;
}TimeParam;
typedef struct
{
    double    time_last;
    double    time_now;
    float     polar_angle_now;
    float     polar_angle_last;
    float     spd;
}fan_spd_t;
typedef enum{
    BGR_METHOD = 0,
    HSV_METHOD = 1,
    YUV_METHOD = 2
}GetLightBarMethod_ENUM;

typedef enum{
    RED = 0,
    BLUE = 1,
    GREEN = 2
}color_ENUM;
typedef enum{
    BigArmor = 0,
    SmallArmor = 1,
    NoneArmor = 6
}TargetTypeEnum;

typedef enum{
    LoseTarget = 0,
    FindTarget = 1
}TargetTrackerStatusEnum;

typedef struct{
    TargetTypeEnum                  TargetType;//0 bigarmor 1 small armor
    TargetTrackerStatusEnum         TargetTrackerStatus;
    float                           Trans_ratio;//trans the pixel distance to the real distance
}TargetInfo;
typedef enum{
    fanwheel = 0,
    robot    = 1
} e_vision_mode_t;
typedef union{
    float f;
    unsigned char uc[4];
}float2uchar;

typedef union{
    short int d_16;
    unsigned char uc[2];
}int16uchar;
typedef struct{
    float2uchar adjustX;
    float2uchar adjustY;
    float2uchar adjustZ;

    float2uchar trans_ratio;

    int16uchar  valid_fps;
    int16uchar  small_armor_flag;
}s_vision_data;
typedef struct{
    uint8_t id;
    uint8_t level;
    uint8_t mode;
    uint8_t fanwheel_dir;
}s_receive_data_t;
typedef struct{
    GetLightBarMethod_ENUM method;
    color_ENUM             color;
    e_vision_mode_t        mode;
    e_vision_mode_t        last_mode;
}s_detect_t;
typedef enum{
   CCW = 2,
   CW  = 1
}e_rotation_t;
#endif

