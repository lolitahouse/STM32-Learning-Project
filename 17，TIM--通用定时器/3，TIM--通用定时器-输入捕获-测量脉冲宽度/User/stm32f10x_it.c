/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_GeneralTim.h" 


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


void GENERAL_TIM_INT_FUN(void)
{
	// ��Ҫ��������źŵ����ڴ��ڶ�ʱ�������ʱʱ����ʱ���ͻ���������������ж�
	// ���ʱ��������Ҫ�������Ķ�ʱ���ڼӵ������źŵ�ʱ������ȥ
	if ( TIM_GetITStatus ( GENERAL_TIM, TIM_IT_Update) != RESET )               
	{	
		TIM_ICUserValueStructure.Capture_Period ++;		
		TIM_ClearITPendingBit ( GENERAL_TIM, TIM_FLAG_Update ); 		
	}
	
	if ( TIM_GetITStatus (GENERAL_TIM, GENERAL_TIM_IT_CCx ) != RESET)							// �����ز����ж�
	{	
		if ( TIM_ICUserValueStructure.Capture_StartFlag == 0 )											// ��һ�β���
		{			
			TIM_SetCounter ( GENERAL_TIM, 0 );																				// ��������0			
			TIM_ICUserValueStructure.Capture_Period = 0;															// �Զ���װ�ؼĴ������±�־��0     		
			TIM_ICUserValueStructure.Capture_CcrValue = 0;														// �沶��ȽϼĴ�����ֵ�ı�����ֵ��0				
			GENERAL_TIM_OCxPolarityConfig_FUN(GENERAL_TIM, TIM_ICPolarity_Falling);		// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���     		
			TIM_ICUserValueStructure.Capture_StartFlag = 1;														// ��ʼ�����׼��1			
		}
		// �½��ز����ж�
		else 																																				// �ڶ��β���
		{			
			TIM_ICUserValueStructure.Capture_CcrValue = 
			GENERAL_TIM_GetCapturex_FUN (GENERAL_TIM);											// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			GENERAL_TIM_OCxPolarityConfig_FUN(GENERAL_TIM, TIM_ICPolarity_Rising);      		
			TIM_ICUserValueStructure.Capture_StartFlag = 0;														// ��ʼ�����־��0      		
			TIM_ICUserValueStructure.Capture_FinishFlag = 1;													// ������ɱ�־��1		
		}
		TIM_ClearITPendingBit (GENERAL_TIM,GENERAL_TIM_IT_CCx);	    
	}		
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
