#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"


/* ����LED���ӵ�GPIO�˿�*/

// R-��ɫ
#define LED1_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LED1_GPIO_PIN				GPIO_Pin_5			        /* ���ӵ�SCLʱ���ߵ�GPIO */

// G-��ɫ
#define LED2_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LED2_GPIO_PIN				GPIO_Pin_0			        /* ���ӵ�SCLʱ���ߵ�GPIO */

// B-��ɫ
#define LED3_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LED3_GPIO_PIN				GPIO_Pin_1			        /* ���ӵ�SCLʱ���ߵ�GPIO */


/* �͵�ƽ�� */
#define ON  	0
#define OFF 	1

/* ���κ궨�� */
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3(a)	if (a)	\
					GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)
					
/* ���巴ת״̬�� */
#define LED1_TOGGLE		 { LED1_GPIO_PORT->ODR ^= LED1_GPIO_PIN; } 			//�����ת״̬
#define LED2_TOGGLE		 { LED2_GPIO_PORT->ODR ^= LED2_GPIO_PIN; }
#define LED3_TOGGLE		 { LED3_GPIO_PORT->ODR ^= LED3_GPIO_PIN; }

/* ������ɫ */


#define LED_RED  				{ LED1(ON); LED2(OFF); LED3(OFF); }					//��
#define LED_GREEN				{ LED1(OFF); LED2(ON); LED3(OFF); }				  //��
#define LED_BLUE				{ LED1(OFF); LED2(OFF); LED3(ON); }					//��					
					
#define LED_YELLOW			{ LED1(ON); LED2(ON); LED3(OFF); }					//��(��+��)
#define LED_PURPLE			{ LED1(ON); LED2(OFF); LED3(ON); }					//��(��+��)
#define LED_CYAN 				{ LED1(OFF); LED2(ON); LED3(ON); }					//��(��+��)					

#define LED_WHITE				{ LED1(ON); LED2(ON); LED3(ON); }						//��(��+��+��)					
#define LED_RGBOFF			{ LED1(OFF); LED2(OFF); LED3(OFF); }				//��(ȫ���ر�)

/* �������� */
void LED_GPIO_Config(void);


#endif /*__LED_H*/
