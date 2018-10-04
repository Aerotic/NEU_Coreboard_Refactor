/**
**Author: Aeroman
**Date:		2018-9-30
**Description: USART2 Configuration 
*/


#ifndef _USART2_H_
#define _USART2_H_

	#include "stm32f4xx.h"
	#include "stm32f4xx_conf.h"

	/**
	**Use DMA
	**If used, uncomment the macro
	*/
	#define USART2_USEDMA

	/**
	**USART mode
	**If used, uncomment corresponding macro and leave others commented
	*/
	#define USART2_MODE_TX			/*Tx Mode*/
	#define USART2_MODE_RX			/*Rx Mode*/

	/**
	**RCC settings
	*/
	#define USART2_RCC_AHB			RCC_AHB1Periph_GPIOA
	#define USART2_RCC_APB			RCC_APB1Periph_USART2

	/**
	**GPIO settings
	*/
	#define USART2_GPIO_PORT		GPIOA
	#define USART2_PIN_TX				GPIO_Pin_2
	#define USART2_PIN_RX				GPIO_Pin_3
	#define USART2_GPIO_TXSRC		GPIO_PinSource2
	#define USART2_GPIO_RXSRC		GPIO_PinSource3

	/**
	**USART settings
	*/
	#define USART2_BAUDRATE			115200
	#define USART2_WORDLEN			USART_WordLength_8b
	#define USART2_STOPBIT			USART_StopBits_1
	#define USART2_PARITY				USART_Parity_No
	#define USART2_HWFLOWCTRL		USART_HardwareFlowControl_None
		
	/**
	**DMA settings
	*/
	#ifdef USART2_USEDMA
		#define USART2_DMA_RX_BUFFSIZE			9
		#define USART2_DMA_TX_BUFFSIZE			9
		
	#endif



 /**
 **Work mode settings
 */
	#if defined USART2_MODE_TX && defined USART2_MODE_RX
		#define USART2_MODE_DX			/*Duplex Mode*/
	#endif	
	#ifdef USART2_MODE_DX
		#define USART2_GPIO_PIN			USART2_PIN_TX | USART2_PIN_RX
		#define USART2_MODE 				USART_Mode_Rx | USART_Mode_Tx
		#ifdef USART2_USEDMA
			extern uint8_t USART2RxBuff[USART2_DMA_RX_BUFFSIZE];
			extern uint8_t USART2TxBuff[USART2_DMA_TX_BUFFSIZE];
			#define USART2_INT USART_IT_IDLE
		#endif	
	#elif defined USART2_MODE_TX
		#define USART2_GPIO_PIN			USART2_PIN_TX
		#define USART2_MODE 				USART_Mode_Tx
		#ifdef USART2_USEDMA
			extern uint8_t USART2TxBuff[USART2_DMA_TX_BUFFSIZE];
		#endif
	#elif defined USART2_MODE_RX
		#define USART2_GPIO_PIN			USART2_PIN_RX
		#define USART2_MODE 			USART_Mode_Rx
		#ifdef USART2_USEDMA
			extern uint8_t USART2RxBuff[USART2_DMA_RX_BUFFSIZE];
			#define USART2_INT USART_IT_IDLE
		#else
			extern uint8_t USART2RxByte;
			#define USART2_INT USART_IT_RXNE
		#endif
	#endif

	
	
	extern u8 USART_RX_TEST;

	void USART2_CFG(void);//USART2配置
	void USART2_IRQHandler(void);//USART2中断
	#if defined USART2_MODE_TX && defined USART2_USEDMA
		void DMA1_Stream6_IRQHandler(void);
	#endif
#endif 