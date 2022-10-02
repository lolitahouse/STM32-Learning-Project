#include	"bsp_clkconfig.h"

/*
 * 使用HSE时，设置系统时钟的步骤
 * 1、开启HSE ，并等待 HSE 稳定
 * 2、设置 AHB、APB2、APB1的预分频因子
 * 3、设置PLL的时钟来源，和PLL的倍频因子，设置各种频率主要就是在这里设置
 * 4、开启PLL，并等待PLL稳定
 * 5、把PLLCK切换为系统时钟SYSCLK
 * 6、读取时钟切换状态位，确保PLLCLK被选为系统时钟
 */

/* 设置 系统时钟:SYSCLK, AHB总线时钟:HCLK, APB2总线时钟:PCLK2, APB1总线时钟:PCLK1
 * PCLK2 = HCLK = SYSCLK
 * PCLK1 = HCLK/2,最高只能是36M
 * 参数说明：pllmul是PLL的倍频因子，在调用的时候可以是：RCC_PLLMul_x , x:[2,3,...16]
 * 举例：User_SetSysClock(RCC_PLLMul_9);  则设置系统时钟为：8MHZ * 9 = 72MHZ
 *       User_SetSysClock(RCC_PLLMul_16); 则设置系统时钟为：8MHZ * 16 = 128MHZ，超频慎用
 *
 * HSE作为时钟来源，经过PLL倍频作为系统时钟，这是通常的做法
 */
 
 void HSE_SetSysClock(uint32_t pllmul)
{
		uint32_t  HSEStartUpStatus = 0;
	
		RCC_DeInit();																					//把RCC外设初始化成复位状态 
		RCC_HSEConfig(RCC_HSE_ON);														//使能HSE，开启外部晶振，本测试使用8M
		HSEStartUpStatus = RCC_WaitForHSEStartUp(); 					//等待 HSE 启动稳定	
	
		if (HSEStartUpStatus == SUCCESS)												//HSE稳定之后继续往下执行
		{
			FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	// 使能FLASH 预存取缓冲区
			FLASH_SetLatency(FLASH_Latency_2);										// SYSCLK周期与闪存访问时间的比例设置，此处设置成2
			/* 设置 AHB、APB2、APB1的预分频因子 */   
			RCC_HCLKConfig(RCC_SYSCLK_Div1); 											// AHB预分频因子设置为1分频，HCLK = SYSCLK       
			RCC_PCLK2Config(RCC_HCLK_Div1); 											// APB2预分频因子设置为1分频，PCLK2 = HCLK  
			RCC_PCLK1Config(RCC_HCLK_Div2); 											// APB1预分频因子设置为2分频，PCLK1 = HCLK/2 		

			RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllmul);				// 设置PLL时钟来源为HSE，设置PLL倍频因子, PLLCLK = 8MHz * pllmul    
			RCC_PLLCmd(ENABLE);																		// 开启PLL     
			while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);	// 等待 PLL稳定    
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);						// 当PLL稳定之后，把PLL时钟切换为系统时钟SYSCLK    
			while (RCC_GetSYSCLKSource() != 0x08);								// 读取时钟切换状态位，确保PLLCLK被选为系统时钟
		}
		else																										//若开启失败，可在这里添加出错的处理代码
		{ 		
			while (1);																						// 当开启失败时，HSI会被自动设置为系统时钟，为8MHZ
		}
}


/*
 * 使用HSI时，设置系统时钟的步骤
 * 1、开启HSI ，并等待 HSI 稳定
 * 2、设置 AHB、APB2、APB1的预分频因子
 * 3、设置PLL的时钟来源，和PLL的倍频因子，设置各种频率主要就是在这里设置
 * 4、开启PLL，并等待PLL稳定
 * 5、把PLLCK切换为系统时钟SYSCLK
 * 6、读取时钟切换状态位，确保PLLCLK被选为系统时钟
 */

/* 设置 系统时钟:SYSCLK, AHB总线时钟:HCLK, APB2总线时钟:PCLK2, APB1总线时钟:PCLK1
 * PCLK2 = HCLK = SYSCLK
 * PCLK1 = HCLK/2,最高只能是36M
 * 参数说明：pllmul是PLL的倍频因子，在调用的时候可以是：RCC_PLLMul_x , x:[2,3,...16]
 * 举例：HSI_SetSysClock(RCC_PLLMul_9);  则设置系统时钟为：4MHZ * 9 = 36MHZ
 *       HSI_SetSysClock(RCC_PLLMul_16); 则设置系统时钟为：4MHZ * 16 = 64MHZ
 *
 * HSI作为时钟来源，经过PLL倍频作为系统时钟，这是在HSE故障的时候才使用的方法
 * HSI会因为温度等原因会有漂移，不稳定，一般不会用HSI作为时钟来源，除非是迫不得已的情况
 * 如果HSI要作为PLL时钟的来源的话，必须二分频之后才可以，即HSI/2，而PLL倍频因子最大只能是16
 * 所以当使用HSI的时候，SYSCLK最大只能是4M*16=64M
 */
 
void HSI_SetSysClock(uint32_t pllmul)
{
	 uint32_t HSIStartUpStatus = 0;
	
	 RCC_DeInit();																							// 把RCC外设初始化成复位状态  
	 RCC_HSICmd(ENABLE);																				//使能HSI  
	 HSIStartUpStatus = RCC->CR & RCC_CR_HSIRDY;								// 等待 HSI 就绪	
	
   if (HSIStartUpStatus == RCC_CR_HSIRDY)											//HSI就绪后继续往下执行
   {
    
			FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);		// 使能FLASH 预存取缓冲区
			FLASH_SetLatency(FLASH_Latency_2);											// SYSCLK周期与闪存访问时间的比例设置，此处设置成2
			/* 设置 AHB、APB2、APB1的预分频因子 */     
			RCC_HCLKConfig(RCC_SYSCLK_Div1); 												// AHB预分频因子设置为1分频，HCLK = SYSCLK     
			RCC_PCLK2Config(RCC_HCLK_Div1); 												// APB2预分频因子设置为1分频，PCLK2 = HCLK    
			RCC_PCLK1Config(RCC_HCLK_Div2);													// APB1预分频因子设置为1分频，PCLK1 = HCLK/2 

			RCC_PLLConfig(RCC_PLLSource_HSI_Div2, pllmul);					// 设置PLL时钟来源为HSE，设置PLL倍频因子,PLLCLK = 4MHz * pllmul   
			RCC_PLLCmd(ENABLE);																			// 开启PLL  
			while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); 		// 等待 PLL稳定    
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);							// 当PLL稳定之后，把PLL时钟切换为系统时钟SYSCLK   
			while (RCC_GetSYSCLKSource() != 0x08);									// 读取时钟切换状态位，确保PLLCLK被选为系统时钟
	 }
	 else																												//若开启失败，可在这里添加出错的处理代码
   { 
			while(1);																								//当开启失败时，HSI会被自动设置为系统时钟，为8MHZ
   }
}

