/*this file is some  function for videowritting*/

#include "get_frame/debug/debug_settings.h"

debug_Settings::debug_Settings(){}
debug_Settings::~debug_Settings(){}

/* @Des:this function is to set the mode of videowriter(save or notsave)
 * @param:is_save -- save--true notsave--false
 * @retval:void
 */
void debug_Settings::set_is_videosavemode(bool is_save)
{
    m_is_save = is_save;
    cout << "video_save_mode is :" << m_is_save << endl;
}

/* @Des:this function is set video path
 * @param:save_path --the path for writting video
 * @param:image_cols -- the number of column of video
 * @param:image_rows -- the number of row of video
 * @retval:void
 */
void debug_Settings::set_videowriter_path(string save_path,int image_cols,int image_rows)/* col=1440 row=480*/
{
    if(m_is_save)
    {

        //struct timeval tv;
        //gettimeofday(&tv, NULL);
        //string video_name = save_path+to_string(tv.tv_usec)+".avi";
        // 基于当前系统的当前日期/时间
        time_t now = time(0);
        tm *ltm = localtime(&now);
        // 输出 tm 结构的各个组成部分
        string year = to_string(1900 + ltm->tm_year);
        string month = to_string(ltm->tm_mon + 1);
        string day = to_string(ltm->tm_mday);
        string hour = to_string(ltm->tm_hour);
        string minute = to_string(ltm->tm_min);
        string second = to_string(ltm->tm_sec);
        string video_name = year + "-" + month + "-" + day + "-" + hour + "-" + minute  + "-" + second + ".avi";
        c_videowrite.open(video_name,CV_FOURCC('M','J','P','G'),25.0,Size(image_cols,image_rows), true);
        if(!c_videowrite.isOpened())
        {
            cout<<"The address of VideoWriter is error!--complete reco_armor"<<endl;
            c_videowrite.release();
        }
    }
}

/* @Des:this function is to writting video to local
 * @param:frame_to_save -- the video source should be saved
 * @retval:void
 */
void debug_Settings::write_video(Mat frame_to_save)
{
    if(m_is_save)
    {
        if(c_videowrite.isOpened())
        {
            c_videowrite.write(frame_to_save);
            cout << "video is saving "  << endl;
        }
        else
        {
            cout << "the videowriter open failed"  << endl;
        }
    }
}

void debug_Settings::get_debugparam()
{
//    cout<<"addr_videowriter= "<<maddr_videowriter<<endl;
//    cout<<"image_cols= "<<image_cols;
//    cout<<"   ";
//    cout<<"image_rows= "<<image_rows<<endl;
}
