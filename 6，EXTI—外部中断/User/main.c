/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   ���԰������ж�ģʽ/EXTIģʽ��
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key_exti.h"


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
		LED_GPIO_Config();							/* LED �˿ڳ�ʼ�� */  	
		EXTI_Key_Config(); 							/* ��ʼ��EXTI�жϣ����°����ᴥ���ж� */	
	
		while(1);												/* �ȴ��жϣ�����ʹ���жϷ�ʽ��CPU������ѯ���� */
}










