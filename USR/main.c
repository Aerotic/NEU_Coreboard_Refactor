/*
*Author: NEU_Aeroman 
*Date: 20180127
*Description: main() of this STM32F4 project
*/

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "usart2.h"
#include "serialprint.h"
void helloworld(void){
	printf("Rx Success!");
}
int main(void)
{	
	USART2_CFG();
	setUSART2DMARxFunc(helloworld);
	printf("Hello World!");
	//USART_SendData(USART2,0xaa);
	while(1){
	}
}
