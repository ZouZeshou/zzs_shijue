#ifndef THREAD_H
#define THREAD_H
#include "get_frame/frame/frame.h"
#include "armor_detect/armor_detect.h"
#include "interface/include/interface.h"
#include "main/global.h"
#include "watchdog/WatchDog.h"
#include "fanwheel/fanwheel.hpp"
#include "fanwheel/fan.hpp"
#include "fanwheel/fan_middleware.h"
class task
{
public:
    void get_image();
    int deal_image();
    void test();
    int plot();
private:
    frame                                   c_frame;
    armor_detect                            c_armor_detect;
    fan                                     c_fan;
    s_detect_t                              s_detect;
    s_vision_data                           s_send;
    Mat                                     m_frame;
    Mat                                     m_src;
    bool                                    m_plot_flag;
    #ifndef NO_SERIAL_PORT
    interface                             c_interface;
    #endif
};
typedef chrono::time_point<chrono::steady_clock,chrono::milliseconds>  micro_time;
#endif // THREAD_H
