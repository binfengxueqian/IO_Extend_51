#ifndef QUEUE_H
#define QUEUE_H
#include "ReceiveData.h"

#define MAX_QUEUE_SIZE 100

// дһ��ö�ٱ�����������ʾָ�������
typedef enum
{
    NONE = 0x00,
    CONFIG_PWM = 0x01,
    CONFIG_OUT = 0x02,
    CONFIG_IN = 0x03,
    CONFIG_ADC = 0x04,
    SET_PWM = 0x05,
    SET_DIGITAL = 0x06,
    GET_DIGITAL = 0x07,
    GET_ADC = 0x08
} CommandType;

typedef struct CommandStruct
{
    CommandType commandType;
    unsigned char gpio;
    unsigned char value8;
    unsigned short value16;
} CommandStruct;

// ������нṹ
typedef struct
{
    CommandStruct command[MAX_QUEUE_SIZE]; // �洢���ݵ�����
    unsigned char front;                   // ����ָ��
    unsigned char rear;                    // ��βָ��
} CommandQueue;

extern CommandQueue commandQueue;

// ��ʼ������
void initCommandQueue(CommandQueue *queue);

// �ж϶����Ƿ�Ϊ��
unsigned char isCommandQueueEmpty(CommandQueue *queue);
// �ж϶����Ƿ�����
unsigned char isCommandQueueFull(CommandQueue *queue);
// ��Ӳ���
unsigned char enqueue(CommandQueue *queue, CommandStruct command);
// ���Ӳ���
unsigned char dequeue(CommandQueue *queue, CommandStruct *command);

#endif