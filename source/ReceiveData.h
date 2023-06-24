
#ifndef _ReceiveData_H
#define _ReceiveData_H
#include "Type_def.h"

#define FRAME_HEADER_1 0xAA  // ֡ͷ��һ���ֽ�
#define FRAME_HEADER_2 0x55  // ֡ͷ�ڶ����ֽ�
// #define USE_CRC16 0           // �Ƿ�ʹ��CRC16У��

extern unsigned char frame[256];
extern unsigned char frameLength;
extern unsigned char receiveComplete;
extern unsigned short receivedCRC;

unsigned short CRC16(unsigned char *data_, unsigned char length);
void processCommand(unsigned char *data_, unsigned char length);
void receiveOneChar(unsigned char data_);

#endif