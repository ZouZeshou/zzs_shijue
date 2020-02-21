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
#define PICTURE_PATH     "/home/zzs/zzs_shijue/RM20_infantry_vision/picture_save/b4.PNG"
#define PICTURE_SAVE_PATH  "/home/zzs/zzs_shijue/RM20_infantry_vision/picture_save/b"
#define VIDEO_READ_PATH  "/home/zzs/zzs_shijue/RM20_infantry_vision/video_save/test energy speed/大符(逆).avi"
#define VIDEO_WRITE_PATH  "/home/zzs/zzs_shijue/RM20_infantry_vision/video_save/a"
#define VIDEO_SAVE_COLUMN   1440
#define VIDEO_SAVE_ROW      840
/*armor detect*/
#define ROBOT_EXPOSURE     1000
#define FAN_EXPOSURE       3000
#define pixelCenterX       720
#define pixelCenterY       420
/*debug define*//*the following definition should colse when works for competition*/
#define DEBUG_PLOT
#define NO_SERIAL_PORT
/*debug constant value*//* the following definition should all be 0 when works for competition*/
#define IS_WRITE_VIDEO          0   /*is turn on videowriter*/
#define IS_TRANS_VIDEO_PICTURE  0   /*is trans video to picture*/
#define IS_FROM_VIDEO           1   /*choose the image source 0--from camera 1--from local video*/
#define IS_USE_PICTRUE          0   /*choose the picture as debug source*/
#define IS_SHOW_PICTURE         1   /*is show the debug pictrue */
#define IS_CREATE_WINDOW        0   /*is create the debug windows*/
#define IS_SHOW_TARGRT_DATA     0   /*is show the target message(x,y,z)*/
#define IS_RESET_MODE           1   /*is reset the mode by hand*/
/*******************************/
#define DEFAULT_MODE            big_fan
#define DEFAULT_COLOR           RED
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
    float     spd_kal1;
    float     spd_kal2;
    float     spd_kal3;
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
typedef struct{
    TargetTypeEnum                  TargetType;//0 big armor 1 small armor
    float                           Trans_ratio;//trans the pixel distance to the real distance
}TargetInfo;
typedef enum{
    robot        = 0,
    small_fan    = 1,
    big_fan      = 2,
    gyro         = 3
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
    uint8_t     is_big_armor;
    uint8_t     is_find_target;
}s_vision_data;
typedef struct{
    uint8_t id;
    uint8_t mode;
    //uint8_t save;
}s_receive_data_t;
typedef struct{
    GetLightBarMethod_ENUM method;
    color_ENUM             color;
    e_vision_mode_t        mode;
    e_vision_mode_t        last_mode;
    bool                   is_big_fan;
}s_detect_t;
typedef enum{
   CCW = 2,
   CW  = 1
}e_rotation_t;
extern double plot_y1,plot_y2,plot_y3,plot_y4;
extern Mat plot_frame;
#endif

