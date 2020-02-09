/*this file inlude the header of debug-Setting.cpp
 * include some definition of some param of save video
 */

#ifndef DEBUG_SETTINGS_H
#define DEBUG_SETTINGS_H

#include "main/global.h"
#include <sys/time.h>


class debug_Settings
{
public:
    debug_Settings();
    ~debug_Settings();
    void set_is_videosavemode(bool is_save);
    void set_videowriter_path(string save_path,int image_cols,int image_rows);
    void get_debugparam();
    void write_video(Mat frame_to_save);
private:
    VideoWriter c_videowrite;
    bool m_is_save;
};


#endif // DEBUG_SETTINGS_H
