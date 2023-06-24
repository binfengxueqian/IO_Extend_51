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
    // �������ݲ�ִ����Ӧ����
    unsigned char command = data_[3]; // ��ȡ���ĸ��ֽڵ�ָ��

    if (command == 'C')
    {
        unsigned char configType = data_[4]; // ��ȡ��������

        if (configType == 'P')
        {
            // ����PWM
            unsigned char gpio = data_[5];
            unsigned short frequency = (data_[6] << 8) | data_[7];
            // ִ������PWM�Ĳ���
            addCommandStruct(CONFIG_PWM, gpio, 0, frequency);
        }
        else if (configType == 'O')
        {
            // �����������
            unsigned char gpio = data_[5];
            // ִ������������ŵĲ���
            addCommandStructZeroParam(CONFIG_OUT, gpio);
        }
        else if (configType == 'I')
        {
            // ������������
            unsigned char gpio = data_[5];
            unsigned char mode = data_[6];
            unsigned char param = data_[7];
            // ִ�������������ŵĲ���
            addCommandStruct(CONFIG_IN, gpio, mode, param);
        }
        else if (configType == 'A')
        {
            // ����ADC��������
            unsigned char gpio = data_[5];
            unsigned char mode = data_[6];
            unsigned char param = data_[7];
            // ִ������ADC�������õĲ���
            addCommandStruct(CONFIG_ADC, gpio, mode, param);
        }
    }
    if (command == 'P')
    {
        // ���PWM
        unsigned char gpio = data_[4];
        unsigned short dutyCycle = (data_[5] << 8) | data_[6];
        // ִ�����PWM�Ĳ���
        addCommandStruct(SET_PWM, gpio, 0, dutyCycle);
    }
    if (command == 'O')
    {
        // �������
        unsigned char gpio = data_[4];
        unsigned char value = data_[5];
        // ִ��������ŵĲ���
        addCommandStruct(SET_DIGITAL, gpio, value, 0);
    }
    if (command == 'I')
    {
        // ��ȡ����
        unsigned char gpio = data_[4];
        // ִ�ж�ȡ���ŵĲ���
        addCommandStruct(GET_DIGITAL, gpio, 0, 0);
    }
    if (command == 'A')
    {
        // ��ȡADC
        unsigned char gpio = data_[4];
        // ִ�ж�ȡADC�Ĳ���
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
            return; // δ���յ�֡ͷ��һ���ֽڣ������ȴ�
        }

        if (frameLength == 1 && data_ != FRAME_HEADER_2)
        {
            frameLength = 0; // δ���յ�֡ͷ�ڶ����ֽڣ����¿�ʼ����
            return;
        }

        frame[frameLength++] = data_;

#ifdef USE_CRC16
        if (frameLength >= 3 && frameLength == frame[2] + 5)
#else
        if (frameLength >= 3 && frameLength == frame[2] + 3)
#endif
        {
            receiveComplete = 1; // �������
        }
    }

    if (receiveComplete)
    {
#ifdef USE_CRC16
        receivedCRC = (frame[frameLength - 2] << 8) | frame[frameLength - 1];
        calculatedCRC = CRC16(frame, frameLength - 2);

        if (receivedCRC == calculatedCRC)
        {
            // У��ͨ����ִ��Э�����
            processCommand(frame, frameLength);
        }
#else
        // ��ʹ��CRCУ�飬ֱ��ִ��Э�����
        processCommand(frame, frameLength);
#endif

        // ���ý���״̬
        frameLength = 0;
        receiveComplete = 0;
    }
}