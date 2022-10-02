/**
  ******************************************************************************
  * @file    bsp_beep.c
  * @author  lolita
  * @brief   ������Ӧ�ú����ӿ�
  ******************************************************************************
  */

#include "bsp_beep.h"

/**
  * @brief  ��ʼ�����Ʒ�������IO
  * @param  ��
  * @retval ��
  */
void BEEP_GPIO_Config(void)
{
		
		GPIO_InitTypeDef	GPIO_InitStructure;								/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/	
		RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);			/*�������Ʒ�������GPIO�Ķ˿�ʱ��*/
	
		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;				/*ѡ��Ҫ���Ʒ�������GPIO*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/*����GPIOģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		/*����GPIO����Ϊ50MHz */
		GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);			/*���ÿ⺯������ʼ�����Ʒ�������GPIO*/
	
		GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);			/* �رշ�����*/
}

