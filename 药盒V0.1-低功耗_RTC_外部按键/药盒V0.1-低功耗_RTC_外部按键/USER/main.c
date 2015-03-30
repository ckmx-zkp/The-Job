#include "led.h"
#include "delay.h"
#include "rtc.h"
#include "sys.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rtc.h"

GPIO_InitTypeDef  GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
EXTI_InitTypeDef EXTI_InitStructure;

//#################按键、LED宏定义######################//
#define OPEN_REDLED 	 GPIO_ResetBits(GPIOD, GPIO_Pin_6)
#define CLOSE_REDLED 	   GPIO_SetBits(GPIOD, GPIO_Pin_6)
#define GetLED0 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)
#define OPEN_GREENLED GPIO_ResetBits(GPIOD,  GPIO_Pin_2)
#define CLOSE_GREENLED GPIO_SetBits(GPIOD,   GPIO_Pin_5)
#define GetLED1 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5)

#define GetKey0	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)
#define GetKey1	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)
#define GetKey2 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)

//将串口配置为外部中断以便于唤醒
void UartStop_Config(void)
{
		printf("现在关闭串口\r\n");
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;               												//将串口接受引脚设置为外部中断
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            											 //上拉输入  IO角保持高电平
		GPIO_Init(GPIOA, &GPIO_InitStructure);	                		

		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);	
		EXTI_InitStructure.EXTI_Line=EXTI_Line10;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	  																				//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;										//使能按键所在的外部中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;					 //抢占优先级最高
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					        //子优先级最高
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;											 //使能外部中断通道
		NVIC_Init(&NVIC_InitStructure); 
}

//将串口恢复的函数
void UartRecovery_Config(void)
{
//		uart_init(115200);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10
	
		EXTI_InitStructure.EXTI_Line=EXTI_Line10;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);	  	
}

void RTC_EXTI_Config(void)
{
		EXTI_ClearITPendingBit(EXTI_Line17);
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
		EXTI_InitStructure.EXTI_Line = EXTI_Line17;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
		NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;										//使能按键所在的外部中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;					 //抢占优先级2， 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					        //子优先级1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;											 //使能外部中断通道
		NVIC_Init(&NVIC_InitStructure); 
}
	

void RCC_wake(void)
{
		ErrorStatus HSEStartUpStatus;
	 /* 使能 HSE */
		RCC_HSEConfig(RCC_HSE_ON);
	 /* 等待 HSE 准备就绪 */
		HSEStartUpStatus = RCC_WaitForHSEStartUp();
		if(HSEStartUpStatus == SUCCESS)
		{
				RCC_PLLCmd(ENABLE);																									 /* 使能 PLL */
				
				while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)								 /* 等待 PLL 准备就绪 */
				;
				
				RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);											/* 选择 PLL 作为系统时钟源 */
			
			 while(RCC_GetSYSCLKSource() != 0x08) 													/* 等待 PLL 被选择为系统时钟源 */
				;
		}
}

void Key_Config(void)
{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;               																							  //KEY0 -->PC5 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            																							 //上拉输入  IO角保持高电平
		GPIO_Init(GPIOC, &GPIO_InitStructure);	  	
}

void LED_Config(void)
{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;;			 																									 //LED0红灯-->PD5 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 																											    //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																													   //IO口速度为50MHz
		GPIO_Init(GPIOD, &GPIO_InitStructure);																																		  //根据设定参数初始化GPIOD5
		OPEN_GREENLED;
		OPEN_REDLED;
}

void EXTI_Config(void)
{
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);		
	
		EXTI_InitStructure.EXTI_Line=EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	  																				//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;										//使能按键所在的外部中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;					 //抢占优先级2， 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					        //子优先级1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;											 //使能外部中断通道
		NVIC_Init(&NVIC_InitStructure); 

	
}
int main(void)
{	
		u8 Time_Data[200];
		delay_init();
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能PA,PC,PD端口时钟
		LED_Config();
		Key_Config();  
          																						  //配置PC5
		EXTI_Config();
		uart_init(115200);
		printf("Init Uart is ok!\r\n");
		//###################################RTC初始化########################################//
		while(RTC_Init())																											//RTC初始化	，一定要初始化成功
		{ 
			printf("RTC ERROR!");	
			delay_ms(800);
			printf("RTC Trying...");	
		}		  
		printf("Init RTC is ok!\r\n");
		

	//#####################################外部中断#######################################//
											
		while(1)
		{
				sprintf(Time_Data,"%d/%d/%d-%d:%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min);
				printf(Time_Data);
				delay_ms(1000);
				if(!GetKey0)
				{
						delay_ms(5);
						if(!GetKey0)
						{
								printf(" 进入低功耗\r\n");
								while(!GetKey0);
								RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
								UartStop_Config();
								PWR_WakeUpPinCmd(ENABLE);  
								RTC_WaitForLastTask();
								RTC_SetAlarm(RTC_GetCounter()+20);
								RTC_EXTI_Config();
								RTC_ClearFlag(RTC_FLAG_SEC);
								PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);											//进入停止模式
						}
				}
		}	
}
void EXTI1_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line1) != RESET) 
		{
				delay_ms(10); 																																				//消抖
				if(!GetKey1)
				{
						RCC_wake();
						printf("进入正常模式!");
				}
				EXTI_ClearITPendingBit(EXTI_Line1);  																								//清除EXTI1线路挂起位
		}
}

void EXTI15_10_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line10) != RESET) 
		{

				RCC_wake();
				UartRecovery_Config();
				delay_ms(5);																																			//轻微延时以避免串口丢包
				printf("通过串口进入正常模式!");
				EXTI_ClearITPendingBit(EXTI_Line10);  																								//清除EXTI1线路挂起位
		}	
}

void RTC_IRQHandler()
{
//		RCC_wake();
//		UartRecovery_Config();
//		delay_ms(5);																																			//轻微延时以避免串口丢包
//		printf("通过RTC进入正常模式!");	
//		EXTI_ClearITPendingBit(EXTI_Line17);
		if(RTC_GetITStatus(RTC_IT_ALR) != RESET) 
		{

				RCC_wake();
				UartRecovery_Config();
				delay_ms(5);																																			//轻微延时以避免串口丢包
				printf("通过RTC进入正常模式!");
				EXTI_ClearITPendingBit(RTCAlarm_IRQn);  																								//清除EXTI1线路挂起位
		}		
}



