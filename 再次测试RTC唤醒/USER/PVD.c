#include "PWD.h"

//void PVD_Configuration(void)
//{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
//		PVD_EXTI_Config();
//		PWR_PVDLevelConfig(PWR_PVDLevel_2V9);
//		PWR_PVDCmd(ENABLE);
//}

//void PVD_EXTI_Config(void)
//{
//		EXTI_InitTypeDef EXTI_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		EXTI_ClearITPendingBit(EXTI_Line16); 
//		EXTI_InitStructure.EXTI_Line = EXTI_Line16;
//		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//		EXTI_Init(&EXTI_InitStructure);
//		
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//		
//		/* Enable the PVD Interrupt */
//		NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//		NVIC_Init(&NVIC_InitStructure);
//}