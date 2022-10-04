/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @author  lolita
  * @brief   重定向c库printf函数到usart端口
  ******************************************************************************
  */ 


#include "bsp_usart.h"

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;  
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);									/* 嵌套向量中断控制器组选择 */    
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;						/* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 			/* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 						 	/* 子优先级 */ 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 								/* 使能中断 */ 
  NVIC_Init(&NVIC_InitStructure);																	/* 初始化配置NVIC */
}


 /**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);				// 打开串口GPIO的时钟		
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);									// 打开串口外设的时钟
	
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;										// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							// 将USART Rx的GPIO配置为浮空输入模式
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);	
	/* 配置串口的工作参数	*/
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;											// 配置波特率	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											// 配置 针数据字长	
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													// 配置停止位	
	USART_InitStructure.USART_Parity = USART_Parity_No ;														// 配置校验位	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 配置硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;								 	// 配置工作模式，收发一起
	USART_Init(DEBUG_USARTx, &USART_InitStructure);																	// 完成串口的初始化配置	
	
	NVIC_Configuration();																						// 串口中断优先级配置		
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);						// 使能串口接收中断		
	USART_Cmd(DEBUG_USARTx, ENABLE);	    													// 使能串口
}



/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{	
	USART_SendData(pUSARTx,ch);																			/* 发送一个字节数据到USART */			
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	/* 等待发送数据寄存器为空 */
}
/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i = 0;
	
	for(i=0; i<num; i++)
  {	    
	    Usart_SendByte(pUSARTx,array[i]);												/* 发送一个字节数据到USART */  
  }
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);		/* 等待发送完成 */
}
/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	uint16_t k = 0;
	
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k) != '\0');   
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);		/* 等待发送完成 */
}
/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h = 0, temp_l = 0;	
	
	temp_h = (ch&0XFF00)>>8;																					/* 取出高八位 */	
	temp_l = ch&0XFF;																									/* 取出低八位 */	
	
	USART_SendData(pUSARTx,temp_h);																		/* 发送高八位 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
		
	USART_SendData(pUSARTx,temp_l);																		/* 发送低八位 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/* 重定向c库函数printf到串口，重定向后可使用printf函数 */
int fputc(int ch, FILE *f)
{
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);														/* 发送一个字节数据到串口 */				
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		/* 等待发送完毕 */
	
		return (ch);
}

/* 重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数 */
int fgetc(FILE *f)
{		
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);	/* 等待串口输入数据 */

		return (int)USART_ReceiveData(DEBUG_USARTx);
}




