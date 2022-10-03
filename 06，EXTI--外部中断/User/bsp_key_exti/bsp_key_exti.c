/**
  ******************************************************************************
  * @file    bsp_key_exti.c
  * @author  lolita
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  */

#include "bsp_key_exti.h"


/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								/* ����NVICΪ���ȼ���1 */
  
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;  			/* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  		/* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  					/* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  							/* ʹ���ж�ͨ�� */
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;  			/* �����ж�Դ������2����������ͬ�� */  
  NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  ����IOΪEXTI�жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK | KEY2_INT_GPIO_CLK,ENABLE);			/*��������GPIO�ڵ�ʱ��*/												
	NVIC_Configuration();																											/* ���� NVIC �ж�*/	
/*--------------------------KEY1����-----------------------------*/
  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;													/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  										/* ����Ϊ�������� */	
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

  GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE); 	/* ѡ��EXTI���ź�Դ */
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;												/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;										/* �������ж� */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  																/* ʹ���ж� */	
  EXTI_Init(&EXTI_InitStructure);	
  /*--------------------------KEY2����-----------------------------*/
  GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;													/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  										/* ����Ϊ�������� */	
  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

  GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE); 	/* ѡ��EXTI���ź�Դ */
  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;												/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;										/* �½����ж� */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  																/* ʹ���ж� */	
  EXTI_Init(&EXTI_InitStructure);
}




