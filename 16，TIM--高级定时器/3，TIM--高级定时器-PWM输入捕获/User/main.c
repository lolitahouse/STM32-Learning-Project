/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   TIM���߼���ʱ��-PWM���벶��Ӧ�ã�ͨ�ö�ʱ������PWM�����߼���ʱ���򲶻����PWM�����������ں�ռ�ձ�
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_AdvanceTim.h"
#include "bsp_usart.h"
#include "bsp_GeneralTim.h" 


/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	USART_Config();								/* ���ڳ�ʼ�� */		
	GENERAL_TIM_Init();						/* ͨ�ö�ʱ����ʼ������������PWM�ź� */		
	ADVANCE_TIM_Init();						/* �߼���ʱ����ʼ�� ���û�����PWM�ź�*/
	
  while(1)
  { 
		
  }
}






