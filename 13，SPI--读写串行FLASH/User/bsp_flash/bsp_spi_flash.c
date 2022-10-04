 /**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  lolita
  * @brief   spi flash �ײ�Ӧ�ú���bsp 
  ******************************************************************************
  */


#include "bsp_spi_flash.h"

static uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;

/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{ 
  FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);					 /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  return 0;
}


/**
  * @brief  SPI_FLASH��ʼ��
  * @param  ��
  * @retval ��
  */
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;	
	
	FLASH_SPI_APBxClock_FUN ( FLASH_SPI_CLK, ENABLE );																	/* ʹ��SPIʱ�� */		
 	FLASH_SPI_CS_APBxClock_FUN ( FLASH_SPI_CS_CLK|FLASH_SPI_SCK_CLK|
																	FLASH_SPI_MISO_PIN|FLASH_SPI_MOSI_PIN, ENABLE );		/* ʹ��SPI������ص�ʱ�� */	
  
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;																			/* ����SPI�� CS���ţ���ͨIO���� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);	
  
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;																		/* ����SPI�� SCK����*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;																		/* ����SPI�� MISO����*/
  GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;																		/* ����SPI�� MOSI����*/
  GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);
  
  SPI_FLASH_CS_HIGH();																																/* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
	
  /* SPI ģʽ���� */
  // FLASHоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
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
  SPI_Cmd(FLASH_SPIx , ENABLE);																												/* ʹ�� SPI  */
	
}


 /**
  * @brief  ����FLASH����
  * @param  SectorAddr��Ҫ������������ַ
  * @retval ��
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{ 
  SPI_FLASH_WriteEnable();														/* ����FLASHдʹ������ */
  SPI_FLASH_WaitForWriteEnd();
  /* �������� */  
  SPI_FLASH_CS_LOW();																	/* ѡ��FLASH: CS�͵�ƽ */  
  SPI_FLASH_SendByte(W25X_SectorErase);								/* ������������ָ��*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16); 	/*���Ͳ���������ַ�ĸ�λ*/  
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);			/* ���Ͳ���������ַ����λ */  
  SPI_FLASH_SendByte(SectorAddr & 0xFF);							/* ���Ͳ���������ַ�ĵ�λ */  
  SPI_FLASH_CS_HIGH();																/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */ 
  SPI_FLASH_WaitForWriteEnd(); 												/* �ȴ��������*/
}

/**
  * @brief  ����FLASH��������Ƭ����
  * @param  ��
  * @retval ��
  */
void SPI_FLASH_BulkErase(void)
{  
  SPI_FLASH_WriteEnable();									/* ����FLASHдʹ������ */
  /* ���� Erase */  
  SPI_FLASH_CS_LOW();												/* ѡ��FLASH: CS�͵�ƽ */ 
  SPI_FLASH_SendByte(W25X_ChipErase);				/* �����������ָ��*/ 
  SPI_FLASH_CS_HIGH();											/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */  
  SPI_FLASH_WaitForWriteEnd();							/* �ȴ��������*/
}

 /**
  * @brief  ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
  * @retval ��
  */
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{  
  SPI_FLASH_WriteEnable();														/* ����FLASHдʹ������ */  
  SPI_FLASH_CS_LOW();																	/* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_SendByte(W25X_PageProgram); 							/* дҳдָ��*/  
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);		/*����д��ַ�ĸ�λ*/  
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);			/*����д��ַ����λ*/  
  SPI_FLASH_SendByte(WriteAddr & 0xFF);								/*����д��ַ�ĵ�λ*/
	
  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!"); 
  }  
  while (NumByteToWrite--)														/* д������*/
  {   
    SPI_FLASH_SendByte(*pBuffer);											/* ���͵�ǰҪд����ֽ����� */    
    pBuffer++;																				/* ָ����һ�ֽ����� */
  } 
  SPI_FLASH_CS_HIGH();																/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */  
  SPI_FLASH_WaitForWriteEnd();												/* �ȴ�д�����*/
}

 /**
  * @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;	
	
  Addr = WriteAddr % SPI_FLASH_PageSize;									/*���࣬������������AddrֵΪ0*/		
  count = SPI_FLASH_PageSize - Addr;											/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/	
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;				/*�����Ҫд��������ҳ*/	
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;			/*�����ʣ�಻��һҳ���ֽ���*/
	
	
  if (Addr == 0)																					/* Addr=0,��պð�ҳ���� */
  {		
    if (NumOfPage == 0) 																	/* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else 																									/* NumByteToWrite > SPI_FLASH_PageSize */
    { 			
      while (NumOfPage--)																	/*�Ȱ�����ҳ��д��*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }			
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);/*���ж���Ĳ���һҳ�����ݣ�����д��*/
    }
  }	
  else 																										/* ������ҳ����  */
  {		
    if (NumOfPage == 0)																		/* NumByteToWrite < SPI_FLASH_PageSize */
    {			
      if (NumOfSingle > count) 														/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��һҳд����*/
      {
        temp = NumOfSingle - count;				
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);		/*��д����ǰҳ*/				
        WriteAddr +=  count;
        pBuffer += count;				
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);		/*��дʣ�������*/
      }
      else 																								/*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else 																									/* NumByteToWrite > SPI_FLASH_PageSize */
    {			
      NumByteToWrite -= count;														/*��ַ����������count�ֿ������������������*/
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;						
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);			/* ��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */						
      WriteAddr +=  count;																/* ���������ظ���ַ�������� */
      pBuffer += count;			
      while (NumOfPage--)																	/*������ҳ��д��*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }		
      if (NumOfSingle != 0)																/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

 /**
  * @brief  ��ȡFLASH����
  * @param 	 pBuffer���洢�������ݵ�ָ��
  * @param   ReadAddr����ȡ��ַ
  * @param   NumByteToRead����ȡ���ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{  
  SPI_FLASH_CS_LOW();																	/* ѡ��FLASH: CS�͵�ƽ */  
  SPI_FLASH_SendByte(W25X_ReadData);									/* ���� �� ָ�� */  
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);		/* ���� �� ��ַ��λ */  
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);				/* ���� �� ��ַ��λ */  
  SPI_FLASH_SendByte(ReadAddr & 0xFF);								/* ���� �� ��ַ��λ */	
	
  while (NumByteToRead--) 														/* ��ȡ���� */
  {    
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);				/* ��ȡһ���ֽ�*/   
    pBuffer++; 																				/* ָ����һ���ֽڻ����� */
  }  
  SPI_FLASH_CS_HIGH();																/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
}

 /**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
  
  SPI_FLASH_CS_LOW();																/* ��ʼͨѶ��CS�͵�ƽ */  
  SPI_FLASH_SendByte(W25X_JedecDeviceID);						/* ����JEDECָ���ȡID */  
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);						/* ��ȡһ���ֽ����� */ 
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);					 	/* ��ȡһ���ֽ����� */  
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);						/* ��ȡһ���ֽ����� */ 
  SPI_FLASH_CS_HIGH();															/* ֹͣͨѶ��CS�ߵ�ƽ */  
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;			/*�����������������Ϊ�����ķ���ֵ*/

  return Temp;
}

 /**
  * @brief  ��ȡFLASH Device ID
  * @param 	��
  * @retval FLASH Device ID
  */
uint32_t SPI_FLASH_ReadDeviceID(void)
{
  uint32_t Temp = 0;

  SPI_FLASH_CS_LOW();															/* ��ʼͨѶ��CS�͵�ƽ */ 

  SPI_FLASH_SendByte(W25X_DeviceID);							/* ����ָ���ȡID */
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte); 
  Temp = SPI_FLASH_SendByte(Dummy_Byte);					/* ��ȡ���� */
  SPI_FLASH_CS_HIGH();														/* ֹͣͨѶ��CS�ߵ�ƽ */

  return Temp;
}


 /**
  * @brief  ʹ��SPI��ȡһ���ֽڵ�����
  * @param  ��
  * @retval ���ؽ��յ�������
  */
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}


 /**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;  
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)			/* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }
  SPI_I2S_SendData(FLASH_SPIx , byte); 																			/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */

	SPITimeout = SPIT_FLAG_TIMEOUT; 
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET) 		/* �ȴ����ջ������ǿգ�RXNE�¼� */
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }
 
  return SPI_I2S_ReceiveData(FLASH_SPIx ); 																	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
}

 /**
  * @brief  ʹ��SPI���������ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;  
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)			/* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
  }	  
  SPI_I2S_SendData(FLASH_SPIx , HalfWord);																	/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */

	SPITimeout = SPIT_FLAG_TIMEOUT;  
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)		/* �ȴ����ջ������ǿգ�RXNE�¼� */
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
  }
  
  return SPI_I2S_ReceiveData(FLASH_SPIx );																	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
}

 /**
  * @brief  ��FLASH���� дʹ�� ����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(void)
{  
  SPI_FLASH_CS_LOW();																/* ͨѶ��ʼ��CS�� */
  SPI_FLASH_SendByte(W25X_WriteEnable);							/* ����дʹ������*/  
  SPI_FLASH_CS_HIGH();															/*ͨѶ������CS�� */
}

 /**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;
  
  SPI_FLASH_CS_LOW();																/* ѡ�� FLASH: CS �� */  
  SPI_FLASH_SendByte(W25X_ReadStatusReg);						/* ���� ��״̬�Ĵ��� ���� */  
  do																								/* ��FLASHæµ����ȴ� */
  {		
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	/* ��ȡFLASHоƬ��״̬�Ĵ��� */
  }
  while ((FLASH_Status & WIP_Flag) == SET);  				/* ����д���־ */  
  SPI_FLASH_CS_HIGH();															/* ֹͣ�ź�  FLASH: CS �� */
}


/* �������ģʽ */
void SPI_Flash_PowerDown(void)   
{   
  SPI_FLASH_CS_LOW();																/* ͨѶ��ʼ��CS�� */  
  SPI_FLASH_SendByte(W25X_PowerDown);								/* ���� ���� ���� */  
  SPI_FLASH_CS_HIGH();															/*ͨѶ������CS�� */
}   

/* ���� */
void SPI_Flash_WAKEUP(void)   
{  
  SPI_FLASH_CS_LOW();																/*ѡ�� FLASH: CS �� */  
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);				/* ���� �ϵ� ���� */  
  SPI_FLASH_CS_HIGH(); 															/* ֹͣ�ź� FLASH: CS �� */
} 



