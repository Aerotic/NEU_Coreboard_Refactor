#ifndef _Dbus_H
#define _Dbus_H

#include "board.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

extern u8 rx_buffer[32];
void Dbus_Config(void);
void DMA1_Stream1_IRQHandler(void);
//void USART3_IRQHandler(void);  
#endif /*_Dbus_H*/
