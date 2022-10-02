/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   ����beep
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_beep.h"


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
	
	BEEP_GPIO_Config();			/* BEEP GPIO ��ʼ�� */
	
	while(1)
	{		
		BEEP( ON ); 			  // ��
		Delay(0x0FFFFF);	
		BEEP( OFF );		  // ����
		Delay(0x0FFFFF);
	}
}

