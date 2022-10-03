 #include "bsp_usart_dma.h"


uint8_t SendBuff[SENDBUFF_SIZE];


 /**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);				// �򿪴���GPIO��ʱ��		
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);									// �򿪴��������ʱ��
	
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;										// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);	
	/* ���ô��ڵĹ�������	*/
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;											// ���ò�����	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											// ���� �������ֳ�	
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													// ����ֹͣλ	
	USART_InitStructure.USART_Parity = USART_Parity_No ;														// ����У��λ	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// ����Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;								 	// ���ù���ģʽ���շ�һ��
	USART_Init(DEBUG_USARTx, &USART_InitStructure);																	// ��ɴ��ڵĳ�ʼ������	
	
	USART_Cmd(DEBUG_USARTx, ENABLE);	    																// ʹ�ܴ���
}



/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{	
	USART_SendData(pUSARTx,ch);																			/* ����һ���ֽ����ݵ�USART */			
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	/* �ȴ��������ݼĴ���Ϊ�� */
}
/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i = 0;
	
	for(i=0; i<num; i++)
  {	    
	    Usart_SendByte(pUSARTx,array[i]);												/* ����һ���ֽ����ݵ�USART */  
  }
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);		/* �ȴ�������� */
}
/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	uint16_t k = 0;
	
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k) != '\0');   
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);		/* �ȴ�������� */
}
/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h = 0, temp_l = 0;	
	
	temp_h = (ch&0XFF00)>>8;																					/* ȡ���߰�λ */	
	temp_l = ch&0XFF;																									/* ȡ���Ͱ�λ */	
	
	USART_SendData(pUSARTx,temp_h);																		/* ���͸߰�λ */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
		
	USART_SendData(pUSARTx,temp_l);																		/* ���͵Ͱ�λ */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/* �ض���c�⺯��printf�����ڣ��ض�����ʹ��printf���� */
int fputc(int ch, FILE *f)
{
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);														/* ����һ���ֽ����ݵ����� */				
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		/* �ȴ�������� */
	
		return (ch);
}

/* �ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ��� */
int fgetc(FILE *f)
{		
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);	/* �ȴ������������� */

		return (int)USART_ReceiveData(DEBUG_USARTx);
}


/**
  * @brief  USARTx TX DMA ���ã��ڴ浽����(USART1->DR)
  * @param  ��
  * @retval ��
  */
void USARTx_DMA_Config(void)
{
		DMA_InitTypeDef DMA_InitStructure;			
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);											// ����DMAʱ��
		
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_ADDRESS;						// ����DMAԴ��ַ���������ݼĴ�����ַ*/		
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SendBuff;							// �ڴ��ַ(Ҫ����ı�����ָ��)		
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											// ���򣺴��ڴ浽����			
		DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;												// �����С			 
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				// �����ַ����	   	
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									// �ڴ��ַ����	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	// �������ݵ�λ			
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 				// �ڴ����ݵ�λ		
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;													// DMAģʽ��һ�λ���ѭ��ģʽ
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;			
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 									// ���ȼ�����			
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														// ��ֹ�ڴ浽�ڴ�Ĵ���		   
		DMA_Init(USART_TX_DMA_CHANNEL, &DMA_InitStructure);											// ����DMAͨ��		
	
		DMA_Cmd (USART_TX_DMA_CHANNEL,ENABLE);																	// ʹ��DMA
}


















