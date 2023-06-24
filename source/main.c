/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
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
/*************	功能说明	**************

本程序演示16路ADC查询采样，通过串口发送给上位机，波特率115200,N,8,1。

用定时器做波特率发生器，建议使用1T模式(除非低波特率用12T)，并选择可被波特率整除的时钟频率，以提高精度。

下载时, 选择时钟 22.1184MHz (可以在配置文件"config.h"中修改).

******************************************/

/*************	本地常量声明	**************/

/*************	本地变量声明	**************/

/*************	本地函数声明	**************/

/*************  外部函数和变量声明 *****************/

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
// 	// 根据端口号（port）选择相应的寄存器
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
// 		return; // 端口号无效，直接返回
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
		return; // 端口号无效，直接返回
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
		return; // 端口号无效，直接返回
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

/******************* IO配置函数 *******************/
void GPIO_config(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;
	// GPIO_InitStructure.Mode = GPIO_PullUp;
	// GPIO_InitStructure.Pin = 0;
	// GPIO_Inilize(0, &GPIO_InitStructure);
	// P0_MODE_IN_HIZ(GPIO_Pin_LOW | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6); // P0.0~P0.6 设置为高阻输入
	// P1_MODE_IN_HIZ(GPIO_Pin_All);										 // P1.0~P1.7 设置为高阻输入
	// P4_MODE_IO_PU(GPIO_Pin_6 | GPIO_Pin_7);								 // P4.6,P4.7 设置为准双向口
}

/******************* AD配置函数 *******************/
void ADC_config(void)
{
	ADC_InitTypeDef ADC_InitStructure;					   // 结构定义
	ADC_InitStructure.ADC_SMPduty = 31;					   // ADC 模拟信号采样时间控制, 0~31（注意： SMPDUTY 一定不能设置小于 10）
	ADC_InitStructure.ADC_CsSetup = 0;					   // ADC 通道选择时间控制 0(默认),1
	ADC_InitStructure.ADC_CsHold = 1;					   // ADC 通道选择保持时间控制 0,1(默认),2,3
	ADC_InitStructure.ADC_Speed = ADC_SPEED_2X1T;		   // 设置 ADC 工作时钟频率	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED; // ADC结果调整,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);					   // 初始化
	ADC_PowerControl(ENABLE);							   // ADC电源开关, ENABLE或DISABLE
	NVIC_ADC_Init(DISABLE, Priority_0);					   // 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

/***************  串口初始化函数 *****************/
void UART_config(void)
{
	COMx_InitDefine COMx_InitStructure;				//结构定义
	COMx_InitStructure.UART_Mode = UART_8bit_BRTx;	//模式,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use = BRT_Timer2;	//选择波特率发生器, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate = 115200ul;	//波特率,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable = ENABLE;		//接收允许,   ENABLE或DISABLE
	UART_Configuration(UART1, &COMx_InitStructure); //初始化串口 UART1,UART2,UART3,UART4
	NVIC_UART1_Init(ENABLE, Priority_1);			//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

	UART1_SW(UART1_SW_P30_P31); // UART1_SW_P30_P31
}

/**********************************************/
void main(void)
{
	u8 i;
	u16 j;
	u8 gpio;
	u8 value;
	unsigned char pin = 0; // GPIO口的引脚号

	CommandStruct commandStruct;
	CommandType commandType;

	WTST = 0; //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
	EAXSFR(); //扩展SFR(XFR)访问使能
	// GPIO_config();
	UART_config();
	// ADC_config();
	EA = 1;
	initCommandQueue(&commandQueue);
	printf("STC32G AD to UART Test Programme!\r\n"); // UART发送一个字符串

	while (1)
	{
		// for (i = 0; i < 16; i++)
		// {
		// 	delay_ms(200);

		// 	// Get_ADCResult(i);		//参数0~15,查询方式做一次ADC, 丢弃一次
		// 	j = Get_ADCResult(i); //参数0~15,查询方式做一次ADC, 返回值就是结果, == 4096 为错误
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
