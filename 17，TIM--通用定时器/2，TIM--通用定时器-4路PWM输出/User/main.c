/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   TIM��ͨ�ö�ʱ��-4·PWM���Ӧ��
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_GeneralTim.h" 



/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{				
	LED_GPIO_Config();				/* led �˿����� */ 		
	GENERAL_TIM_Init();				/* ��ʱ����ʼ�� */
	
  while(1)
  {     
         
  }
}






