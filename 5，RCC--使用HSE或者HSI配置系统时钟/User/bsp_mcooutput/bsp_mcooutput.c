#include	"bsp_mcooutput.h"

/*
 * ��ʼ��MCO����PA8
 * ��F1ϵ����MCO����ֻ��һ������PA8����F4ϵ���У�MCO���Ż�������
 */
void MCO_GPIO_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			// ����GPIOA��ʱ��	
	
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;									// ѡ��GPIO8����		
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//����Ϊ���ù����������		
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					//����IO�ķ�ת����Ϊ50M
   GPIO_Init(GPIOA, &GPIO_InitStructure);											// ��ʼ��PA8												
}

