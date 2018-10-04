#include "serialprint.h"
#if 1
#pragma import(__use_no_semihosting)                             
struct __FILE 
{ 
	int handle; 

}; 
FILE __stdout;       
    
int fputc(int ch, FILE *f)
{      
	while((SERIALPRINT_PORT->SR&0X40)==0);  
    SERIALPRINT_PORT->DR = (u8) ch;      
	return ch;
}
#endif