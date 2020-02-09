#ifndef GET_FRAME_H
#define GET_FRAME_H

#include "main/global.h"
#include "get_frame/camera/include/hikcamera.h"
#include "get_frame/debug/debug_settings.h"

class frame
{
public:
    frame();
    ~frame();
    void turn_on_videowriter(bool is_turn_on);
    void choose_frame_src(bool is_camera,string video_read_path);
    Mat  get_frame();
    void get_img_fps();
    void calculate_framerate(float center_x);
    bool video_to_picture(Mat frame,string picture_save_path,uint8_t time_gap,bool is_transfer);
public:
    TimeParam s_main_time;
    TimeParam s_img_time;
    HIKCamera c_hik_camera;
private:

    debug_Settings c_debug;
    VideoCapture c_video;


    bool m_is_camera;
    string m_video_read_path;
    Mat m_frame;
};

#endif
