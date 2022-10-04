/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   I2C EEPROM(AT24C02)���ԣ�������Ϣͨ��USART1��ӡ�ڵ���
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_i2c.h"
#include "bsp_led.h"
#include <string.h>

#define  EEP_Firstpage      0x00
uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];
uint8_t I2C_Test(void);


/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{							
		LED_GPIO_Config(); 
		LED_BLUE;		
		USART_Config();																								/* ���ڳ�ʼ�� */		
		printf("\r\n ����һ��I2C����(AT24C02)��д�������� \r\n");		
		I2C_EE_Init();																								/* I2C �����(AT24C02)ʼ�� */
		printf("\r\n ����һ��I2C����(AT24C02)��д�������� \r\n");						 
		
		if(I2C_Test() ==1)																						//EEPROM ��д����
		{
				LED_GREEN;
		}
		else
		{
				LED_RED;
		}		
		while (1);
}


/**
  * @brief  I2C(AT24C02)��д����
  * @param  ��
  * @retval ��������1���쳣����0
  */
uint8_t I2C_Test(void)
{
	uint16_t i = 0;

	printf("д�������\n\r");    
	for ( i=0; i<=255; i++ ) 																			//��仺��
  {   
    I2c_Buf_Write[i] = i;
    printf("0x%02X ", I2c_Buf_Write[i]);
    if(i%16 == 15)    
        printf("\n\r");    
   }
  
	I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 256);				//��I2c_Buf_Write��˳�����������д��EERPOM��  
  EEPROM_INFO("\n\rд�ɹ�\n\r");  
  EEPROM_INFO("\n\r����������\n\r");  
	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256); 					//��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read��   
  
	for (i=0; i<256; i++)																					//��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			EEPROM_ERROR("0x%02X ", I2c_Buf_Read[i]);
			EEPROM_ERROR("����:I2C EEPROMд������������ݲ�һ��\n\r");
			return 0;
		}
    printf("0x%02X ", I2c_Buf_Read[i]);
    if(i%16 == 15)    
        printf("\n\r");    
	}
  EEPROM_INFO("I2C(AT24C02)��д���Գɹ�\n\r");
  
  return 1;
}







