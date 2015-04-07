/******************** (C) COPYRIGHT 2015 haohai ������ **************************
 * �ļ���  ��main.c
 * ����    �������λ�����Դ��ڵĳ���  
 * ��汾  ��ST3.5.0
 *
 * ����    : ������
**********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "delay.h"

u16 Handshake_Protocol(u16 i);

u16 GetData;
u16 SendData;



/* 
 * ��������main
 * ����  : ������
 * ����  ����
 * ���  : ��
 */
int main(void)
{
		SystemInit();
		delay_init();
		
		USART1_Config();
		printf("�����Ѵ�\r\n");
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


/******************* (C) COPYRIGHT 2015 haohai ������ *****END OF FILE************/


