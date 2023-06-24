#include "ReceiveData.h"
#include "queue.h"

unsigned char frame[256];
unsigned char frameLength = 0;
unsigned char receiveComplete = 0;
unsigned short receivedCRC = 0;
unsigned short calculatedCRC = 0;

unsigned short CRC16(unsigned char *data_, unsigned char length)
{
    unsigned short crc = 0xFFFF;
    unsigned char i, j;

    for (i = 0; i < length; i++)
    {
        crc ^= data_[i];
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

void addCommandStruct(CommandType type_, unsigned char gpio_, unsigned char value8_, unsigned short value16_)
{
    CommandStruct commandStruct;
    commandStruct.commandType = type_;
    commandStruct.gpio = gpio_;
    commandStruct.value8 = value8_;
    commandStruct.value16 = value16_;
    enqueue(&commandQueue, commandStruct);
}

void addCommandStructOneParam(CommandType type_, unsigned char gpio_, unsigned char value8_)
{
    addCommandStruct(type_, gpio_, value8_, 0);
}

void addCommandStructZeroParam(CommandType type_, unsigned char gpio_)
{
    addCommandStruct(type_, gpio_, 0, 0);
}

void processCommand(unsigned char *data_, unsigned char length)
{
    // 解析数据并执行相应操作
    unsigned char command = data_[3]; // 获取第四个字节的指令

    if (command == 'C')
    {
        unsigned char configType = data_[4]; // 获取配置类型

        if (configType == 'P')
        {
            // 配置PWM
            unsigned char gpio = data_[5];
            unsigned short frequency = (data_[6] << 8) | data_[7];
            // 执行配置PWM的操作
            addCommandStruct(CONFIG_PWM, gpio, 0, frequency);
        }
        else if (configType == 'O')
        {
            // 配置输出引脚
            unsigned char gpio = data_[5];
            // 执行配置输出引脚的操作
            addCommandStructZeroParam(CONFIG_OUT, gpio);
        }
        else if (configType == 'I')
        {
            // 配置输入引脚
            unsigned char gpio = data_[5];
            unsigned char mode = data_[6];
            unsigned char param = data_[7];
            // 执行配置输入引脚的操作
            addCommandStruct(CONFIG_IN, gpio, mode, param);
        }
        else if (configType == 'A')
        {
            // 配置ADC输入配置
            unsigned char gpio = data_[5];
            unsigned char mode = data_[6];
            unsigned char param = data_[7];
            // 执行配置ADC输入配置的操作
            addCommandStruct(CONFIG_ADC, gpio, mode, param);
        }
    }
    if (command == 'P')
    {
        // 输出PWM
        unsigned char gpio = data_[4];
        unsigned short dutyCycle = (data_[5] << 8) | data_[6];
        // 执行输出PWM的操作
        addCommandStruct(SET_PWM, gpio, 0, dutyCycle);
    }
    if (command == 'O')
    {
        // 输出引脚
        unsigned char gpio = data_[4];
        unsigned char value = data_[5];
        // 执行输出引脚的操作
        addCommandStruct(SET_DIGITAL, gpio, value, 0);
    }
    if (command == 'I')
    {
        // 读取引脚
        unsigned char gpio = data_[4];
        // 执行读取引脚的操作
        addCommandStruct(GET_DIGITAL, gpio, 0, 0);
    }
    if (command == 'A')
    {
        // 读取ADC
        unsigned char gpio = data_[4];
        // 执行读取ADC的操作
        addCommandStruct(GET_ADC, gpio, 0, 0);
    }
}

void receiveOneChar(unsigned char data_)
{
    // printf("%c",data_);
    if (!receiveComplete)
    {
        if (frameLength == 0 && data_ != FRAME_HEADER_1)
        {
            return; // 未接收到帧头第一个字节，继续等待
        }

        if (frameLength == 1 && data_ != FRAME_HEADER_2)
        {
            frameLength = 0; // 未接收到帧头第二个字节，重新开始接收
            return;
        }

        frame[frameLength++] = data_;

#ifdef USE_CRC16
        if (frameLength >= 3 && frameLength == frame[2] + 5)
#else
        if (frameLength >= 3 && frameLength == frame[2] + 3)
#endif
        {
            receiveComplete = 1; // 接收完成
        }
    }

    if (receiveComplete)
    {
#ifdef USE_CRC16
        receivedCRC = (frame[frameLength - 2] << 8) | frame[frameLength - 1];
        calculatedCRC = CRC16(frame, frameLength - 2);

        if (receivedCRC == calculatedCRC)
        {
            // 校验通过，执行协议解析
            processCommand(frame, frameLength);
        }
#else
        // 不使用CRC校验，直接执行协议解析
        processCommand(frame, frameLength);
#endif

        // 重置接收状态
        frameLength = 0;
        receiveComplete = 0;
    }
}