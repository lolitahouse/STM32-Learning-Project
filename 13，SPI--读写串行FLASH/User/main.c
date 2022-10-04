/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   8M串行flash测试，并将测试信息通过串口1在电脑中打印出来
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_spi_flash.h"
#include "bsp_led.h"


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define BufferSize 			(countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress


/* 发送缓冲区初始化 */
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
	* @brief  比较两个缓冲区中的数据是否相等
	* @param  pBuffer1     src缓冲区指针
	*         pBuffer2     dst缓冲区指针
	*         BufferLength 缓冲区长度
	* @retval PASSED pBuffer1 等于   pBuffer2
	*         FAILED pBuffer1 不同于 pBuffer2
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
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{ 	
	LED_GPIO_Config();
	LED_BLUE;		
	USART_Config();																								/* 配置串口为：115200 8-N-1 */
	printf("\r\n 这是一个8Mbyte串行flash(W25Q64)实验 \r\n");		
	SPI_FLASH_Init();																							/* 8M串行flash W25Q64初始化 */	
	
	DeviceID = SPI_FLASH_ReadDeviceID();													/* 获取 Flash Device ID */
	Delay( 200 );		
	FlashID = SPI_FLASH_ReadID();																	/* 获取 SPI Flash ID */
	printf("\r\n FlashID is 0x%X,Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);	
	
	if (FlashID == sFLASH_ID)																			/* 检验 SPI Flash ID */
	{	
		printf("\r\n 检测到串行flash W25Q64 !\r\n");
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 								/* 擦除一个扇区 */ 				
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);	// 写一页数据到flash中，大小为256个字节
		printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);				
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		printf("\r\n 读出的数据为：%s \r\n", Rx_Buffer);		
		
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);		/* 检查写入的数据与读出的数据是否相等 */		
		if( PASSED == TransferStatus1 )
		{ 
			LED_GREEN;
			printf("\r\n 8M串行flash(W25Q64)测试成功!\n\r");
		}
		else
		{        
			LED_RED;
			printf("\r\n 8M串行flash(W25Q64)测试失败!\n\r");
		}
	}
	else
	{ 
		LED_RED;
		printf("\r\n 获取不到 W25Q64 ID!\n\r");
	}	
	while(1);  
}








