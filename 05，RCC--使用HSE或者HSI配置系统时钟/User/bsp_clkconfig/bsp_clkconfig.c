#include	"bsp_clkconfig.h"

/*
 * ʹ��HSEʱ������ϵͳʱ�ӵĲ���
 * 1������HSE �����ȴ� HSE �ȶ�
 * 2������ AHB��APB2��APB1��Ԥ��Ƶ����
 * 3������PLL��ʱ����Դ����PLL�ı�Ƶ���ӣ����ø���Ƶ����Ҫ��������������
 * 4������PLL�����ȴ�PLL�ȶ�
 * 5����PLLCK�л�Ϊϵͳʱ��SYSCLK
 * 6����ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
 */

/* ���� ϵͳʱ��:SYSCLK, AHB����ʱ��:HCLK, APB2����ʱ��:PCLK2, APB1����ʱ��:PCLK1
 * PCLK2 = HCLK = SYSCLK
 * PCLK1 = HCLK/2,���ֻ����36M
 * ����˵����pllmul��PLL�ı�Ƶ���ӣ��ڵ��õ�ʱ������ǣ�RCC_PLLMul_x , x:[2,3,...16]
 * ������User_SetSysClock(RCC_PLLMul_9);  ������ϵͳʱ��Ϊ��8MHZ * 9 = 72MHZ
 *       User_SetSysClock(RCC_PLLMul_16); ������ϵͳʱ��Ϊ��8MHZ * 16 = 128MHZ����Ƶ����
 *
 * HSE��Ϊʱ����Դ������PLL��Ƶ��Ϊϵͳʱ�ӣ�����ͨ��������
 */
 
 void HSE_SetSysClock(uint32_t pllmul)
{
		uint32_t  HSEStartUpStatus = 0;
	
		RCC_DeInit();																					//��RCC�����ʼ���ɸ�λ״̬ 
		RCC_HSEConfig(RCC_HSE_ON);														//ʹ��HSE�������ⲿ���񣬱�����ʹ��8M
		HSEStartUpStatus = RCC_WaitForHSEStartUp(); 					//�ȴ� HSE �����ȶ�	
	
		if (HSEStartUpStatus == SUCCESS)												//HSE�ȶ�֮���������ִ��
		{
			FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	// ʹ��FLASH Ԥ��ȡ������
			FLASH_SetLatency(FLASH_Latency_2);										// SYSCLK�������������ʱ��ı������ã��˴����ó�2
			/* ���� AHB��APB2��APB1��Ԥ��Ƶ���� */   
			RCC_HCLKConfig(RCC_SYSCLK_Div1); 											// AHBԤ��Ƶ��������Ϊ1��Ƶ��HCLK = SYSCLK       
			RCC_PCLK2Config(RCC_HCLK_Div1); 											// APB2Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK2 = HCLK  
			RCC_PCLK1Config(RCC_HCLK_Div2); 											// APB1Ԥ��Ƶ��������Ϊ2��Ƶ��PCLK1 = HCLK/2 		

			RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllmul);				// ����PLLʱ����ԴΪHSE������PLL��Ƶ����, PLLCLK = 8MHz * pllmul    
			RCC_PLLCmd(ENABLE);																		// ����PLL     
			while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);	// �ȴ� PLL�ȶ�    
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);						// ��PLL�ȶ�֮�󣬰�PLLʱ���л�Ϊϵͳʱ��SYSCLK    
			while (RCC_GetSYSCLKSource() != 0x08);								// ��ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
		}
		else																										//������ʧ�ܣ�����������ӳ���Ĵ������
		{ 		
			while (1);																						// ������ʧ��ʱ��HSI�ᱻ�Զ�����Ϊϵͳʱ�ӣ�Ϊ8MHZ
		}
}


/*
 * ʹ��HSIʱ������ϵͳʱ�ӵĲ���
 * 1������HSI �����ȴ� HSI �ȶ�
 * 2������ AHB��APB2��APB1��Ԥ��Ƶ����
 * 3������PLL��ʱ����Դ����PLL�ı�Ƶ���ӣ����ø���Ƶ����Ҫ��������������
 * 4������PLL�����ȴ�PLL�ȶ�
 * 5����PLLCK�л�Ϊϵͳʱ��SYSCLK
 * 6����ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
 */

/* ���� ϵͳʱ��:SYSCLK, AHB����ʱ��:HCLK, APB2����ʱ��:PCLK2, APB1����ʱ��:PCLK1
 * PCLK2 = HCLK = SYSCLK
 * PCLK1 = HCLK/2,���ֻ����36M
 * ����˵����pllmul��PLL�ı�Ƶ���ӣ��ڵ��õ�ʱ������ǣ�RCC_PLLMul_x , x:[2,3,...16]
 * ������HSI_SetSysClock(RCC_PLLMul_9);  ������ϵͳʱ��Ϊ��4MHZ * 9 = 36MHZ
 *       HSI_SetSysClock(RCC_PLLMul_16); ������ϵͳʱ��Ϊ��4MHZ * 16 = 64MHZ
 *
 * HSI��Ϊʱ����Դ������PLL��Ƶ��Ϊϵͳʱ�ӣ�������HSE���ϵ�ʱ���ʹ�õķ���
 * HSI����Ϊ�¶ȵ�ԭ�����Ư�ƣ����ȶ���һ�㲻����HSI��Ϊʱ����Դ���������Ȳ����ѵ����
 * ���HSIҪ��ΪPLLʱ�ӵ���Դ�Ļ����������Ƶ֮��ſ��ԣ���HSI/2����PLL��Ƶ�������ֻ����16
 * ���Ե�ʹ��HSI��ʱ��SYSCLK���ֻ����4M*16=64M
 */
 
void HSI_SetSysClock(uint32_t pllmul)
{
	 uint32_t HSIStartUpStatus = 0;
	
	 RCC_DeInit();																							// ��RCC�����ʼ���ɸ�λ״̬  
	 RCC_HSICmd(ENABLE);																				//ʹ��HSI  
	 HSIStartUpStatus = RCC->CR & RCC_CR_HSIRDY;								// �ȴ� HSI ����	
	
   if (HSIStartUpStatus == RCC_CR_HSIRDY)											//HSI�������������ִ��
   {
    
			FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);		// ʹ��FLASH Ԥ��ȡ������
			FLASH_SetLatency(FLASH_Latency_2);											// SYSCLK�������������ʱ��ı������ã��˴����ó�2
			/* ���� AHB��APB2��APB1��Ԥ��Ƶ���� */     
			RCC_HCLKConfig(RCC_SYSCLK_Div1); 												// AHBԤ��Ƶ��������Ϊ1��Ƶ��HCLK = SYSCLK     
			RCC_PCLK2Config(RCC_HCLK_Div1); 												// APB2Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK2 = HCLK    
			RCC_PCLK1Config(RCC_HCLK_Div2);													// APB1Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK1 = HCLK/2 

			RCC_PLLConfig(RCC_PLLSource_HSI_Div2, pllmul);					// ����PLLʱ����ԴΪHSE������PLL��Ƶ����,PLLCLK = 4MHz * pllmul   
			RCC_PLLCmd(ENABLE);																			// ����PLL  
			while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); 		// �ȴ� PLL�ȶ�    
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);							// ��PLL�ȶ�֮�󣬰�PLLʱ���л�Ϊϵͳʱ��SYSCLK   
			while (RCC_GetSYSCLKSource() != 0x08);									// ��ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
	 }
	 else																												//������ʧ�ܣ�����������ӳ���Ĵ������
   { 
			while(1);																								//������ʧ��ʱ��HSI�ᱻ�Զ�����Ϊϵͳʱ�ӣ�Ϊ8MHZ
   }
}

