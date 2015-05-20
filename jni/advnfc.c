#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <jni.h>

#include "advnfc.h"
#include "serialport.h"

int init_uart()
{
	int fd;
	char buff[BUFFER_SIZE];
	unsigned char cardtype[2];
	unsigned char databuf[16];

	if((fd = open_port(UART_PORT)) < 0) {
		ALOGE("open_port error");
		return -1;
	}
	if(set_com_config(fd, 9600, 8, 'N', 1) < 0)  {
		ALOGE("set_com_config error");
		return -1;
	}

	close(fd);
	return 1;
}

/*
 * Class:     com_advantech_advnfc_AdvNFC
 * Method:    loadKey:将密匙0xffffffffffff装载到模块内第0区的A密匙区，该命令仅需调用一次就行了
 *            自动保存在模块EEPROM里，若高强度刷写，容易出现EEPROM被写坏的问题。
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_advantech_advnfc_AdvNFC_loadKey
  (JNIEnv *env, jobject thiz)
{
	int i, fd;
	unsigned char buff[BUFFER_SIZE];

	if((fd = open_port(UART_PORT)) < 0) {
		ALOGE("open_port error");
		//return -1;
	}
	if(set_com_config(fd, 9600, 8, 'N', 1) < 0)  {
		ALOGE("set_com_config error");
		//return -1;
	}

	write(fd, loadKeyCmd, (sizeof(loadKeyCmd) / sizeof(unsigned char)));
	for (i = 0; i < (sizeof(loadKeyRtn) / sizeof(unsigned char)); i++) {
		if(read(fd, buff, BUFFER_SIZE) > 0){
			if(buff[0] != loadKeyRtn[i]){
				close(fd);
				return (jboolean) -1;
			}
			memset(buff, 0, BUFFER_SIZE);
		}
	}

	close(fd);
	return (jboolean) 1;
}

/*
 * Class:     com_advantech_advnfc_AdvNFC
 * Method:    readCard
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_advantech_advnfc_AdvNFC_readCard
  (JNIEnv *env, jobject thiz)
{
	int i, fd;
	unsigned char buff[BUFFER_SIZE];
	unsigned char readData[DATACOUNT];

	if((fd = open_port(UART_PORT)) < 0) {
		ALOGE("open_port error");
		//return -1;
	}
	if(set_com_config(fd, 9600, 8, 'N', 1) < 0)  {
		ALOGE("set_com_config error");
		//return -1;
	}

	write(fd, getCardSnrCmd, (sizeof(getCardSnrCmd) / sizeof(unsigned char)));
	write(fd, authCmd, (sizeof(authCmd) / sizeof(unsigned char)));
	write(fd, readDataCmd, (sizeof(authCmd) / sizeof(unsigned char)));

	for (i = 0; i < DATACOUNT; i++) {
		if (read(fd, buff, BUFFER_SIZE) > 0) {
			readData[i] = buff[i];
			ALOGE("readData[%d]=%x\n", i, readData[i]);
			memset(buff, 0, BUFFER_SIZE);
		}
	}

	close(fd);
}

