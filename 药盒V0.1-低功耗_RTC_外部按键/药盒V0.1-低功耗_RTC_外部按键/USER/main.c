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

//����������Ϊ�ⲿ�ж��Ա��ڻ���
void UartStop_Config(void)
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

//�����ڻָ��ĺ���
void UartRecovery_Config(void)
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

void RTC_EXTI_Config(void)
{
		EXTI_ClearITPendingBit(EXTI_Line17);
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
		EXTI_InitStructure.EXTI_Line = EXTI_Line17;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
		NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;										//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;					 //��ռ���ȼ�2�� 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					        //�����ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;											 //ʹ���ⲿ�ж�ͨ��
		NVIC_Init(&NVIC_InitStructure); 
}
	

void RCC_wake(void)
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

void Key_Config(void)
{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;               																							  //KEY0 -->PC5 �˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            																							 //��������  IO�Ǳ��ָߵ�ƽ
		GPIO_Init(GPIOC, &GPIO_InitStructure);	  	
}

void LED_Config(void)
{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;;			 																									 //LED0���-->PD5 �˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 																											    //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																													   //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOD, &GPIO_InitStructure);																																		  //�����趨������ʼ��GPIOD5
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
		EXTI_Init(&EXTI_InitStructure);	  																				//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;										//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;					 //��ռ���ȼ�2�� 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					        //�����ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;											 //ʹ���ⲿ�ж�ͨ��
		NVIC_Init(&NVIC_InitStructure); 

	
}
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
		
		RTC_WaitForLastTask();
		RTC_SetAlarm(RTC_GetCounter()+10);
		RTC_EXTI_Config();
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
								RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
								UartStop_Config();
								PWR_WakeUpPinCmd(ENABLE);  
								PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);											//����ֹͣģʽ
						}
				}
		}	
}
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

void RTC_IRQHandler()
{
//		RCC_wake();
//		UartRecovery_Config();
//		delay_ms(5);																																			//��΢��ʱ�Ա��⴮�ڶ���
//		printf("ͨ��RTC��������ģʽ!");	
//		EXTI_ClearITPendingBit(EXTI_Line17);
		if(RTC_GetITStatus(RTC_IT_ALR) != RESET) 
		{

				RCC_wake();
				UartRecovery_Config();
				delay_ms(5);																																			//��΢��ʱ�Ա��⴮�ڶ���
				printf("ͨ��RTC��������ģʽ!");
				EXTI_ClearITPendingBit(RTCAlarm_IRQn);  																								//���EXTI1��·����λ
		}		
}



