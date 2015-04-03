/******************** (C) COPYRIGHT 2013 电子DIY社团 **************************
 * 文件名  ：adc.c
 * 描述    ：ADC模块的驱动函数
 * 作者    ：张鲲鹏
 * 版本    ：V1.0
 * 日期    ：2013/11/30
 * 
**********************************************************************************/
#include "adc.h"

void ADC_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;
		__IO uint16_t ADCConvertedValue;

			/* 使能 ADC1 and GPIOC clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

		/* 配置PC4为模拟输入(ADC Channel14) */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		/* 配置ADC1, 不用DMA, 用软件触发 */
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 1;
		ADC_Init(ADC1, &ADC_InitStructure);

		/* 配置ADC1 规则通道14 channel14 configuration */
		ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);

		/* 使能ADC1 DMA功能 */
		ADC_DMACmd(ADC1, ENABLE);

		/* 使能 ADC1 */
		ADC_Cmd(ADC1, ENABLE);

		/* 使能ADC1 复位校准寄存器 */
		ADC_ResetCalibration(ADC1);
		/* 检查ADC1的复位寄存器 */
		while(ADC_GetResetCalibrationStatus(ADC1));

		/* 启动ADC1校准 */
		ADC_StartCalibration(ADC1);
		/* 检查校准是否结束 */
		while(ADC_GetCalibrationStatus(ADC1));

		/* 软件启动ADC转换 */
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

//u16 GET_ADC_Res(u8 ch)
//{
//	 //设置指定ADC的规则组通道，一个序列，采样时间
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));						//等待转换结束
//	return ADC_GetConversionValue(ADC1);								//返回最近一次ADC1规则组的转换结果
//}



/******************* (C) COPYRIGHT 2013 电子DIY社团 *****END OF FILE************/
