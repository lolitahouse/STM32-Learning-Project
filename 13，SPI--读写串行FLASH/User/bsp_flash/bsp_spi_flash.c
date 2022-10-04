 /**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  lolita
  * @brief   spi flash 底层应用函数bsp 
  ******************************************************************************
  */


#include "bsp_spi_flash.h"

static uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{ 
  FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);					 /* 等待超时后的处理,输出错误信息 */
  return 0;
}


/**
  * @brief  SPI_FLASH初始化
  * @param  无
  * @retval 无
  */
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;	
	
	FLASH_SPI_APBxClock_FUN ( FLASH_SPI_CLK, ENABLE );																	/* 使能SPI时钟 */		
 	FLASH_SPI_CS_APBxClock_FUN ( FLASH_SPI_CS_CLK|FLASH_SPI_SCK_CLK|
																	FLASH_SPI_MISO_PIN|FLASH_SPI_MOSI_PIN, ENABLE );		/* 使能SPI引脚相关的时钟 */	
  
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;																			/* 配置SPI的 CS引脚，普通IO即可 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);	
  
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;																		/* 配置SPI的 SCK引脚*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;																		/* 配置SPI的 MISO引脚*/
  GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;																		/* 配置SPI的 MOSI引脚*/
  GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);
  
  SPI_FLASH_CS_HIGH();																																/* 停止信号 FLASH: CS引脚高电平*/
	
  /* SPI 模式配置 */
  // FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(FLASH_SPIx , &SPI_InitStructure);  
  SPI_Cmd(FLASH_SPIx , ENABLE);																												/* 使能 SPI  */
	
}


 /**
  * @brief  擦除FLASH扇区
  * @param  SectorAddr：要擦除的扇区地址
  * @retval 无
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{ 
  SPI_FLASH_WriteEnable();														/* 发送FLASH写使能命令 */
  SPI_FLASH_WaitForWriteEnd();
  /* 擦除扇区 */  
  SPI_FLASH_CS_LOW();																	/* 选择FLASH: CS低电平 */  
  SPI_FLASH_SendByte(W25X_SectorErase);								/* 发送扇区擦除指令*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16); 	/*发送擦除扇区地址的高位*/  
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);			/* 发送擦除扇区地址的中位 */  
  SPI_FLASH_SendByte(SectorAddr & 0xFF);							/* 发送擦除扇区地址的低位 */  
  SPI_FLASH_CS_HIGH();																/* 停止信号 FLASH: CS 高电平 */ 
  SPI_FLASH_WaitForWriteEnd(); 												/* 等待擦除完毕*/
}

/**
  * @brief  擦除FLASH扇区，整片擦除
  * @param  无
  * @retval 无
  */
void SPI_FLASH_BulkErase(void)
{  
  SPI_FLASH_WriteEnable();									/* 发送FLASH写使能命令 */
  /* 整块 Erase */  
  SPI_FLASH_CS_LOW();												/* 选择FLASH: CS低电平 */ 
  SPI_FLASH_SendByte(W25X_ChipErase);				/* 发送整块擦除指令*/ 
  SPI_FLASH_CS_HIGH();											/* 停止信号 FLASH: CS 高电平 */  
  SPI_FLASH_WaitForWriteEnd();							/* 等待擦除完毕*/
}

 /**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
  * @retval 无
  */
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{  
  SPI_FLASH_WriteEnable();														/* 发送FLASH写使能命令 */  
  SPI_FLASH_CS_LOW();																	/* 选择FLASH: CS低电平 */
  SPI_FLASH_SendByte(W25X_PageProgram); 							/* 写页写指令*/  
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);		/*发送写地址的高位*/  
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);			/*发送写地址的中位*/  
  SPI_FLASH_SendByte(WriteAddr & 0xFF);								/*发送写地址的低位*/
	
  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!"); 
  }  
  while (NumByteToWrite--)														/* 写入数据*/
  {   
    SPI_FLASH_SendByte(*pBuffer);											/* 发送当前要写入的字节数据 */    
    pBuffer++;																				/* 指向下一字节数据 */
  } 
  SPI_FLASH_CS_HIGH();																/* 停止信号 FLASH: CS 高电平 */  
  SPI_FLASH_WaitForWriteEnd();												/* 等待写入完毕*/
}

 /**
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  * @retval 无
  */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;	
	
  Addr = WriteAddr % SPI_FLASH_PageSize;									/*求余，若是整数倍，Addr值为0*/		
  count = SPI_FLASH_PageSize - Addr;											/*差count个数据值，刚好可以对齐到页地址*/	
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;				/*计算出要写多少整数页*/	
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;			/*计算出剩余不满一页的字节数*/
	
	
  if (Addr == 0)																					/* Addr=0,则刚好按页对齐 */
  {		
    if (NumOfPage == 0) 																	/* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else 																									/* NumByteToWrite > SPI_FLASH_PageSize */
    { 			
      while (NumOfPage--)																	/*先把整数页都写了*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }			
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);/*若有多余的不满一页的数据，把它写完*/
    }
  }	
  else 																										/* 若不按页对齐  */
  {		
    if (NumOfPage == 0)																		/* NumByteToWrite < SPI_FLASH_PageSize */
    {			
      if (NumOfSingle > count) 														/*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
      {
        temp = NumOfSingle - count;				
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);		/*先写满当前页*/				
        WriteAddr +=  count;
        pBuffer += count;				
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);		/*再写剩余的数据*/
      }
      else 																								/*当前页剩余的count个位置能写完NumOfSingle个数据*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else 																									/* NumByteToWrite > SPI_FLASH_PageSize */
    {			
      NumByteToWrite -= count;														/*地址不对齐多出的count分开处理，不加入这个运算*/
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;						
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);			/* 先写完count个数据，为的是让下一次要写的地址对齐 */						
      WriteAddr +=  count;																/* 接下来就重复地址对齐的情况 */
      pBuffer += count;			
      while (NumOfPage--)																	/*把整数页都写了*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }		
      if (NumOfSingle != 0)																/*若有多余的不满一页的数据，把它写完*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

 /**
  * @brief  读取FLASH数据
  * @param 	 pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval 无
  */
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{  
  SPI_FLASH_CS_LOW();																	/* 选择FLASH: CS低电平 */  
  SPI_FLASH_SendByte(W25X_ReadData);									/* 发送 读 指令 */  
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);		/* 发送 读 地址高位 */  
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);				/* 发送 读 地址中位 */  
  SPI_FLASH_SendByte(ReadAddr & 0xFF);								/* 发送 读 地址低位 */	
	
  while (NumByteToRead--) 														/* 读取数据 */
  {    
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);				/* 读取一个字节*/   
    pBuffer++; 																				/* 指向下一个字节缓冲区 */
  }  
  SPI_FLASH_CS_HIGH();																/* 停止信号 FLASH: CS 高电平 */
}

 /**
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
  
  SPI_FLASH_CS_LOW();																/* 开始通讯：CS低电平 */  
  SPI_FLASH_SendByte(W25X_JedecDeviceID);						/* 发送JEDEC指令，读取ID */  
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);						/* 读取一个字节数据 */ 
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);					 	/* 读取一个字节数据 */  
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);						/* 读取一个字节数据 */ 
  SPI_FLASH_CS_HIGH();															/* 停止通讯：CS高电平 */  
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;			/*把数据组合起来，作为函数的返回值*/

  return Temp;
}

 /**
  * @brief  读取FLASH Device ID
  * @param 	无
  * @retval FLASH Device ID
  */
uint32_t SPI_FLASH_ReadDeviceID(void)
{
  uint32_t Temp = 0;

  SPI_FLASH_CS_LOW();															/* 开始通讯：CS低电平 */ 

  SPI_FLASH_SendByte(W25X_DeviceID);							/* 发送指令，读取ID */
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte); 
  Temp = SPI_FLASH_SendByte(Dummy_Byte);					/* 读取数据 */
  SPI_FLASH_CS_HIGH();														/* 停止通讯：CS高电平 */

  return Temp;
}


 /**
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}


 /**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;  
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)			/* 等待发送缓冲区为空，TXE事件 */
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }
  SPI_I2S_SendData(FLASH_SPIx , byte); 																			/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */

	SPITimeout = SPIT_FLAG_TIMEOUT; 
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET) 		/* 等待接收缓冲区非空，RXNE事件 */
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }
 
  return SPI_I2S_ReceiveData(FLASH_SPIx ); 																	/* 读取数据寄存器，获取接收缓冲区数据 */
}

 /**
  * @brief  使用SPI发送两个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;  
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)			/* 等待发送缓冲区为空，TXE事件 */
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
  }	  
  SPI_I2S_SendData(FLASH_SPIx , HalfWord);																	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */

	SPITimeout = SPIT_FLAG_TIMEOUT;  
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)		/* 等待接收缓冲区非空，RXNE事件 */
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
  }
  
  return SPI_I2S_ReceiveData(FLASH_SPIx );																	/* 读取数据寄存器，获取接收缓冲区数据 */
}

 /**
  * @brief  向FLASH发送 写使能 命令
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(void)
{  
  SPI_FLASH_CS_LOW();																/* 通讯开始：CS低 */
  SPI_FLASH_SendByte(W25X_WriteEnable);							/* 发送写使能命令*/  
  SPI_FLASH_CS_HIGH();															/*通讯结束：CS高 */
}

 /**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;
  
  SPI_FLASH_CS_LOW();																/* 选择 FLASH: CS 低 */  
  SPI_FLASH_SendByte(W25X_ReadStatusReg);						/* 发送 读状态寄存器 命令 */  
  do																								/* 若FLASH忙碌，则等待 */
  {		
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	/* 读取FLASH芯片的状态寄存器 */
  }
  while ((FLASH_Status & WIP_Flag) == SET);  				/* 正在写入标志 */  
  SPI_FLASH_CS_HIGH();															/* 停止信号  FLASH: CS 高 */
}


/* 进入掉电模式 */
void SPI_Flash_PowerDown(void)   
{   
  SPI_FLASH_CS_LOW();																/* 通讯开始：CS低 */  
  SPI_FLASH_SendByte(W25X_PowerDown);								/* 发送 掉电 命令 */  
  SPI_FLASH_CS_HIGH();															/*通讯结束：CS高 */
}   

/* 唤醒 */
void SPI_Flash_WAKEUP(void)   
{  
  SPI_FLASH_CS_LOW();																/*选择 FLASH: CS 低 */  
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);				/* 发送 上电 命令 */  
  SPI_FLASH_CS_HIGH(); 															/* 停止信号 FLASH: CS 高 */
} 



