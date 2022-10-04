/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   8M����flash���ԣ�����������Ϣͨ������1�ڵ����д�ӡ����
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_spi_flash.h"
#include "bsp_led.h"


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* ��ȡ�������ĳ��� */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define BufferSize 			(countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress


/* ���ͻ�������ʼ�� */
uint8_t Tx_Buffer[] = "this is a test routine\r\n";
uint8_t Rx_Buffer[BufferSize];

uint32_t DeviceID = 0;
uint32_t FlashID = 0;
TestStatus TransferStatus1 = FAILED;


void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


/**
	* @brief  �Ƚ������������е������Ƿ����
	* @param  pBuffer1     src������ָ��
	*         pBuffer2     dst������ָ��
	*         BufferLength ����������
	* @retval PASSED pBuffer1 ����   pBuffer2
	*         FAILED pBuffer1 ��ͬ�� pBuffer2
	*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}


/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{ 	
	LED_GPIO_Config();
	LED_BLUE;		
	USART_Config();																								/* ���ô���Ϊ��115200 8-N-1 */
	printf("\r\n ����һ��8Mbyte����flash(W25Q64)ʵ�� \r\n");		
	SPI_FLASH_Init();																							/* 8M����flash W25Q64��ʼ�� */	
	
	DeviceID = SPI_FLASH_ReadDeviceID();													/* ��ȡ Flash Device ID */
	Delay( 200 );		
	FlashID = SPI_FLASH_ReadID();																	/* ��ȡ SPI Flash ID */
	printf("\r\n FlashID is 0x%X,Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);	
	
	if (FlashID == sFLASH_ID)																			/* ���� SPI Flash ID */
	{	
		printf("\r\n ��⵽����flash W25Q64 !\r\n");
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 								/* ����һ������ */ 				
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);	// дһҳ���ݵ�flash�У���СΪ256���ֽ�
		printf("\r\n д�������Ϊ��%s \r\t", Tx_Buffer);				
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);		/* ���ո�д������ݶ������ŵ����ջ������� */
		printf("\r\n ����������Ϊ��%s \r\n", Rx_Buffer);		
		
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);		/* ���д�������������������Ƿ���� */		
		if( PASSED == TransferStatus1 )
		{ 
			LED_GREEN;
			printf("\r\n 8M����flash(W25Q64)���Գɹ�!\n\r");
		}
		else
		{        
			LED_RED;
			printf("\r\n 8M����flash(W25Q64)����ʧ��!\n\r");
		}
	}
	else
	{ 
		LED_RED;
		printf("\r\n ��ȡ���� W25Q64 ID!\n\r");
	}	
	while(1);  
}








