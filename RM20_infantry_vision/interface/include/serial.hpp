#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <stdio.h>    //Standard input-output definition
#include <stdlib.h>   //standard function library definition
#include <unistd.h>   //Unix standard function definition
#include <sys/types.h>  //include primitive system data types
#include <sys/stat.h>  //get file attribute
#include <fcntl.h>     //Defined related macros used in file operations
#include <termios.h>   //PPSIX terminal control define
#include <errno.h>     // error number define
#include <string.h>    //string dispose function
#include <sys/ioctl.h>  //IO control
#include "main/global.h"

using namespace std;

#define TRUE 0
#define FALSE -1

#define BAUDRATE        115200
#define UART_DEVICE     "/dev/ttyUSB0"        // "/dev/ttySerial0"

#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)
//class ImageProc;

class Serial
{
public:
    Serial();
    ~Serial();
    void sendAdjustValue(const s_vision_data &data);
    void get_msg_from_MCU(void);
public:
    s_receive_data_t m_receive_data;
private:
    int set_disp_mode(int fd,int option);
    void set_speed(int fd, int speed);
    int set_Parity(int fd,int databits,int stopbits,int parity);

    int serial_port;
    unsigned char tdata[256];
    unsigned char rdata[256];
    static int speed_arr[16];
    static int name_arr[16];

};

#endif // SERIAL_HPP
