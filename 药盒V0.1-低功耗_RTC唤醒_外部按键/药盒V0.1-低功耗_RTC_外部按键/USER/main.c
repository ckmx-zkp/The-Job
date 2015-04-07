/******************** (C) COPYRIGHT 2014 ??DIY?? **************************
 * �ļ���  : main.c
 * ����    : ������ 
 * ��汾  : ST3.5.0
 * ����    : ���� ������
 * ����    : ����ҩ�л������ܵĲ��� 
 * �汾    : V1.0
 * ����    : 2015/4/7
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

//#################������LED�궨��######################//
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
 * ������  : UartStop_Config
 * ����    : ����������Ϊ�ⲿ�ж��Ա��ڻ���
 * ����	   : ��
 * ���    : ��
**********************************************************************************/
extern void UartStop_Config(void)
{
		printf("���ڹرմ���\r\n");
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;               												//�����ڽ�����������Ϊ�ⲿ�ж�
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            											 //��������  IO�Ǳ��ָߵ�ƽ
		GPIO_Init(GPIOA, &GPIO_InitStructure);	                		

		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);	
		EXTI_InitStructure.EXTI_Line=EXTI_Line10;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	  																				//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;										//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;					 //��ռ���ȼ����
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					        //�����ȼ����
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;											 //ʹ���ⲿ�ж�ͨ��
		NVIC_Init(&NVIC_InitStructure); 
}

/********************************************************************************
 * ������  : UartRecovery_Config
 * ����    : �����ڻָ��ĺ���
 * ����	   : ��
 * ���    : ��
**********************************************************************************/
extern void UartRecovery_Config(void)
{
//		uart_init(115200);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10
	
		EXTI_InitStructure.EXTI_Line=EXTI_Line10;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);	  	
}

	
/********************************************************************************
 * ������  : RCC_wake
 * ����    : �ָ�ϵͳʱ��
 * ����	   : ��
 * ���    : ��
**********************************************************************************/
extern void RCC_wake(void)
{
		ErrorStatus HSEStartUpStatus;
	 /* ʹ�� HSE */
		RCC_HSEConfig(RCC_HSE_ON);
	 /* �ȴ� HSE ׼������ */
		HSEStartUpStatus = RCC_WaitForHSEStartUp();
		if(HSEStartUpStatus == SUCCESS)
		{
				RCC_PLLCmd(ENABLE);																									 /* ʹ�� PLL */
				
				while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)								 /* �ȴ� PLL ׼������ */
				;
				
				RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);											/* ѡ�� PLL ��Ϊϵͳʱ��Դ */
			
			 while(RCC_GetSYSCLKSource() != 0x08) 													/* �ȴ� PLL ��ѡ��Ϊϵͳʱ��Դ */
				;
		}
}

/********************************************************************************
 * ������  : Key_Config
 * ����    : ������ʼ��
 * ����	   : ��
 * ���    : ��
**********************************************************************************/
void Key_Config(void)
{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;               																							  //KEY0 -->PC5 �˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            																							 //��������  IO�Ǳ��ָߵ�ƽ
		GPIO_Init(GPIOC, &GPIO_InitStructure);	  	
}

/********************************************************************************
 * ������  : LED_Config
 * ����    : LED��ʼ��
 * ����	   : ��
 * ���    : ��
**********************************************************************************/
void LED_Config(void)
{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;;			 																									 //LED0���-->PD5 �˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 																											    //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																													   //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOD, &GPIO_InitStructure);																																		  //�����趨������ʼ��GPIOD5
		OPEN_GREENLED;
		OPEN_REDLED;
}

/********************************************************************************
 * ������  : EXTI_Config
 * ����    : ���������ⲿ�жϳ�ʼ��
 * ����	   : ��
 * ���    : ��
**********************************************************************************/
void EXTI_Config(void)
{
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);		
	
		EXTI_InitStructure.EXTI_Line=EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	  																				//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;										//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;					 //��ռ���ȼ�2�� 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					        //�����ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;											 //ʹ���ⲿ�ж�ͨ��
		NVIC_Init(&NVIC_InitStructure); 

	
}

/********************************************************************************
 * ������  : main
 * ����    : ������
 * ����	   : ��
 * ���    : ��
 * ��ע 	 ���ȳ�ʼ������ģ��
 *				 �����°���0����͹���ģʽ
 *				 �����°���1�ָ�
 *				 �����ڷ��������Զ��ظ�
 *				 ��5����Զ��ָ�
**********************************************************************************/
int main(void)
{	
		u8 Time_Data[200];
		delay_init();
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ��PA,PC,PD�˿�ʱ��
		LED_Config();
		Key_Config();  
          																						  //����PC5
		EXTI_Config();
		uart_init(115200);
		printf("Init Uart is ok!\r\n");
		//###################################RTC��ʼ��########################################//
		while(RTC_Init())																											//RTC��ʼ��	��һ��Ҫ��ʼ���ɹ�
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
	//#####################################�ⲿ�ж�#######################################//
											
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
								printf(" ����͹���\r\n");
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
								PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);											//����ֹͣģʽ
						}
				}
		}	
}

/********************************************************************************
 * ������  : EXTI1_IRQHandler
 * ����    : ��������
 * ����	   : ��
 * ���    : ��
**********************************************************************************/
void EXTI1_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line1) != RESET) 
		{
				delay_ms(10); 																																				//����
				if(!GetKey1)
				{
						RCC_wake();
						printf("��������ģʽ!");
				}
				EXTI_ClearITPendingBit(EXTI_Line1);  																								//���EXTI1��·����λ
		}
}

/********************************************************************************
 * ������  : EXTI15_10_IRQHandler
 * ����    : ���ڻ���
 * ����	   : ��
 * ���    : ��
**********************************************************************************/
void EXTI15_10_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line10) != RESET) 
		{

				RCC_wake();
				UartRecovery_Config();
				delay_ms(5);																																			//��΢��ʱ�Ա��⴮�ڶ���
				printf("ͨ�����ڽ�������ģʽ!");
				EXTI_ClearITPendingBit(EXTI_Line10);  																								//���EXTI1��·����λ
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
//				delay_ms(5);																																			//��΢��ʱ�Ա��⴮�ڶ���
//				printf("ͨ��RTC��������ģʽ!");			

//		}
//}

/********************************************************************************
 * ������  : RTCAlarm_IRQHandler
 * ����    : RTC����
 * ����	   : ��
 * ���    : ��
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
		delay_ms(5);																																			//��΢��ʱ�Ա��⴮�ڶ���
		printf("ͨ��RTC��������ģʽ!");			
}




