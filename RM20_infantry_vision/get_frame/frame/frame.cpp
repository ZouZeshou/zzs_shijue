/*this file is for get frame and videowrite,also include calculate framerate
* the first part
* the file is should include  "hikcamera.h" and "debug_setting.h"
*/
#include "get_frame/frame/frame.h"

frame::frame(){}
frame::~frame(){}

/* @Des:this function is to turn on the videowriter
 * @param:is_turn_on: turn on videowriter--true
 * @retval:void
 */
void frame::turn_on_videowriter(bool is_turn_on)
{
    c_debug.set_is_videosavemode(is_turn_on);
    c_debug.set_videowriter_path(VIDEO_WRITE_PATH,VIDEO_SAVE_COLUMN,VIDEO_SAVE_ROW);
}

/* @Des:this function is to choose the frame source(from camera or local video)
 * @param:is_video -- frame from camera or video(camera --false video--true)
 * @param:video_read_path -- the path of local video which used
 * @retval:void
 */
void frame::choose_frame_src(bool is_video,string video_read_path)
{
    m_is_video = is_video;
    m_video_read_path = video_read_path;
    cout << "frame source is from video :"<< m_is_video <<endl;

    if(!m_is_video)
    {
        c_hik_camera.SettingParas();
    }
    else
    {
       c_video.open(video_read_path);
       if(!c_video.isOpened())
       {
           cout<<"video read fail"<<endl;
       }
    }
}

/* @Des:this function is to get the frame to m_frame
 * @param:is_video -- frame frome camera or video(camera --true video--false)
 * @param:video_read_path -- the path of local video which used
 * @retval:void
 */
Mat frame::get_frame()
{
    if(!m_is_video)
    {
        c_hik_camera.getFrame();
        m_frame = c_hik_camera.raw_image;
    }
    else
    {
        c_video >> m_frame;
    }
    c_debug.write_video(m_frame);
    return m_frame;
}
/* @Des:this function is to calculate the framerate of get image
 * @param:no
 * @retval:void
 */
void frame::get_img_fps()
{
    static int  init_mark = 0;
    if(init_mark==0)
    {
        s_img_time.currentTime = 0;
        s_img_time.previousTime = 0;
        s_img_time.fps = 0;
        s_img_time.send_fps = 0;
        s_img_time.valid_fps = 0;
        init_mark = 1;
    }
    else
    {
        s_img_time.fps++;
        s_img_time.currentTime = static_cast<double>(getTickCount());
        if((s_img_time.currentTime - s_img_time.previousTime) / getTickFrequency() > 1.0)
        {
            s_img_time.previousTime = s_img_time.currentTime;
            s_img_time.send_fps = s_img_time.valid_fps ;
            std::cout << "img_fps: " << s_img_time.fps << std::endl;
            s_img_time.fps = 0;
        }
    }
}
/* @Des:this function is to calculate the framerate of main function
 * @param:no
 * @retval:void
 */
int16_t frame::calculate_framerate(bool is_find_target)
{
    static int  init_mark = 0;
    if(init_mark==0)
    {
        s_main_time.currentTime = 0;
        s_main_time.previousTime = 0;
        s_main_time.fps = 0;
        s_main_time.send_fps = 0;
        s_main_time.valid_fps = 0;
        init_mark = 1;
    }
    else
    {
        s_main_time.fps++;
        if(is_find_target)
            s_main_time.valid_fps++;
        s_main_time.currentTime = static_cast<double>(getTickCount());
        if((s_main_time.currentTime - s_main_time.previousTime) / getTickFrequency() > 1.0)
        {
            s_main_time.previousTime = s_main_time.currentTime;
            s_main_time.send_fps = s_main_time.valid_fps ;
            std::cout << "fps: " << s_main_time.fps << std::endl;
            s_main_time.fps = 0;
            std::cout << "valid_fps: " << s_main_time.valid_fps << std::endl;
            s_main_time.valid_fps = 0;
        }
    }
    return static_cast<int16_t>(s_main_time.send_fps);
}
/* @Des:this function is to transfer video to picture
 * @param:is_video -- frame frome camera or video(camera --true video--false)
 * @param:video_read_path -- the path of local video which used
 * @retval:void
 */
bool frame::video_to_picture(Mat frame,string picture_save_path,uint8_t time_gap,bool is_transfer)
{
    static uint8_t cnt=0;
    static uint picture_cnt=0;
    if(is_transfer && !frame.empty())
    {
        if(time_gap==0) return false;
        if(cnt++ > time_gap)
        {
            cnt = 0;
            struct timeval tv;
            gettimeofday(&tv, NULL);
            //string picture_name = picture_save_path+to_string(tv.tv_sec * 1000000 + tv.tv_usec)+".PNG";
            string picture_name = picture_save_path+to_string(picture_cnt++)+".PNG";
            imwrite(picture_name,frame);
            return true;
        }
    }
    return false;
}
