/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：用3.5.0版本建的工程模板。         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "adc.h"
#include "delay.h"

u16 Value = 0;
u16 tmp;
/* 
 * 函数名：main
 * 描述  : 主函数
 * 输入  ：无
 * 输出  : 无
 */
int main(void)
{
  	SystemInit(); 
		USART1_Config();
		ADC_Configuration();
		delay_init();
		printf("ADC ok\r\n");
		while(1)
		{
						Value = ADC_GetConversionValue(ADC1);
						tmp = (Value * 6600)/4096;
						printf("%d\r\n",tmp);
						delay_ms(500);
		}
	  // add your code here ^_^。
}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/


