#include <thread>
#include <chrono>
#include <ctime>
#include <time.h>
#include "main/thread.h"
#include "plot/qcustomplot.h"
#include "plot/mainwindow.h"
double plot_y1,plot_y2,plot_y3,plot_y4,plot_y5,plot_y6,plot_y7,plot_y8;
Mat plot_frame;
/* @Des:this function is thread of get image
 * @param:
 * @retval:void
 */
void task::get_image()
{
    c_frame.turn_on_videowriter(IS_WRITE_VIDEO);
    c_frame.choose_frame_src(IS_FROM_VIDEO,VIDEO_READ_PATH);
    while(1)
    {
        micro_time time_now = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
        /***** this part is to get the picture  *********************/
        if(IS_USE_PICTRUE)
            m_frame = imread(PICTURE_PATH);/*1440 840*/
        else
            m_frame = c_frame.get_frame();
        c_frame.video_to_picture(m_frame,PICTURE_SAVE_PATH,20,IS_TRANS_VIDEO_PICTURE);
        if(!m_frame.empty())
            c_frame.get_img_fps();
        this_thread::sleep_until(time_now+chrono::microseconds(45000));
    }
}
/* @Des:this function is to thread of deal image
 * @param:
 * @retval:void
 */
int task::deal_image()
{
    WatchDog dog("/home/ubuntu/ZZS_vision/RM20_infantry_vision/watchdog_fifo");
    cout<<"openwatchdog success"<<endl;
    dog.feed();
    while(1)
    {
        micro_time time_now = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());//get the start time
        Mat src = m_frame.clone();
        if(src.empty())
        {
            //cout<<"no picture on current frame"<<endl;
            continue;
        }
        /******* get the data from MCU**************************************/
        if(IS_RESET_MODE)
        {
            s_detect.mode = DEFAULT_MODE;
            s_detect.color = DEFAULT_COLOR;
        }
        else
        {
            #ifndef NO_SERIAL_PORT
            c_interface.deal_msg_from_MCU();
            s_detect = c_interface.m_detect;
            #endif
        }
        /******image process to find target *******************************/
        if( s_detect.mode == small_fan||s_detect.mode == big_fan)
        {/*attcak fanwheel*/
            if(s_detect.is_mode_change)
            {
                c_frame.c_hik_camera.HIKsetExposure(c_frame.c_hik_camera.HIKhandle_,FAN_EXPOSURE);
                c_fan.init();
            }
            else
            {
                s_send.is_find_target = c_fan.run(src,s_detect.mode,s_detect.color,YUV_METHOD,IS_SHOW_PICTURE);
                s_send.adjustX.f = c_fan.m_shoot_point.x;
                s_send.adjustY.f = c_fan.m_shoot_point.y;
                s_send.adjustZ.f = 7000;
            }
        }
        else
        {/*attack robot*/
            if(s_detect.is_mode_change)
            {
                c_frame.c_hik_camera.HIKsetExposure(c_frame.c_hik_camera.HIKhandle_,ROBOT_EXPOSURE);
                c_armor_detect.init();
            }
            else
            {
                s_send.is_find_target = c_armor_detect.run(src,s_detect.color,YUV_METHOD,IS_SHOW_PICTURE);
                s_send.adjustX.f = c_armor_detect.m_target_center.x;
                s_send.adjustY.f = c_armor_detect.m_target_center.y;
                s_send.adjustZ.f = c_armor_detect.m_target_center.z;
                s_send.is_big_armor = c_armor_detect.m_targetinfo.TargetType;
                s_send.trans_ratio.f = c_armor_detect.m_targetinfo.Trans_ratio;
            }
        }
        /*****************calculate frame ********************************************************/
        c_frame.calculate_framerate(&s_send);
        /*******this part is to send data to MCU*******************************************/
        #ifndef NO_SERIAL_PORT
        c_interface.send_msg_to_MCU(s_send);
        #endif
        /************* feed dog ******************************************************************/
        if(c_frame.s_main_time.fps  > 50)
            dog.feed();
        /******** this part is for debug ******************************************************/
        if(IS_SHOW_TARGRT_DATA)
        {
            cout<<"x:"<<s_send.adjustX.f<<" y:"<<s_send.adjustY.f<<" z:"<<s_send.adjustZ.f<<endl;
            cout<<"is find target"<<s_send.is_find_target<<endl;
        }
        if(m_plot_flag)
        {
            plot_y1 = static_cast<double>(c_fan.m_last_sec);
            plot_y2 = static_cast<double>(c_fan.m_angle_last);
//            plot_y3 = static_cast<double>(c_fan.m_angle_now);
//            plot_y4 = static_cast<double>(c_fan.m_test_angle2);
            plot_y5 = static_cast<double>(c_fan.m_state*10);
            plot_y6 = static_cast<double>(c_fan.m_predict_angle);
            plot_y7 = static_cast<double>(c_fan.m_test_angle1);
            plot_y8 = static_cast<double>(c_fan.m_test_angle2);
//            cout<<"c_fan.m_state"<<c_fan.m_state<<endl;
            if(s_detect.mode == robot)
                plot_frame = c_armor_detect.m_debug_img;
            else
                plot_frame = c_fan.m_debug_img;

        }
        if(IS_CREATE_WINDOW)
            waitKey(1);
        this_thread::sleep_until(time_now+chrono::milliseconds(20));//delay_until
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

        this_thread::sleep_until(time_now+chrono::milliseconds(10));
    }
}
/* @Des:this function is the thread of plot
 * @param:
 * @retval:void
 */
int task::plot()
{
    int argc;
    char **argv = nullptr;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    m_plot_flag = true;
    return a.exec();
}
