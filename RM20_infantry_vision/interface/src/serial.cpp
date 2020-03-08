#include "interface/include/serial.hpp"
#include "interface/include/CRC_Check.h"
int Serial::speed_arr[16] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
     B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,};
int Serial::name_arr[16] = {115200, 38400, 19200, 9600, 4800, 2400, 1200,  300,
     115200, 38400, 19200, 9600, 4800, 2400, 1200,  300,};

Serial::Serial(){
    serial_port = open(UART_DEVICE, O_RDWR);
    if (serial_port < 0)
    {
        perror(UART_DEVICE);
        exit(1);
    }
    cout<<"open serial port..."<<endl;
    set_speed(serial_port,115200);
    if (set_Parity(serial_port,8,1,'N') == FALSE)
    {
        printf("Set Parity Error\n");
        exit (0);
    }
    set_disp_mode(serial_port,0);
    cout<<"open serial success"<<endl;
}
Serial::~Serial(){
    close(serial_port);
    cout<<"close serial ok!"<<endl;
}
void Serial::sendAdjustValue(const s_vision_data &data)
{
    tdata[0] = 0xA5;

    tdata[1] = data.adjustX.uc[0];
    tdata[2] = data.adjustX.uc[1];
    tdata[3] = data.adjustX.uc[2];
    tdata[4] = data.adjustX.uc[3];

    tdata[5] = data.adjustY.uc[0];
    tdata[6] = data.adjustY.uc[1];
    tdata[7] = data.adjustY.uc[2];
    tdata[8] = data.adjustY.uc[3];

    tdata[9] = data.adjustZ.uc[0];
    tdata[10] = data.adjustZ.uc[1];
    tdata[11] = data.adjustZ.uc[2];
    tdata[12] = data.adjustZ.uc[3];

    tdata[13] = data.fps;
    tdata[14] = data.valid_fps;
    tdata[15] = data.is_find_target;
    tdata[16] = data.is_big_armor;

    tdata[17] = data.trans_ratio.uc[0];
    tdata[18] = data.trans_ratio.uc[1];
    tdata[19] = data.trans_ratio.uc[2];
    tdata[20] = data.trans_ratio.uc[3];

    Append_CRC16_Check_Sum(tdata,23);
//    cout<<"writting data to serial..."<<endl;
    write(serial_port,tdata,23);
//    cout<<"send msg success"<<endl;
}

void Serial::get_msg_from_MCU(void)
{
    int bytes;
    ioctl(serial_port, FIONREAD, &bytes);
    if(bytes == 0)
        return;

    bytes = static_cast<int>(read(serial_port,rdata,7));
    if(rdata[0] == 0xA0 && Verify_CRC8_Check_Sum(rdata,5))
    {
        m_receive_data.id  = rdata[1];
        m_receive_data.mode = rdata[2];
        //m_receive_data.is_big_fan = rdata[3];
    }
}

int Serial::set_disp_mode(int fd,int option){
   int err;
   struct termios term;
   if(tcgetattr(fd,&term)==-1){
     perror("Cannot get the attribution of the terminal");
     return 1;
   }
   if(option)
        term.c_lflag|=ECHOFLAGS;
   else
        term.c_lflag &=~ECHOFLAGS;
   err=tcsetattr(fd,TCSAFLUSH,&term);
   if(err==-1 && err==EINTR){
        perror("Cannot set the attribution of the terminal");
        return 1;
   }
   return 0;
}

void Serial::set_speed(int fd, int speed){
    int   i;
    int   status;
    struct termios   Opt;
    tcgetattr(fd, &Opt);
    for ( i= 0;  i < (int)(sizeof(speed_arr) / sizeof(int));  i++) {
        if  (speed == name_arr[i]) {
            tcflush(fd, TCIOFLUSH);
            cfsetispeed(&Opt, speed_arr[i]);
            cfsetospeed(&Opt, speed_arr[i]);
            status = tcsetattr(fd, TCSANOW, &Opt);
            if  (status != 0) {
                perror("tcsetattr fd1");
                return;
            }
            tcflush(fd,TCIOFLUSH);
        }
    }
}

int Serial::set_Parity(int fd,int databits,int stopbits,int parity){
    struct termios options;
    if  (tcgetattr( fd,&options)  !=  0) {
        perror("SetupSerial 1");
        return(FALSE);
    }
    options.c_cflag &= ~CSIZE;
    switch (databits) /*????????��??*/
    {
        case 7:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;
        default:
            fprintf(stderr,"Unsupported data size\n"); return (FALSE);
    }
    switch (parity)
    {
        case 'n':
        case 'N':
            options.c_cflag &= ~PARENB;   /* Clear parity enable */
            options.c_iflag &= ~INPCK;     /* Enable parity checking */
            break;
        case 'o':
        case 'O':
            options.c_cflag |= (PARODD | PARENB); /* ???????��??*/
            options.c_iflag |= INPCK;             /* Disnable parity checking */
            break;
        case 'e':
        case 'E':
            options.c_cflag |= PARENB;     /* Enable parity */
            options.c_cflag &= ~PARODD;   /* ?????��??*/
            options.c_iflag |= INPCK;       /* Disnable parity checking */
            break;
        case 'S':
        case 's':  /*as no parity*/
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;break;
        default:
            fprintf(stderr,"Unsupported parity\n");
            return (FALSE);
    }
    /* ?????????*/
    switch (stopbits)
    {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
        default:
            fprintf(stderr,"Unsupported stop bits\n");
            return (FALSE);
    }
    /* Set input parity option */
    if (parity != 'n')
        options.c_iflag |= INPCK;
    tcflush(fd,TCIFLUSH);
    options.c_cc[VTIME] = 150; /* ???????15 seconds*/
    options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
        perror("SetupSerial 3");
        return (FALSE);
    }
    //options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
    options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    options.c_lflag &= ~(ICANON);
    options.c_lflag &= ~(ICANON | ISIG);
    options.c_iflag &= ~(ICRNL | IGNCR);
    options.c_lflag &= ~(ECHO | ECHOE);
    options.c_oflag  &= ~OPOST;   /*Output*/
    if (tcsetattr(fd,TCSAFLUSH,&options) != 0)
    {
        perror("SetupSerial 3");
        return (FALSE);
    }
    return (TRUE);
}
