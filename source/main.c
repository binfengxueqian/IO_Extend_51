/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

#include "config.h"
#include "ReceiveData.h"
#include "queue.h"
#include "STC32G_GPIO.h"
#include "STC32G_ADC.h"
#include "STC32G_UART.h"
#include "STC32G_Delay.h"
#include "STC32G_NVIC.h"
#include "STC32G_Switch.h"
#define WriteGPIO(port, pin, value) \
	{                               \
		if (value == 0)             \
		{                           \
			port &= ~(1 << pin);    \
		}                           \
		else                        \
		{                           \
			port |= (1 << pin);     \
		}                           \
	}
#define ReadGPIO(port, pin) ((port >> pin) & 0x01)
/*************	����˵��	**************

��������ʾ16·ADC��ѯ������ͨ�����ڷ��͸���λ����������115200,N,8,1��

�ö�ʱ���������ʷ�����������ʹ��1Tģʽ(���ǵͲ�������12T)����ѡ��ɱ�������������ʱ��Ƶ�ʣ�����߾��ȡ�

����ʱ, ѡ��ʱ�� 22.1184MHz (�����������ļ�"config.h"���޸�).

******************************************/

/*************	���س�������	**************/

/*************	���ر�������	**************/

/*************	���غ�������	**************/

/*************  �ⲿ�����ͱ������� *****************/

void GetGPIO(u8 gpio, u8 *port, u8 *pin)
{
	*port = gpio / 16;
	*pin = gpio % 16;
}
void GPIO_Inilization(u8 gpio, u8 mode)
{
	u8 port, pin;
	GPIO_InitTypeDef GPIO_InitStructure;
	GetGPIO(gpio, &port, &pin);
	GPIO_InitStructure.Mode = mode;
	GPIO_InitStructure.Pin = 1 << pin;
	GPIO_Inilize(port, &GPIO_InitStructure);
}

// sfr getGPIOPort(u8 port)
// {
// 	// ���ݶ˿ںţ�port��ѡ����Ӧ�ļĴ���
// 	sfr portRegister;

// 	switch (port)
// 	{
// 	case 0:
// 		portRegister = P0;
// 		break;
// 	case 1:
// 		portRegister = P1;
// 		break;
// 	case 2:
// 		portRegister = P2;
// 		break;
// 	case 3:
// 		portRegister = P3;
// 		break;
// 	case 4:
// 		portRegister = P4;
// 		break;
// 	case 5:
// 		portRegister = P5;
// 		break;
// 	case 6:
// 		portRegister = P6;
// 		break;
// 	case 7:
// 		portRegister = P7;
// 		break;
// 	default:
// 		return; // �˿ں���Ч��ֱ�ӷ���
// 	}
// 	return portRegister;
// }

void GPIO_SetValue(u8 port, u8 pin, u8 value)
{
	switch (port)
	{
	case 0:
		WriteGPIO(P0, pin, value);
		break;
	case 1:
		WriteGPIO(P1, pin, value);
		break;
	case 2:
		WriteGPIO(P2, pin, value);
		break;
	case 3:
		WriteGPIO(P3, pin, value);
		break;
	case 4:
		WriteGPIO(P4, pin, value);
		break;
	case 5:
		WriteGPIO(P5, pin, value);
		break;
	case 6:
		WriteGPIO(P6, pin, value);
		break;
	case 7:
		WriteGPIO(P7, pin, value);
		break;
	default:
		return; // �˿ں���Ч��ֱ�ӷ���
	}
}

u8 GPIO_GetValue(u8 port, u8 pin)
{
	u8 value;
	switch (port)
	{
	case 0:
		value = ReadGPIO(P0, pin);
		break;
	case 1:
		value = ReadGPIO(P1, pin);
		break;
	case 2:
		value = ReadGPIO(P2, pin);
		break;
	case 3:
		value = ReadGPIO(P3, pin);
		break;
	case 4:
		value = ReadGPIO(P4, pin);
		break;
	case 5:
		value = ReadGPIO(P5, pin);
		break;
	case 6:
		value = ReadGPIO(P6, pin);
		break;
	case 7:
		value = ReadGPIO(P7, pin);
		break;
	default:
		return; // �˿ں���Ч��ֱ�ӷ���
	}
	return value;
}

void GPIO_Set(u8 gpio, u8 value)
{
	u8 port, pin;
	GetGPIO(gpio, &port, &pin);
	GPIO_SetValue(port, pin, value);
}

u8 GPIO_Get(u8 gpio)
{
	u8 port, pin;
	GetGPIO(gpio, &port, &pin);
	return GPIO_GetValue(port, pin);
}

/******************* IO���ú��� *******************/
void GPIO_config(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;
	// GPIO_InitStructure.Mode = GPIO_PullUp;
	// GPIO_InitStructure.Pin = 0;
	// GPIO_Inilize(0, &GPIO_InitStructure);
	// P0_MODE_IN_HIZ(GPIO_Pin_LOW | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6); // P0.0~P0.6 ����Ϊ��������
	// P1_MODE_IN_HIZ(GPIO_Pin_All);										 // P1.0~P1.7 ����Ϊ��������
	// P4_MODE_IO_PU(GPIO_Pin_6 | GPIO_Pin_7);								 // P4.6,P4.7 ����Ϊ׼˫���
}

/******************* AD���ú��� *******************/
void ADC_config(void)
{
	ADC_InitTypeDef ADC_InitStructure;					   // �ṹ����
	ADC_InitStructure.ADC_SMPduty = 31;					   // ADC ģ���źŲ���ʱ�����, 0~31��ע�⣺ SMPDUTY һ����������С�� 10��
	ADC_InitStructure.ADC_CsSetup = 0;					   // ADC ͨ��ѡ��ʱ����� 0(Ĭ��),1
	ADC_InitStructure.ADC_CsHold = 1;					   // ADC ͨ��ѡ�񱣳�ʱ����� 0,1(Ĭ��),2,3
	ADC_InitStructure.ADC_Speed = ADC_SPEED_2X1T;		   // ���� ADC ����ʱ��Ƶ��	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED; // ADC�������,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);					   // ��ʼ��
	ADC_PowerControl(ENABLE);							   // ADC��Դ����, ENABLE��DISABLE
	NVIC_ADC_Init(DISABLE, Priority_0);					   // �ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/***************  ���ڳ�ʼ������ *****************/
void UART_config(void)
{
	COMx_InitDefine COMx_InitStructure;				//�ṹ����
	COMx_InitStructure.UART_Mode = UART_8bit_BRTx;	//ģʽ,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use = BRT_Timer2;	//ѡ�����ʷ�����, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate = 115200ul;	//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable = ENABLE;		//��������,   ENABLE��DISABLE
	UART_Configuration(UART1, &COMx_InitStructure); //��ʼ������ UART1,UART2,UART3,UART4
	NVIC_UART1_Init(ENABLE, Priority_1);			//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	UART1_SW(UART1_SW_P30_P31); // UART1_SW_P30_P31
}

/**********************************************/
void main(void)
{
	u8 i;
	u16 j;
	u8 gpio;
	u8 value;
	unsigned char pin = 0; // GPIO�ڵ����ź�

	CommandStruct commandStruct;
	CommandType commandType;

	WTST = 0; //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR(); //��չSFR(XFR)����ʹ��
	// GPIO_config();
	UART_config();
	// ADC_config();
	EA = 1;
	initCommandQueue(&commandQueue);
	printf("STC32G AD to UART Test Programme!\r\n"); // UART����һ���ַ���

	while (1)
	{
		// for (i = 0; i < 16; i++)
		// {
		// 	delay_ms(200);

		// 	// Get_ADCResult(i);		//����0~15,��ѯ��ʽ��һ��ADC, ����һ��
		// 	j = Get_ADCResult(i); //����0~15,��ѯ��ʽ��һ��ADC, ����ֵ���ǽ��, == 4096 Ϊ����
		// 	printf("AD%02d=%04d ", i, j);
		// 	if ((i & 7) == 7)
		// 		printf("\r\n");
		// }
		printf("1");
		while (!isCommandQueueEmpty(&commandQueue))
		{
			printf("2\r\n");
			if (dequeue(&commandQueue, &commandStruct) == 0)
			{
				break;
			}
			commandType = commandStruct.commandType;
			gpio = commandStruct.gpio;

			switch (commandType)
			{
			case CONFIG_PWM:
				printf("%d,CONFIG_PWM\r\n", gpio);
				break;
			case CONFIG_ADC:
				printf("%d,CONFIG_ADC\r\n", gpio);
				break;
			case CONFIG_OUT:
				printf("%d,CONFIG_OUT\r\n", gpio);
				GPIO_Inilization(gpio, GPIO_PullUp);
				break;
			case CONFIG_IN:
				printf("%d,CONFIG_IN\r\n", gpio);
				GPIO_Inilization(gpio, GPIO_PullUp);
				break;
			case SET_PWM:
				printf("%d,SET_PWM\r\n", gpio);
				break;
			case SET_DIGITAL:
				printf("%d,SET_DIGITAL\r\n", gpio);
				GPIO_Set(gpio, commandStruct.value8);
				break;
			case GET_DIGITAL:
				printf("%d,GET_DIGITAL\r\n", gpio);
				// todo send to uart
				value = GPIO_Get(gpio);
				printf("%#X,%d\r\n", gpio, value);
				break;
			case GET_ADC:
				printf("%d,GET_ADC\r\n", gpio);
				break;
			default:
				printf("%d,UNKNOWN\r\n", gpio);
				break;
			}
		}
		if (frameLength != 0)
		{
			printf("frameLength=%d\r\n", frameLength);
			for (i = 0; i < frameLength; i++)
			{
				printf("%02x ", frame[i]);
			}
			printf("\r\n");
			// frameLength = 0;
		}
		delay_ms(1000);
	}
}
