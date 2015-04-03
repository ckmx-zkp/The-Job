/******************** (C) COPYRIGHT 2013 ����DIY���� **************************
 * �ļ���  ��adc.c
 * ����    ��ADCģ�����������
 * ����    ��������
 * �汾    ��V1.0
 * ����    ��2013/11/30
 * 
**********************************************************************************/
#include "adc.h"

void ADC_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;
		__IO uint16_t ADCConvertedValue;

			/* ʹ�� ADC1 and GPIOC clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

		/* ����PC4Ϊģ������(ADC Channel14) */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		/* ����ADC1, ����DMA, ��������� */
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 1;
		ADC_Init(ADC1, &ADC_InitStructure);

		/* ����ADC1 ����ͨ��14 channel14 configuration */
		ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);

		/* ʹ��ADC1 DMA���� */
		ADC_DMACmd(ADC1, ENABLE);

		/* ʹ�� ADC1 */
		ADC_Cmd(ADC1, ENABLE);

		/* ʹ��ADC1 ��λУ׼�Ĵ��� */
		ADC_ResetCalibration(ADC1);
		/* ���ADC1�ĸ�λ�Ĵ��� */
		while(ADC_GetResetCalibrationStatus(ADC1));

		/* ����ADC1У׼ */
		ADC_StartCalibration(ADC1);
		/* ���У׼�Ƿ���� */
		while(ADC_GetCalibrationStatus(ADC1));

		/* �������ADCת�� */
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

//u16 GET_ADC_Res(u8 ch)
//{
//	 //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));						//�ȴ�ת������
//	return ADC_GetConversionValue(ADC1);								//�������һ��ADC1�������ת�����
//}



/******************* (C) COPYRIGHT 2013 ����DIY���� *****END OF FILE************/
