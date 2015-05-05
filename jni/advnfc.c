#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <jni.h>

#include "advnfc.h"
#include "serialport.h"
#include "HSJ_B.h"

#define UART_PORT "/dev/ttymxc1"

/*
 * 设置HSJ522波特率
 * 发送两次0x20,间隔数十us，让读卡模块探测该波特率
 */
void set_module_baud(int fd)
{
	int i;
	char buff[BUFFER_SIZE];
	memset(buff, 0, BUFFER_SIZE);
	sprintf(buff,"%d",STX);
	write(fd, buff, strlen(buff));
	for(i=150;i>0;i--);	//延时
	write(fd, buff, strlen(buff));
}

int init_uart()
{
	int fd;
	char buff[BUFFER_SIZE];

	if((fd = open_port(UART_PORT)) < 0) {
		ALOGE("open_port error");
		return -1;
	}
	if(set_com_config(fd, 9600, 8, 'N', 1) < 0)  {
		ALOGE("set_com_config error");
		return -1;
	}

	set_module_baud(fd);

	close(fd);
	return 1;
}

/*
 * Class:     com_advantech_advnfc_AdvNFC
 * Method:    doInit
 * Signature: ()I
 */
JNIEXPORT void JNICALL Java_com_advantech_advnfc_AdvNFC_doInit
  (JNIEnv *env, jobject thiz)
{
	ALOGE("OOOOOOOOOOOdddddddddddOOOOOOOk\n");
	init_uart();
	ALOGE("OOOOOOOOOOOOOOOOOOk\n");
}

/*
 * Class:     com_advantech_advnfc_AdvNFC
 * Method:    scan
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_advantech_advnfc_AdvNFC_scan
  (JNIEnv *env, jobject thiz)
{

}

/*
 * Class:     com_advantech_advnfc_AdvNFC
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_advantech_advnfc_AdvNFC_close
  (JNIEnv *env, jobject thiz)
{

}

