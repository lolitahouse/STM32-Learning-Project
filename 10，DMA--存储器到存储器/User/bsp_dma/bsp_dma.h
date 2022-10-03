#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"

/* 当使用存储器到存储器模式时候，通道可以随便选 */
#define DMA_CHANNEL     DMA1_Channel6
#define DMA_CLOCK       RCC_AHBPeriph_DMA1

// 传输完成标志
#define DMA_FLAG_TC     DMA1_FLAG_TC6

// 要发送的数据大小
#define BUFFER_SIZE     32

extern uint32_t aDST_Buffer[BUFFER_SIZE];
extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];

/* 函数声明 */																	
uint8_t Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength);
void DMA_Config(void);

#endif /*__DMA_H*/
