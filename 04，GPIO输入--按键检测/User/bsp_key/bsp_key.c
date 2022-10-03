/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  lolita
  * @brief   ����Ӧ��bsp��ɨ��ģʽ��
  ******************************************************************************
  */

#include "bsp_key.h"

/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)
{
		GPIO_InitTypeDef	GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK,ENABLE);								/*���������˿ڵ�ʱ��*/
		
		GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 																//ѡ�񰴼�������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;												//���ð���������Ϊ��������
		GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);															//ʹ�ýṹ���ʼ������
	
		GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 																//ѡ�񰴼�������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;												//���ð���������Ϊ��������
		GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);															//ʹ�ýṹ���ʼ������
}

/**
	* @brief  ����Ƿ��а�������
	* @param  GPIOx��x ������ A��B��C��D���� E
	*		     	GPIO_Pin������ȡ�Ķ˿�λ 	
	* @retval KEY_OFF(û���°���)��KEY_ON�����°�����
	*/
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
		if( GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON )											/* ����Ƿ��а������� */
		{
			 while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON); 									/* �ȴ������ͷ� */
			 return 	KEY_ON;
		}
		else
			 return 	KEY_OFF;
}










