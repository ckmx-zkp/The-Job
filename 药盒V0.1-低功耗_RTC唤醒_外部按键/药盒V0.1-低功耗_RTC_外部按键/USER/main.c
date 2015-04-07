/******************** (C) COPYRIGHT 2014 ??DIY?? **************************
 * 文件名  : main.c
 * 描述    : 主函数 
 * 库版本  : ST3.5.0
 * 作者    : 刘洋 张鲲鹏
 * 描述    : 关于药盒基本功能的测试 
 * 版本    : V1.0
 * 日期    : 2015/4/7
**********************************************************************************/

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

//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

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

/********************************************************************************
 * 函数名  : UartStop_Config
 * 描述    : 将串口配置为外部中断以便于唤醒
 * 输入	   : 无
 * 输出    : 无
**********************************************************************************/
extern void UartStop_Config(void)
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

/********************************************************************************
 * 函数名  : UartRecovery_Config
 * 描述    : 将串口恢复的函数
 * 输入	   : 无
 * 输出    : 无
**********************************************************************************/
extern void UartRecovery_Config(void)
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

	
/********************************************************************************
 * 函数名  : RCC_wake
 * 描述    : 恢复系统时钟
 * 输入	   : 无
 * 输出    : 无
**********************************************************************************/
extern void RCC_wake(void)
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

/********************************************************************************
 * 函数名  : Key_Config
 * 描述    : 按键初始化
 * 输入	   : 无
 * 输出    : 无
**********************************************************************************/
void Key_Config(void)
{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;               																							  //KEY0 -->PC5 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            																							 //上拉输入  IO角保持高电平
		GPIO_Init(GPIOC, &GPIO_InitStructure);	  	
}

/********************************************************************************
 * 函数名  : LED_Config
 * 描述    : LED初始化
 * 输入	   : 无
 * 输出    : 无
**********************************************************************************/
void LED_Config(void)
{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;;			 																									 //LED0红灯-->PD5 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 																											    //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																													   //IO口速度为50MHz
		GPIO_Init(GPIOD, &GPIO_InitStructure);																																		  //根据设定参数初始化GPIOD5
		OPEN_GREENLED;
		OPEN_REDLED;
}

/********************************************************************************
 * 函数名  : EXTI_Config
 * 描述    : 按键唤醒外部中断初始化
 * 输入	   : 无
 * 输出    : 无
**********************************************************************************/
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

/********************************************************************************
 * 函数名  : main
 * 描述    : 主函数
 * 输入	   : 无
 * 输出    : 无
 * 备注 	 ：先初始化各个模块
 *				 ：按下按键0进入低功耗模式
 *				 ：按下按键1恢复
 *				 ：串口发送数据自动回复
 *				 ：5秒后自动恢复
**********************************************************************************/
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
		RTC_ALARM_Config();
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
		PWR_BackupAccessCmd(ENABLE);
		RCC_RTCCLKCmd(ENABLE);
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
								
								RTC_ClearFlag(RTC_FLAG_SEC);
								while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);

								/* Alarm in 3 second */
								RTC_SetAlarm(RTC_GetCounter()+ 5);
								/* Wait until last write operation on RTC registers has finished */
								RTC_WaitForLastTask();
								//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
								UartStop_Config();
								//PWR_WakeUpPinCmd(ENABLE);  
								PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);											//进入停止模式
						}
				}
		}	
}

/********************************************************************************
 * 函数名  : EXTI1_IRQHandler
 * 描述    : 按键唤醒
 * 输入	   : 无
 * 输出    : 无
**********************************************************************************/
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

/********************************************************************************
 * 函数名  : EXTI15_10_IRQHandler
 * 描述    : 串口唤醒
 * 输入	   : 无
 * 输出    : 无
**********************************************************************************/
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


//void RTCAlarm_IRQHandler(void)
//{
//		if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
//		{
//			/* Clear EXTI line17 pending bit */
//				EXTI_ClearITPendingBit(EXTI_Line17);

//				/* Check if the Wake-Up flag is set */
//				if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
//				{
//					/* Clear Wake Up flag */
//						PWR_ClearFlag(PWR_FLAG_WU);
//				}

//				/* Wait until last write operation on RTC registers has finished */
//				RTC_WaitForLastTask();   
//				/* Clear RTC Alarm interrupt pending bit */
//				RTC_ClearITPendingBit(RTC_IT_ALR);
//				/* Wait until last write operation on RTC registers has finished */
//				RTC_WaitForLastTask();
//				
//				RCC_wake();
//				UartRecovery_Config();
//				delay_ms(5);																																			//轻微延时以避免串口丢包
//				printf("通过RTC进入正常模式!");			

//		}
//}

/********************************************************************************
 * 函数名  : RTCAlarm_IRQHandler
 * 描述    : RTC唤醒
 * 输入	   : 无
 * 输出    : 无
**********************************************************************************/
void RTCAlarm_IRQHandler(void)
{
		EXTI_ClearITPendingBit(EXTI_Line17);
		if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
		{
			/* Clear EXTI line17 pending bit */
				

				/* Check if the Wake-Up flag is set */
				if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
				{
					/* Clear Wake Up flag */
						PWR_ClearFlag(PWR_FLAG_WU);
				}

				/* Wait until last write operation on RTC registers has finished */


		}
		RTC_WaitForLastTask();   
		/* Clear RTC Alarm interrupt pending bit */
		RTC_ClearITPendingBit(RTC_IT_ALR);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		RCC_wake();
		UartRecovery_Config();
		delay_ms(5);																																			//轻微延时以避免串口丢包
		printf("通过RTC进入正常模式!");			
}




