#include <thread>
#include <chrono>
#include <ctime>
#include <time.h>
#include "main/thread.h"
/* @Des:this function is thread of get image
 * @param:
 * @retval:void
 */
void task::get_image()
{
    c_frame.turn_on_videowriter(false);
    c_frame.choose_frame_src(false,VIDEO_READ_PATH);/*true--camera false--video*/
    while(1)
    {
        micro_time time_now = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
        /***** this part is to get the picture  *********************/
        //m_frame = imread(PICTURE_PATH);/*1440 840*/
        m_frame = c_frame.get_frame();
        c_frame.video_to_picture(m_frame,PICTURE_SAVE_PATH,30,false);/*true--transfer*/
        if(!m_frame.empty())
            c_frame.get_img_fps();
//        //waitKey(1);
        this_thread::sleep_until(time_now+chrono::milliseconds(40));
    }
}
/* @Des:this function is to thread of deal image
 * @param:
 * @retval:void
 */
void task::deal_image()
{
    WatchDog dog("/home/ubuntu/ZZS_vision/RM20_infantry_vision/watchdog_fifo");
    cout<<"openwatchdog success"<<endl;
    dog.feed();
    while(1)
    {
        micro_time time_now = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());//get the start time
        if(m_frame.empty())
            continue;
        /******* this part is to deal with the picture**************************************/
        s_detect.mode = fanwheel;
        s_detect.color = RED;
        if( s_detect.mode == fanwheel)
        {
            if(s_detect.last_mode != s_detect.mode)
            {
                c_frame.c_hik_camera.HIKsetExposure(c_frame.c_hik_camera.HIKhandle_,2000);
            }
            else
            {
                c_fan.run(m_frame,s_detect.color,YUV_METHOD,true);/*true--show image*/
            }
        }
        else
        {
            if(s_detect.last_mode != s_detect.mode)
            {
                c_frame.c_hik_camera.HIKsetExposure(c_frame.c_hik_camera.HIKhandle_,1000);
            }
            else
            {
                c_armor_detect.get_armor_center(m_frame,s_detect.color,HSV_METHOD,false);/*true--show image*/
            }
        }
        s_detect.last_mode = s_detect.mode;
        /*******this part is to interface with MCU*******************************************/
        //c_interface.deal_msg_from_MCU();
        //c_interface.send_msg_to_MCU(c_armor_detect.m_target_center.x,\
                                    c_armor_detect.m_target_center.y,\
                                    c_armor_detect.m_target_center.z,\
                                    c_armor_detect.m_targetinfo.TargetType,\
                                    c_armor_detect.m_targetinfo.Trans_ratio,\
                                    c_frame.s_main_time.send_fps);
       // cout<<"x="<<c_armor_detect.m_target_center.x<<"y="<< c_armor_detect.m_target_center.y<<"z="<< c_armor_detect.m_target_center.z<<endl;
        /******** this part is for debug ******************************************************/
        if(m_frame.size().height > 0)
        {
            //namedWindow("frame",WINDOW_NORMAL);
            //imshow("frame",m_frame);
        }
        else
        {
           cout<<"no picture on current frame"<<endl;
        }
        /*****************calculate frame ********************************************************/
        c_frame.calculate_framerate(c_armor_detect.m_target_center.x);
        /************* feed dog ******************************************************************/
        if(c_frame.s_main_time.fps  > 50)
            dog.feed();
        waitKey(1);
        this_thread::sleep_until(time_now+chrono::milliseconds(10));//delay_until
    }
}
/* @Des:this function is the thread of test
 * @param:
 * @retval:void
 */
void task::test()
{
    while(1)
    {
        micro_time time_now = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());

        this_thread::sleep_until(time_now+chrono::milliseconds(50));
    }
}
