/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   ����led
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"


/* �򵥵���ʱ���� */
void Delay( uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}


/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{		
	LED_GPIO_Config();	 											/* LED �˿ڳ�ʼ�� */

	while (1)
	{
		LED1(ON);			  												// ��
		Delay(0x0FFFFF);
		LED1(OFF);		   													// ��

		LED2(ON);			 													// ��
		Delay(0x0FFFFF);
		LED2(OFF);		   													// ��

		LED3(ON);			 													// ��
		Delay(0x0FFFFF);
		LED3(OFF);		   													// ��	 
																			
		LED_RED;																/*������ʾ ������������� ��ɫ*/
		Delay(0x0FFFFF);
		
		LED_GREEN;
		Delay(0x0FFFFF);
		
		LED_BLUE;
		Delay(0x0FFFFF);
		
		LED_YELLOW;
		Delay(0x0FFFFF);
		
		LED_PURPLE;
		Delay(0x0FFFFF);	
				
		LED_CYAN;
		Delay(0x0FFFFF);
		
		LED_WHITE;
		Delay(0x0FFFFF);
		
		LED_RGBOFF;
		Delay(0x0FFFFF);		
	}
}

