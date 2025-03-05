#include "usart.h"
//-------------------------------------------------------------------------
void USART_SetGain_Init(void) {
USART_ClockInitTypeDef USART_ClockInitStruct;
/* USART configuration -------------------------------------------------------*/
USART_ClockInitStruct.USART_CPHA=USART_CPHA_1Edge;
USART_ClockInitStruct.USART_CPOL=USART_CPOL_Low;
USART_ClockInitStruct.USART_LastBit=USART_LastBit_Disable;
/* The synchronous mode is selected by writing the CLKEN bit in the USART_CR2 register to 1*/
USART_ClockInitStruct.USART_Clock=USART_Clock_Enable;
/*In synchronous mode, the following bits must be kept cleared:*/
// cleared LINEN bit in the USART_CR2 register
USART_LINCmd(USART3,DISABLE);
//cleared SCEN,
USART_SmartCardCmd(USART3,DISABLE);
//cleared HDSEL
USART_HalfDuplexCmd(USART3,DISABLE);
//cleared IREN bits in the USART_CR3 register.
USART_IrDACmd(USART3,DISABLE);
USART_ClockInit(USART3,&USART_ClockInitStruct);

USART_InitTypeDef USART_InitStruct;
USART_StructInit(&USART_InitStruct);
USART_InitStruct.USART_BaudRate=4500000;//4.5 MHz (for AD5160 Fclk max=25 MHz)
USART_InitStruct.USART_Mode = USART_Mode_Tx;

USART_Init(USART3, &USART_InitStruct);

/* Enable SPI */
USART_Cmd(USART3, ENABLE);
}
//-----------------------------------------------------------------------------------------------------
void USART_SetGain_SendData(uint16_t Data)
{
	GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_RESET); //SC to "0" for U1 Digital Potentiometer AD5160
	USART_SendData(USART3, Data);
	  /* Loop until the end of transmission */
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_SET); //PD2 - SC to "1" for  transfer to the internal RDAC register (U1 Digital Potentiometer AD5160)
}

