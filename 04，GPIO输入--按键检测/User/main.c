/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   ���԰���
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"


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
		LED_GPIO_Config();							/* LED�˿ڳ�ʼ�� */
		LED1(ON);
	
		Key_GPIO_Config();							/* �����˿ڳ�ʼ�� */
	
		while (1)												/* ��ѯ����״̬��������������תLED */
		{
				if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
				{						
						LED1_TOGGLE;						/*LED1��ת*/
				}
				
				if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
				{						
						LED2_TOGGLE;						/*LED2��ת*/
				}	
		}
}










