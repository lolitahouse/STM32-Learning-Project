#include "bsp_systick.h"

/* Systick΢�뼶��ʱ */
void SysTick_Delay_Us(uint32_t us)
{
	uint32_t i = 0;
	SysTick_Config(72);
	
	for(i=0;i<us;i++)
	{		
		while( !((SysTick->CTRL)&(1<<16)) );				// ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1	
	}	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		// �ر�SysTick��ʱ��
}


/* Systick���뼶��ʱ */
void SysTick_Delay_Ms(uint32_t ms)
{
	uint32_t i = 0;	
	SysTick_Config(72000);
	
	for(i=0;i<ms;i++)
	{				
		while( !((SysTick->CTRL)&(1<<16)) );				// ����������С��0ʱ��CRTL�Ĵ�����λ16����1,����1ʱ����ȡ��λ����0
	}	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		// �ر�SysTick��ʱ��
}









