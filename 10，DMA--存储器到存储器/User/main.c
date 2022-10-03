/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   DMA-�洢�����洢��ģʽ �����ڲ���FLASH���ڲ���SRAM
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_dma.h"


//�򵥵���ʱ����
void Delay(__IO uint32_t nCount)	 
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
  uint8_t TransferStatus;					/* �����űȽϽ������ */
  	
	LED_GPIO_Config();							/* LED �˿ڳ�ʼ�� */      
  LED_PURPLE;  										/* ����RGB��ɫ��Ϊ��ɫ */   
  Delay(0xFFFFFF);  						 	/* ����ʱ���� */  
  
  DMA_Config(); 																														/* DMA�������� */    
  while(DMA_GetFlagStatus(DMA_FLAG_TC)==RESET);															/* �ȴ�DMA������� */   
  TransferStatus = Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);	/* �Ƚ�Դ�����봫������� */    
  if(TransferStatus == 0)  																									/* �ж�Դ�����봫������ݱȽϽ��*/
	{
			LED_RED;																															/* ���ݲ����ʱRGB��ɫ����ʾ��ɫ */
	}
  else
	{
			LED_BLUE; 																														/* �������ʱRGB��ɫ����ʾ��ɫ */
	}

	while (1);
}









