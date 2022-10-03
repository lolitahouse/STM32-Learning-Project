#include "bsp_systick.h"

/* Systick微秒级延时 */
void SysTick_Delay_Us(uint32_t us)
{
	uint32_t i = 0;
	SysTick_Config(72);
	
	for(i=0;i<us;i++)
	{		
		while( !((SysTick->CTRL)&(1<<16)) );				// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1	
	}	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		// 关闭SysTick定时器
}


/* Systick毫秒级延时 */
void SysTick_Delay_Ms(uint32_t ms)
{
	uint32_t i = 0;	
	SysTick_Config(72000);
	
	for(i=0;i<ms;i++)
	{				
		while( !((SysTick->CTRL)&(1<<16)) );				// 当计数器减小到0时，CRTL寄存器的位16会置1,当置1时，读取该位会清0
	}	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		// 关闭SysTick定时器
}









