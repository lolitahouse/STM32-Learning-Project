/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @author  lolita
  * @brief   �ض���c��printf������usart�˿�
  ******************************************************************************
  */ 


#include "bsp_usart.h"


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
	
	USART_Cmd(DEBUG_USARTx, ENABLE);	    																					// ʹ�ܴ���
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




