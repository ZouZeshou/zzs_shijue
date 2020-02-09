#ifndef INTERFACE_H
#define INTERFACE_H
#include "main/global.h"
#include "interface/include/serial.hpp"
#include "armor_detect/armor_detect.h"
#include "get_frame/frame/frame.h"
class interface
{
public:
    interface();
    ~interface();
    void send_msg_to_MCU(float target_x,\
                         float target_y,\
                         float target_z,\
                         bool is_small,\
                         float trans_ratio,\
                         int16_t  fps);
    void deal_msg_from_MCU(void);
public:
    s_detect_t              m_detect;
private:
    s_vision_data           m_vision_data;
    Serial                  m_serial;

};
#endif
