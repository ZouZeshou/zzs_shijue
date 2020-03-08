/*this file have some functions for interface with MCU*/
#include "interface/include/interface.h"
#include "interface/include/serial.hpp"

interface::interface(){};
interface::~interface(){};
/* @Des:this function is to end message to MCU
 * @param: mode--detect mode(fanwheel or robot)
 * @param: armor--the class of armor
 * @param: fps -- the class of picture
 * @retval:
 */
void interface::send_msg_to_MCU(s_vision_data s_send)
{
//    cout<<"the send data is ready,waititng for send"<<endl;
    m_serial.sendAdjustValue(s_send);
}
/* @Des:this function is to end message to MCU
 * @param: *s_data -- the receive struct
 * @retval:
 */
void interface::deal_msg_from_MCU(void)
{
    m_serial.get_msg_from_MCU();
    m_detect.last_mode = m_detect.mode;
    if(m_serial.m_receive_data.id > 10)
    {
        m_detect.color = RED;
    }
    else if(m_serial.m_receive_data.id > 0)
    {
        m_detect.color = BLUE;
    }
    switch(m_serial.m_receive_data.mode)
    {
        case 1:
           m_detect.mode = robot;
           break;
        case 2:
           m_detect.mode = small_fan;
           break;
        case 3:
           m_detect.mode = big_fan;
           break;
        case 4:
           m_detect.mode = gyro;
           break;
        default:
            break;
    }
    if(m_detect.last_mode != m_detect.mode)
        m_detect.is_mode_change = true;
    else
        m_detect.is_mode_change = false;
}
