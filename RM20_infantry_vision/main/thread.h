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
    void deal_image();
    void test();
private:
    frame                                   c_frame;
    armor_detect                            c_armor_detect;
    fan                                     c_fan;
    //interface                             c_interface;
    s_detect_t                              s_detect;

    Mat                                     m_frame;
};
typedef chrono::time_point<chrono::steady_clock,chrono::milliseconds>  micro_time;
#endif // THREAD_H
