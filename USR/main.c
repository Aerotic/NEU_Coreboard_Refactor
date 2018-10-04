/*
*Author: NEU_Aeroman  东北大学陈刚团队 胡韬
*Date: 20180127
*Description: main() of this STM32F4 project
*/

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "usart2.h"
#include "serialprint.h"
int main(void)
{	
	USART2_CFG();
	printf("Hello World!");
	//USART_SendData(USART2,0xaa);
	while(1){
	}
}
