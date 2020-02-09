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
void interface::send_msg_to_MCU(float target_x,\
                                float target_y,\
                                float target_z,\
                                bool is_small,\
                                float trans_ratio,\
                                int16_t  fps
                                )
{
    static float last_center_x = 0.0f, last_center_y = 0.0f, last_center_z = 0.0f;
    static uint16_t cnt = 0;

    if(target_x > 0.0f)
    {
       m_vision_data.adjustX.f = target_x;
       m_vision_data.adjustY.f = target_y;
       m_vision_data.adjustZ.f = target_z;
    }
//    printf("mode = %d\r\n", infantry_info.mode);
    if(last_center_x == m_vision_data.adjustX.f)
    {
        cnt ++;
        if(cnt > 40)
        {
            m_vision_data.adjustX.f = 0.0f;
            m_vision_data.adjustY.f = 0.0f;
            m_vision_data.adjustZ.f = 0.0f;
            cnt = 0;
        }
    }
    last_center_x = m_vision_data.adjustX.f;
    last_center_y = m_vision_data.adjustY.f;
    last_center_z = m_vision_data.adjustZ.f;
    m_vision_data.small_armor_flag.d_16 = is_small;
    m_vision_data.valid_fps.d_16 = fps;
    m_vision_data.trans_ratio.f = trans_ratio;
//    cout<<"the send data is ready,waititng for send"<<endl;
    m_serial.sendAdjustValue(m_vision_data);

}
/* @Des:this function is to end message to MCU
 * @param: *s_data -- the receive struct
 * @retval:
 */
void interface::deal_msg_from_MCU(void)
{
    m_serial.get_msg_from_MCU();
    if(m_serial.m_receive_data.id > 10)
    {
        m_detect.color = BLUE;
    }
    else
    {
        m_detect.color = RED;
    }
    if(m_serial.m_receive_data.mode == 1)
    {
        m_detect.mode = fanwheel;
    }
    else
    {
        m_detect.mode = robot;
    }
}
