#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <jni.h>

#include<android/log.h>
static const char *TAG="advnfc_jni";
#define ALOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define ALOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define ALOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

#define 	BUFFER_SIZE		32
#define		UART_PORT		"/dev/ttymxc1"

//定义通信帧常量
#define		STX			0x20
#define		ETX			0x03

//定义数据块格式的位置
#define		SEQNR                   1          //数据交换包的序号
#define		COMMAND                 2          //命令字符
#define		STATUS                  2          //状态字符
#define		LENGTH                  3          //数据的长度
#define		DATA                    4          //数据字节

//定义命令
unsigned char loadKeyCmd[] = { 0x20, 0x00, 0x4C, 0x08, 0x00, 0x00, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xBB, 0x03 };
unsigned char loadKeyRtn[] = { 0x06, 0x20, 0x00, 0x00, 0x00, 0xFF, 0x03 };
unsigned char getCardSnrCmd[] = { 0x20, 0x00, 0x10, 0x01, 0x00, 0xee, 0x03 };
unsigned char authCmd[] =
		{ 0x20, 0x00, 0x72, 0x03, 0x00, 0x00, 0x00, 0x8e, 0x03 };
unsigned char readDataCmd[] = { 0x20, 0x00, 0x46, 0x01, 0x02, 0xba, 0x03 };
const int DATACOUNT = 23;

int open_port(char* com_port)
{
    int fd;

    fd = open(com_port, O_RDWR|O_NOCTTY|O_NDELAY);

    if (fd < 0){
    	ALOGE("open serial port");
        return(-1);
    }

    if (fcntl(fd, F_SETFL, 0) < 0){
    	ALOGE("fcntl F_SETFL\n");
    }

    if (isatty(STDIN_FILENO) == 0){
    	ALOGE("standard input is not a terminal device");
    }

    return fd;
}

int set_com_config(int fd,int baud_rate, int data_bits, char parity, int stop_bits)
{
    struct termios new_cfg,old_cfg;
    int speed;

    if  (tcgetattr(fd, &old_cfg)  !=  0){
    	ALOGE("tcgetattr");
        return -1;
    }


    new_cfg = old_cfg;
    cfmakeraw(&new_cfg);
    new_cfg.c_cflag &= ~CSIZE;

    switch (baud_rate){
    case 2400:
        speed = B2400;
        break;

    case 4800:
        speed = B4800;
        break;

    case 9600:
        speed = B9600;
        break;

    case 19200:
        speed = B19200;
        break;

    case 38400:
        speed = B38400;
        break;

    default:
    case 115200:
        speed = B115200;
        break;
    }
    cfsetispeed(&new_cfg, speed);
    cfsetospeed(&new_cfg, speed);

    switch (data_bits){
    case 7:
        new_cfg.c_cflag |= CS7;
        break;

    default:
    case 8:
        new_cfg.c_cflag |= CS8;
        break;
    }

    switch (parity){
    default:
    case 'n':
    case 'N':
        new_cfg.c_cflag &= ~PARENB;
        new_cfg.c_iflag &= ~INPCK;
        break;

    case 'o':
    case 'O':
        new_cfg.c_cflag |= (PARODD | PARENB);
        new_cfg.c_iflag |= INPCK;
        break;

    case 'e':
    case 'E':
        new_cfg.c_cflag |= PARENB;
        new_cfg.c_cflag &= ~PARODD;
        new_cfg.c_iflag |= INPCK;
        break;

    case 's':  /*as no parity*/
    case 'S':
        new_cfg.c_cflag &= ~PARENB;
        new_cfg.c_cflag &= ~CSTOPB;
        break;
    }

    switch (stop_bits){
    default:
    case 1:
        new_cfg.c_cflag &=  ~CSTOPB;
        break;

    case 2:
        new_cfg.c_cflag |= CSTOPB;
    }

    new_cfg.c_cc[VTIME]  = 0;
    new_cfg.c_cc[VMIN] = 1;

    tcflush(fd, TCIFLUSH);

    if((tcsetattr(fd, TCSANOW, &new_cfg)) != 0){
    	ALOGE("tcsetattr");
        return -1;
    }

    return 0;
}
