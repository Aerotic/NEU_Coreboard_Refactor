/**
**Author: Aeroman
**Date:		2018-9-30
**Description: USART2 Configuration 
*/
#include "usart2.h"
 
#ifdef USART2_USEDMA
	#ifdef USART2_MODE_RX
		uint8_t USART2RxBuff[USART2_DMA_RX_BUFFSIZE];
		void (*USART2DMARxFunc)(void);
	#endif
	#ifdef USART2_MODE_TX
		uint8_t USART2TxBuff[USART2_DMA_TX_BUFFSIZE];
	#endif
#else
	#ifdef USART2_MODE_RX
		uint8_t USART2RxByte;
	#endif
#endif



u8 USART_RX_TEST;
void USART2_CFG()
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	#ifdef USART2_USEDMA
		DMA_InitTypeDef   DMA_InitStructure;
  #endif
	
	//配置时钟    
	RCC_AHB1PeriphClockCmd(USART2_RCC_AHB,ENABLE);//配置相关IO的时钟
	RCC_APB1PeriphClockCmd(USART2_RCC_APB,ENABLE);//配置USART2外设的时钟
	
	
	//使能IO的第二功能
	#ifdef USART2_MODE_RX
		GPIO_PinAFConfig(USART2_GPIO_PORT,USART2_GPIO_RXSRC ,GPIO_AF_USART2);//使能IO的第二功能
	#endif
	#ifdef USART2_MODE_TX
		GPIO_PinAFConfig(USART2_GPIO_PORT,USART2_GPIO_TXSRC ,GPIO_AF_USART2);//使能IO的第二功能
	#endif
	
	/*错误实例->GPIO_PinAFConfig(USART2_GPIO_PORT,USART2_GPIO_PINSRC ,GPIO_AF_USART2);
	**“USART2_GPIO_PINSRC”是一个宏定义，代表“GPIO_PinSource2 | GPIO_PinSource3”
	**在此特别提醒后来者，
	**GPIO_PinAFConfig()这个函数只能如上所示一个引脚一个引脚地挂可选功能(AF)
	**若如同错误实例所示那样，会导致乱码
	*/

	//配置GPIO
	GPIO_InitStructure.GPIO_Pin = USART2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(USART2_GPIO_PORT,&GPIO_InitStructure);
	
	
	//配置USART外设
	USART_DeInit(USART2);
	USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART2_WORDLEN;
	USART_InitStructure.USART_StopBits = USART2_STOPBIT;
	USART_InitStructure.USART_Parity = USART2_PARITY ;
	USART_InitStructure.USART_Mode = USART2_MODE;
	USART_InitStructure.USART_HardwareFlowControl = USART2_HWFLOWCTRL;
	USART_Init(USART2,&USART_InitStructure);
  USART_Cmd(USART2,ENABLE);
	
	
	#ifndef USART2_USEDMA//非DMA模式配置USART相关中断
		/*普通USART模式
		**接收缓冲非空中断
		*/
		#define USART2_INT USART_IT_RXNE //在此以宏的形式以保证前后中断方式一致
		USART_ITConfig(USART2,USART2_INT,ENABLE);  
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
	#elif defined USART2_USEDMA /*DMA模式配置DMA及相关中断*/
		#ifdef USART2_MODE_RX/*接收DMA配置*/
			/*配置USART2_RX_DMA
			**不定长数据接收模式，即USART空闲中断
			**最大接收长度128byte
			*/
			#define USART2_INT USART_IT_IDLE
			USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
			DMA_DeInit(DMA1_Stream5);
			DMA_InitStructure.DMA_Channel= DMA_Channel_4;
			DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
			DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART2RxBuff;
			DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
			DMA_InitStructure.DMA_BufferSize = 128;
			DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
			DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
			DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
			DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
			DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;
			DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
			DMA_Init(DMA1_Stream5,&DMA_InitStructure);
		  //中断配置
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			USART_ITConfig(USART2,USART2_INT,ENABLE);
			DMA_Cmd(DMA1_Stream5,ENABLE);
			#endif
		#ifdef USART2_MODE_TX/*发送DMA配置*/
			/*配置USART2_TX_DMA
			**定长数据接收模式，即DMA传输完成中断
			*/
			DMA_DeInit(DMA1_Stream6);
			DMA_InitStructure.DMA_Channel= DMA_Channel_4;
			DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
			DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART2TxBuff;
			DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
			DMA_InitStructure.DMA_BufferSize = USART2_DMA_TX_BUFFSIZE;
			DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
			DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
			DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
			DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
			DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;
			DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
			DMA_Init(DMA1_Stream6,&DMA_InitStructure);
			//DMA中断配置
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
			DMA_Cmd(DMA1_Stream6,DISABLE);
    #endif
  #endif
}
void USART2_IRQHandler(void)
{
	#if defined USART2_USEDMA && defined USART2_MODE_RX
		uint8_t num_1;
		uint16_t data_len_1;
		if(USART_GetITStatus(USART2, USART2_INT) != RESET){
			data_len_1 = 128 - DMA_GetCurrDataCounter(DMA1_Stream5);
			num_1 = USART2->SR;
			num_1 = USART2->DR;
			DMA_Cmd(DMA1_Stream5, DISABLE);   
			DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TEIF5 | DMA_FLAG_TCIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_HTIF5 | DMA_FLAG_FEIF5);
			DMA_SetCurrDataCounter(DMA1_Stream5,128);
			USART2DMARxFunc();
			USART_ClearITPendingBit(USART2,USART2_INT);
			DMA_Cmd(DMA1_Stream5, ENABLE);
		}
	#else
		if(USART_GetITStatus(USART2, USART2_INT) != RESET){
			USART_RX_TEST = USART_ReceiveData(USART2);
			printf("Received data is %c\n",USART_RX_TEST);		
			USART_ClearITPendingBit(USART2,USART2_INT);
		}
	#endif
}
#if defeined USART2_USEDMA && defeined USART2_MODE_TX
	void DMA1_Stream6_IRQHandler(void)
	{
		if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6))
		{
			DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
			DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
			
		}
	}
	void writeUSART2(uint8_t* dptr, int32_t dlen){
	uint8_t i;
	DMA_Cmd(DMA1_Stream6,DISABLE);
	if(dlen > USART2_DMA_TX_BUFFSIZE) dlen = USART2_DMA_TX_BUFFSIZE;
	for(i = 0; i < dlen; i++) USART2TxBuff[i] = dptr[i];
}	
#endif
void setUSART2DMARxFunc(void (*fptr)(void)){
	USART2DMARxFunc = *fptr;
}
