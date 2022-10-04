#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"


/* 定义LED连接的GPIO端口*/

// R-红色
#define LED1_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED1_GPIO_PIN				GPIO_Pin_5			        /* 连接到SCL时钟线的GPIO */

// G-绿色
#define LED2_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED2_GPIO_PIN				GPIO_Pin_0			        /* 连接到SCL时钟线的GPIO */

// B-蓝色
#define LED3_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED3_GPIO_PIN				GPIO_Pin_1			        /* 连接到SCL时钟线的GPIO */


/* 低电平开 */
#define ON  	0
#define OFF 	1

/* 带参宏定义 */
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
					
/* 定义反转状态宏 */
#define LED1_TOGGLE		 { LED1_GPIO_PORT->ODR ^= LED1_GPIO_PIN; } 			//输出反转状态
#define LED2_TOGGLE		 { LED2_GPIO_PORT->ODR ^= LED2_GPIO_PIN; }
#define LED3_TOGGLE		 { LED3_GPIO_PORT->ODR ^= LED3_GPIO_PIN; }

/* 基本混色 */


#define LED_RED  				{ LED1(ON); LED2(OFF); LED3(OFF); }					//红
#define LED_GREEN				{ LED1(OFF); LED2(ON); LED3(OFF); }				  //绿
#define LED_BLUE				{ LED1(OFF); LED2(OFF); LED3(ON); }					//蓝					
					
#define LED_YELLOW			{ LED1(ON); LED2(ON); LED3(OFF); }					//黄(红+绿)
#define LED_PURPLE			{ LED1(ON); LED2(OFF); LED3(ON); }					//紫(红+蓝)
#define LED_CYAN 				{ LED1(OFF); LED2(ON); LED3(ON); }					//青(绿+蓝)					

#define LED_WHITE				{ LED1(ON); LED2(ON); LED3(ON); }						//白(红+绿+蓝)					
#define LED_RGBOFF			{ LED1(OFF); LED2(OFF); LED3(OFF); }				//黑(全部关闭)

/* 函数声明 */
void LED_GPIO_Config(void);


#endif /*__LED_H*/
