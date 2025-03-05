#ifndef _ports_H_
#define _ports_H_

#include "main.h"



extern volatile bool Flag_Start_Cycle_DataReceive_SPI1;
extern volatile bool Flag_End_Cycle_DataReceive_SPI1;






void Ports_Config(void);
void EXTI9_5_IRQHandler(void);
void GPIO_Pin_4_Mode_IPUp(void) ;
void GPIO_Pin_4_Mode_IPDown(void);


#endif
