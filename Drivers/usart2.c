#include "usart2.h"
 
#ifdef USART2_USEDMA
	#ifdef USART2_MODE_RX
		uint8_t USART2RxBuff[USART2_DMA_RX_BUFFSIZE];
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
	
	    
	RCC_AHB1PeriphClockCmd(USART2_RCC_AHB,ENABLE);
	RCC_APB1PeriphClockCmd(USART2_RCC_APB,ENABLE);
	
	#ifdef USART2_MODE_RX
		GPIO_PinAFConfig(USART2_GPIO_PORT,USART2_GPIO_RXSRC ,GPIO_AF_USART2);
	#endif
	#ifdef USART2_MODE_TX
		GPIO_PinAFConfig(USART2_GPIO_PORT,USART2_GPIO_TXSRC ,GPIO_AF_USART2);
	#endif
	
//错误实例->GPIO_PinAFConfig(USART2_GPIO_PORT,USART2_GPIO_PINSRC ,GPIO_AF_USART2);
//“USART2_GPIO_PINSRC”是一个宏定义，代表“GPIO_PinSource2 | GPIO_PinSource3”
//在此特别提醒后来者，
//GPIO_PinAFConfig()这个函数只能如上所示一个引脚一个引脚地挂可选功能(AF)
//若如同错误实例所示那样，会导致乱码


	GPIO_InitStructure.GPIO_Pin = USART2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(USART2_GPIO_PORT,&GPIO_InitStructure);
	
	
	USART_DeInit(USART2);
	USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART2_WORDLEN;
	USART_InitStructure.USART_StopBits = USART2_STOPBIT;
	USART_InitStructure.USART_Parity = USART2_PARITY ;
	USART_InitStructure.USART_Mode = USART2_MODE;
	USART_InitStructure.USART_HardwareFlowControl = USART2_HWFLOWCTRL;
	USART_Init(USART2,&USART_InitStructure);
  USART_Cmd(USART2,ENABLE);
	
	USART_ITConfig(USART2,USART2_INT,ENABLE);  
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);		
}
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET){
			USART_RX_TEST = USART_ReceiveData(USART2);
			printf("Received data is %c\n",USART_RX_TEST);		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
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
#endif