/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @author  lolita
  * @brief   i2c EEPROM(AT24C02)应用函数bsp
  ******************************************************************************
  */ 


#include "bsp_i2c.h"
#include "bsp_usart.h"

uint16_t EEPROM_ADDRESS;
static uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;  

/**
  * @brief  处理IIC等待超时情况
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示IIC读取失败.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  EEPROM_ERROR("I2C 等待超时!errorCode = %d",errorCode);					//使用串口 printf 输出错误信息，方便调试  
  return 0;
}


/**
  * @brief  I2C I/O配置
  * @param  无
  * @retval 无
  */
static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	
	EEPROM_I2C_APBxClock_FUN ( EEPROM_I2C_CLK, ENABLE );						/* 使能与 I2C 有关的时钟 */
	EEPROM_I2C_GPIO_APBxClock_FUN ( EEPROM_I2C_GPIO_CLK, ENABLE );	    
 
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;							  /* I2C_SCL、I2C_SDA*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	      		 			// 开漏输出
  GPIO_Init(EEPROM_I2C_SCL_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       					// 开漏输出
  GPIO_Init(EEPROM_I2C_SDA_PORT, &GPIO_InitStructure);			
}

/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Configu(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 
  
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;																/* I2C 配置 */		
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;												/* 高电平数据稳定，低电平数据变化 */	
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;	 
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	/* I2C的寻址模式 */	
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;															/* 通信速率 */  
  I2C_Init(EEPROM_I2Cx, &I2C_InitStructure);																/* I2C 初始化 */  
	
  I2C_Cmd(EEPROM_I2Cx, ENABLE);  																						/* 使能 I2C */ 
}


/**
  * @brief  I2C 外设(EEPROM)初始化
  * @param  无
  * @retval 无
  */
void I2C_EE_Init(void)
{
  I2C_GPIO_Config();  
  I2C_Mode_Configu();

/* 根据头文件i2c_ee.h中的定义来选择EEPROM的设备地址 */
#ifdef EEPROM_Block0_ADDRESS 
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;/* 选择 EEPROM Block0 来写入 */
#endif
	
#ifdef EEPROM_Block1_ADDRESS  	
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;/* 选择 EEPROM Block1 来写入 */
#endif

#ifdef EEPROM_Block2_ADDRESS  
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;/* 选择 EEPROM Block2 来写入 */
#endif

#ifdef EEPROM_Block3_ADDRESS  	
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;/* 选择 EEPROM Block3 来写入 */
#endif
}


/**
  * @brief   将缓冲区中的多字节数据写到I2C EEPROM中
  * @param   
  *		@arg 	 pBuffer:缓冲区指针
  *		@arg 	 WriteAddr:写地址
  *   @arg 	 NumByteToWrite:写的字节数
  * @retval  无
  */
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;							/* 取余，整数倍时addr为0 */
  count = I2C_PageSize - Addr;									/* 差count个数据值，才能对齐到页地址 */
  NumOfPage =  NumByteToWrite / I2C_PageSize;		/* 计算要写多少个整数页 */
  NumOfSingle = NumByteToWrite % I2C_PageSize;	/* 计算剩余字节数 */
 
  if(Addr == 0) 																/* addr=0，刚好按页对齐 */
  {
    
    if(NumOfPage == 0) 													/* 如果 NumByteToWrite < I2C_PageSize */
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    else																				/* 如果 NumByteToWrite > I2C_PageSize */  
    {
      while(NumOfPage--)												/* 先写整数页 */
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
				I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)												/* 写剩余的数据 */
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  else																					/* addr != 0，不按页对齐 */ 
  {  
    if(NumOfPage== 0)  													/* 若NumByteToWrite < I2C_PageSize */
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }  
    else 																				/* 若 NumByteToWrite > I2C_PageSize */
    {
      NumByteToWrite -= count;									/* 多出的count分开处理，不加入这个运算 */
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)														/* 先把WriteAddr所在页的剩余字节写了 */
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;											/* 加上后，地址就对齐到页了 */
        pBuffer += count;
      } 
      
      while(NumOfPage--)												/* 把整数页写了 */
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)											/* 写完多余的不满一页的数据 */
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}


/**
  * @brief   写一个字节到I2C EEPROM中
  * @param   
  *		@arg 	 pBuffer:缓冲区指针
  *		@arg 	 WriteAddr:写地址 
  * @retval  无
  */
uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr) 
{
  I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);																					//产生起始信号
  I2CTimeout = I2CT_FLAG_TIMEOUT;  
  
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  							/* 检测 EV5 事件并清除标志 */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  } 
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;																									//重新设置超时等待时间
  I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);		//发送设备地址   
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* 检测 EV6 事件并清除标志 */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }  
	
  I2C_SendData(EEPROM_I2Cx, WriteAddr);																						//发送要写入的 EEPROM 内部地址 
  I2CTimeout = I2CT_FLAG_TIMEOUT; 
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))					/* 检测 EV8 事件并清除标志 */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  }  
	
  I2C_SendData(EEPROM_I2Cx, *pBuffer); 																						//发送一字节要写入的数据  
  I2CTimeout = I2CT_FLAG_TIMEOUT;   
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))					/* 检测 EV8 事件并清除标志 */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  }  
  I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);																					//发送停止信号
  
  return 1;
}

/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
uint32_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
  I2CTimeout = I2CT_LONG_TIMEOUT;

  while(I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))   
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
  } 
  
  I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);																								//产生起始信号 
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) 										/* 检测 EV5 事件并清除标志 */ 
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(5);
  } 
  
  I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);					//发送 EEPROM 设备地址  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 			/* 检测 EV6 事件并清除标志 */  
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(6);
  } 
     
  I2C_SendData(EEPROM_I2Cx, WriteAddr);  																								//发送要写入的 EEPROM 内部地址
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(! I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))								/* 检测 EV8 事件并清除标志 */ 
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(7);
  } 

  while(NumByteToWrite--)  																															//循环发送 NumByteToWrite 个数据
  {
    I2C_SendData(EEPROM_I2Cx, *pBuffer); 																								//发送缓冲区中的数据
    pBuffer++; 																																					//指向缓冲区中的下一个数据
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))							/* 检测 EV8 事件并清除标志 */
    {
      if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(8);
    } 
  }
  I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);																								//发送停止信号
  
  return 1;
}


/**
  * @brief   从EEPROM里面读取一块数据 
  * @param   
  *		@arg 	 pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg 	 WriteAddr:接收数据的EEPROM的地址
  *   @arg 	 NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */
uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{    
  I2CTimeout = I2CT_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
   }
  
  I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);																							//产生起始信号 
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))										/* 检测 EV5 事件并清除标志 */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
   }

  I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);				//发送 EEPROM 设备地址 
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))			/* 检测 EV6 事件并清除标志 */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
   }   
  I2C_Cmd(EEPROM_I2Cx, ENABLE);																												//通过重新设置PE位清除 EV6 事件
	 
  I2C_SendData(EEPROM_I2Cx, ReadAddr);  																							//发送要读取的 EEPROM 内部地址   
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))							/* 检测 EV8 事件并清除标志 */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
   }
    
  I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);																							//产生第二次起始信号  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))										/* 检测 EV5 事件并清除标志 */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
   }
    
  I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Receiver);						//发送 EEPROM 设备地址  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))				/* 检测 EV6 事件并清除标志 */
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
   }
  
  while(NumByteToRead)  																															//循环发送 NumByteToWrite 个数据
  {
    if(NumByteToRead == 1)																														//表示接收到最后一个数据了，应结束传输
    {
      I2C_AcknowledgeConfig(EEPROM_I2Cx, DISABLE);																		//发送非应答信号
      I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);																					//发送停止信号
    } 
    I2CTimeout = I2CT_LONG_TIMEOUT;   
		while(I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)==0) 						/* 检测 EV7 事件并清除标志 */ 
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
		}      
		
    *pBuffer = I2C_ReceiveData(EEPROM_I2Cx);																					//通过I2C，从设备中读取一个字节的数据
     pBuffer++; 																																			//存储数据的指针指向下一个地址
     NumByteToRead--;    																															//接收数据自减      
  }
  I2C_AcknowledgeConfig(EEPROM_I2Cx, ENABLE);																					//使能应答，方便下一次I2C传输
  
  return 1;
}


/**
  * @brief  Wait for EEPROM Standby state 
  * @param  无
  * @retval 无
  */
void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;

  do
  {
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);																							//发送起始信号
    SR1_Tmp = I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1);													//读IIC SR1寄存器
    I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);				//发送 EEPROM 地址 + 写方向
  }while(!(I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1) & 0x0002));									//等待地址发送成功
  
  I2C_ClearFlag(EEPROM_I2Cx, I2C_FLAG_AF);   																						//清除AF位 
  I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE); 																								//发送停止信号
}





