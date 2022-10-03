#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"

/* ��ʹ�ô洢�����洢��ģʽʱ��ͨ���������ѡ */
#define DMA_CHANNEL     DMA1_Channel6
#define DMA_CLOCK       RCC_AHBPeriph_DMA1

// ������ɱ�־
#define DMA_FLAG_TC     DMA1_FLAG_TC6

// Ҫ���͵����ݴ�С
#define BUFFER_SIZE     32

extern uint32_t aDST_Buffer[BUFFER_SIZE];
extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];

/* �������� */																	
uint8_t Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength);
void DMA_Config(void);

#endif /*__DMA_H*/
