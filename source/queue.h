#ifndef QUEUE_H
#define QUEUE_H
#include "ReceiveData.h"

#define MAX_QUEUE_SIZE 100

// 写一个枚举变量，用来表示指令的类型
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

// 定义队列结构
typedef struct
{
    CommandStruct command[MAX_QUEUE_SIZE]; // 存储数据的数组
    unsigned char front;                   // 队首指针
    unsigned char rear;                    // 队尾指针
} CommandQueue;

extern CommandQueue commandQueue;

// 初始化队列
void initCommandQueue(CommandQueue *queue);

// 判断队列是否为空
unsigned char isCommandQueueEmpty(CommandQueue *queue);
// 判断队列是否已满
unsigned char isCommandQueueFull(CommandQueue *queue);
// 入队操作
unsigned char enqueue(CommandQueue *queue, CommandStruct command);
// 出队操作
unsigned char dequeue(CommandQueue *queue, CommandStruct *command);

#endif