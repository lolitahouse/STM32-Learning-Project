/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @author  lolita
  * @brief   i2c EEPROM(AT24C02)Ӧ�ú���bsp
  ******************************************************************************
  */ 


#include "bsp_i2c.h"
#include "bsp_usart.h"

uint16_t EEPROM_ADDRESS;
static uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;  

/**
  * @brief  ����IIC�ȴ���ʱ���
  * @param  errorCode��������룬����������λ���ĸ����ڳ���.
  * @retval ����0����ʾIIC��ȡʧ��.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  EEPROM_ERROR("I2C �ȴ���ʱ!errorCode = %d",errorCode);					//ʹ�ô��� printf ���������Ϣ���������  
  return 0;
}


/**
  * @brief  I2C I/O����
  * @param  ��
  * @retval ��
  */
static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	
	EEPROM_I2C_APBxClock_FUN ( EEPROM_I2C_CLK, ENABLE );						/* ʹ���� I2C �йص�ʱ�� */
	EEPROM_I2C_GPIO_APBxClock_FUN ( EEPROM_I2C_GPIO_CLK, ENABLE );	    
 
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;							  /* I2C_SCL��I2C_SDA*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	      		 			// ��©���
  GPIO_Init(EEPROM_I2C_SCL_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       					// ��©���
  GPIO_Init(EEPROM_I2C_SDA_PORT, &GPIO_InitStructure);			
}

/**
  * @brief  I2C ����ģʽ����
  * @param  ��
  * @retval ��
  */
static void I2C_Mode_Configu(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 
  
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;																/* I2C ���� */		
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;												/* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 */	
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;	 
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	/* I2C��Ѱַģʽ */	
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;															/* ͨ������ */  
  I2C_Init(EEPROM_I2Cx, &I2C_InitStructure);																/* I2C ��ʼ�� */  
	
  I2C_Cmd(EEPROM_I2Cx, ENABLE);  																						/* ʹ�� I2C */ 
}


/**
  * @brief  I2C ����(EEPROM)��ʼ��
  * @param  ��
  * @retval ��
  */
void I2C_EE_Init(void)
{
  I2C_GPIO_Config();  
  I2C_Mode_Configu();

/* ����ͷ�ļ�i2c_ee.h�еĶ�����ѡ��EEPROM���豸��ַ */
#ifdef EEPROM_Block0_ADDRESS 
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;/* ѡ�� EEPROM Block0 ��д�� */
#endif
	
#ifdef EEPROM_Block1_ADDRESS  	
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;/* ѡ�� EEPROM Block1 ��д�� */
#endif

#ifdef EEPROM_Block2_ADDRESS  
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;/* ѡ�� EEPROM Block2 ��д�� */
#endif

#ifdef EEPROM_Block3_ADDRESS  	
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;/* ѡ�� EEPROM Block3 ��д�� */
#endif
}


/**
  * @brief   ���������еĶ��ֽ�����д��I2C EEPROM��
  * @param   
  *		@arg 	 pBuffer:������ָ��
  *		@arg 	 WriteAddr:д��ַ
  *   @arg 	 NumByteToWrite:д���ֽ���
  * @retval  ��
  */
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;							/* ȡ�࣬������ʱaddrΪ0 */
  count = I2C_PageSize - Addr;									/* ��count������ֵ�����ܶ��뵽ҳ��ַ */
  NumOfPage =  NumByteToWrite / I2C_PageSize;		/* ����Ҫд���ٸ�����ҳ */
  NumOfSingle = NumByteToWrite % I2C_PageSize;	/* ����ʣ���ֽ��� */
 
  if(Addr == 0) 																/* addr=0���պð�ҳ���� */
  {
    
    if(NumOfPage == 0) 													/* ��� NumByteToWrite < I2C_PageSize */
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    else																				/* ��� NumByteToWrite > I2C_PageSize */  
    {
      while(NumOfPage--)												/* ��д����ҳ */
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
				I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)												/* дʣ������� */
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  else																					/* addr != 0������ҳ���� */ 
  {  
    if(NumOfPage== 0)  													/* ��NumByteToWrite < I2C_PageSize */
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }  
    else 																				/* �� NumByteToWrite > I2C_PageSize */
    {
      NumByteToWrite -= count;									/* �����count�ֿ������������������ */
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)														/* �Ȱ�WriteAddr����ҳ��ʣ���ֽ�д�� */
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;											/* ���Ϻ󣬵�ַ�Ͷ��뵽ҳ�� */
        pBuffer += count;
      } 
      
      while(NumOfPage--)												/* ������ҳд�� */
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)											/* д�����Ĳ���һҳ������ */
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}


/**
  * @brief   дһ���ֽڵ�I2C EEPROM��
  * @param   
  *		@arg 	 pBuffer:������ָ��
  *		@arg 	 WriteAddr:д��ַ 
  * @retval  ��
  */
uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr) 
{
  I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);																					//������ʼ�ź�
  I2CTimeout = I2CT_FLAG_TIMEOUT;  
  
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  							/* ��� EV5 �¼��������־ */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  } 
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;																									//�������ó�ʱ�ȴ�ʱ��
  I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);		//�����豸��ַ   
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* ��� EV6 �¼��������־ */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }  
	
  I2C_SendData(EEPROM_I2Cx, WriteAddr);																						//����Ҫд��� EEPROM �ڲ���ַ 
  I2CTimeout = I2CT_FLAG_TIMEOUT; 
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))					/* ��� EV8 �¼��������־ */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  }  
	
  I2C_SendData(EEPROM_I2Cx, *pBuffer); 																						//����һ�ֽ�Ҫд�������  
  I2CTimeout = I2CT_FLAG_TIMEOUT;   
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))					/* ��� EV8 �¼��������־ */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  }  
  I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);																					//����ֹͣ�ź�
  
  return 1;
}

/**
  * @brief   ��EEPROM��һ��дѭ���п���д����ֽڣ���һ��д����ֽ���
  *          ���ܳ���EEPROMҳ�Ĵ�С��AT24C02ÿҳ��8���ֽ�
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
uint32_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
  I2CTimeout = I2CT_LONG_TIMEOUT;

  while(I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))   
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
  } 
  
  I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);																								//������ʼ�ź� 
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) 										/* ��� EV5 �¼��������־ */ 
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(5);
  } 
  
  I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);					//���� EEPROM �豸��ַ  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 			/* ��� EV6 �¼��������־ */  
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(6);
  } 
     
  I2C_SendData(EEPROM_I2Cx, WriteAddr);  																								//����Ҫд��� EEPROM �ڲ���ַ
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(! I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))								/* ��� EV8 �¼��������־ */ 
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(7);
  } 

  while(NumByteToWrite--)  																															//ѭ������ NumByteToWrite ������
  {
    I2C_SendData(EEPROM_I2Cx, *pBuffer); 																								//���ͻ������е�����
    pBuffer++; 																																					//ָ�򻺳����е���һ������
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))							/* ��� EV8 �¼��������־ */
    {
      if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(8);
    } 
  }
  I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);																								//����ֹͣ�ź�
  
  return 1;
}


/**
  * @brief   ��EEPROM�����ȡһ������ 
  * @param   
  *		@arg 	 pBuffer:��Ŵ�EEPROM��ȡ�����ݵĻ�����ָ��
  *		@arg 	 WriteAddr:�������ݵ�EEPROM�ĵ�ַ
  *   @arg 	 NumByteToWrite:Ҫ��EEPROM��ȡ���ֽ���
  * @retval  ��
  */
uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{    
  I2CTimeout = I2CT_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
   }
  
  I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);																							//������ʼ�ź� 
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))										/* ��� EV5 �¼��������־ */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
   }

  I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);				//���� EEPROM �豸��ַ 
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))			/* ��� EV6 �¼��������־ */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
   }   
  I2C_Cmd(EEPROM_I2Cx, ENABLE);																												//ͨ����������PEλ��� EV6 �¼�
	 
  I2C_SendData(EEPROM_I2Cx, ReadAddr);  																							//����Ҫ��ȡ�� EEPROM �ڲ���ַ   
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))							/* ��� EV8 �¼��������־ */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
   }
    
  I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);																							//�����ڶ�����ʼ�ź�  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))										/* ��� EV5 �¼��������־ */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
   }
    
  I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Receiver);						//���� EEPROM �豸��ַ  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))				/* ��� EV6 �¼��������־ */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
   }
  
  while(NumByteToRead)  																															//ѭ������ NumByteToWrite ������
  {
    if(NumByteToRead == 1)																														//��ʾ���յ����һ�������ˣ�Ӧ��������
    {
      I2C_AcknowledgeConfig(EEPROM_I2Cx, DISABLE);																		//���ͷ�Ӧ���ź�
      I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);																					//����ֹͣ�ź�
    } 
    I2CTimeout = I2CT_LONG_TIMEOUT;   
		while(I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)==0) 						/* ��� EV7 �¼��������־ */ 
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
		}      
		
    *pBuffer = I2C_ReceiveData(EEPROM_I2Cx);																					//ͨ��I2C�����豸�ж�ȡһ���ֽڵ�����
     pBuffer++; 																																			//�洢���ݵ�ָ��ָ����һ����ַ
     NumByteToRead--;    																															//���������Լ�      
  }
  I2C_AcknowledgeConfig(EEPROM_I2Cx, ENABLE);																					//ʹ��Ӧ�𣬷�����һ��I2C����
  
  return 1;
}


/**
  * @brief  Wait for EEPROM Standby state 
  * @param  ��
  * @retval ��
  */
void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;

  do
  {
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);																							//������ʼ�ź�
    SR1_Tmp = I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1);													//��IIC SR1�Ĵ���
    I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);				//���� EEPROM ��ַ + д����
  }while(!(I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1) & 0x0002));									//�ȴ���ַ���ͳɹ�
  
  I2C_ClearFlag(EEPROM_I2Cx, I2C_FLAG_AF);   																						//���AFλ 
  I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE); 																								//����ֹͣ�ź�
}





