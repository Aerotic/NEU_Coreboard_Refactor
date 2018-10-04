#ifndef __USART1_H
#define	__USART1_H

#include "stm32f4xx_conf.h"
#include <stdio.h>

void Usart1_Config(void);
void Usart1_SendChar(unsigned char b);
int fputc(int ch, FILE *f);

#endif /* __USART1_H */
