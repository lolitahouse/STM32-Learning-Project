#include	"bsp_mcooutput.h"

/*
 * 初始化MCO引脚PA8
 * 在F1系列中MCO引脚只有一个，即PA8，在F4系列中，MCO引脚会有两个
 */
void MCO_GPIO_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			// 开启GPIOA的时钟	
	
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;									// 选择GPIO8引脚		
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//设置为复用功能推挽输出		
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					//设置IO的翻转速率为50M
   GPIO_Init(GPIOA, &GPIO_InitStructure);											// 初始化PA8												
}

