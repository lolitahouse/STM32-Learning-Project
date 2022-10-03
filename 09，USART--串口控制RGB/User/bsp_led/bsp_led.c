/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  lolita
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  */

#include "bsp_led.h"

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
	

void LED_GPIO_Config(void)
{
		GPIO_InitTypeDef	GPIO_InitStructure;																							/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		RCC_APB2PeriphClockCmd(LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK, ENABLE);		/*����LED��ص�GPIO����ʱ��*/
	
		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;																		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;																/*��������ģʽΪͨ���������*/ 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																/*������������Ϊ50MHz */
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);																	/*���ÿ⺯������ʼ��GPIO*/	
	
		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;																		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);																	/*���ÿ⺯������ʼ��GPIO*/
				
		GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;																		/*ѡ��Ҫ���Ƶ�GPIO����*/		
		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);																	/*���ÿ⺯������ʼ��GPIO*/	
		
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);																		/* �ر�����led��	*/
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);	
		GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}





