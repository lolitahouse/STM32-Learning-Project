/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   �����жϽ��ղ���
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_usart.h"


/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{							
		USART_Config();																								/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	
		Usart_SendString( DEBUG_USARTx,"����һ�������ж�ʵ��\n");			/* ����һ���ַ��� */
		printf("ʵ��ƽ̨ΪҰ��ָ����\n\n\n\n");	
		while(1);					
}










