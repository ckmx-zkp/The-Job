/******************** (C) COPYRIGHT 2015 haohai 张鲲鹏 **************************
 * 文件名  ：main.c
 * 描述    ：配合上位机测试串口的程序  
 * 库版本  ：ST3.5.0
 *
 * 作者    : 张鲲鹏
**********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "delay.h"

u16 Handshake_Protocol(u16 i);

u16 GetData;
u16 SendData;



/* 
 * 函数名：main
 * 描述  : 主函数
 * 输入  ：无
 * 输出  : 无
 */
int main(void)
{
		SystemInit();
		delay_init();
		
		USART1_Config();
		printf("串口已打开\r\n");
		while(1)
		{
				GetData = USART_ReceiveData(USART1);
				SendData = Handshake_Protocol(GetData);
				delay_ms(50);
		}
}


u16 Handshake_Protocol(u16 i)
{
		if(i == 123)
		{
				return 321;
		}
		else
			return 123;
}


/******************* (C) COPYRIGHT 2015 haohai 张鲲鹏 *****END OF FILE************/


