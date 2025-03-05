#ifndef _SystemM_H_
#define _SystemM_H_

#include "main.h"

//----------------------------------------------------------------------

#define GetNumBit(e)  (log2(e)-1)
// Convert PERI address - Using bit-banding
#define  DMA_CCR1_EN_bit             GetNumBit(DMA_CCR1_EN) // DMA Channel enable
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))

//bit DMA_CCR1_EN reg. DMA_CCR1 for enable DMA1 Channel5, because SPI2/I2S2_TX use this channel for DMA in mode transmit
#define DMA1_Channel5_Enable_Bit *((volatile unsigned char *)(BITBAND_PERI(DMA1_Channel5_BASE, DMA_CCR1_EN_bit)))
#define DMA1_Channel5_Enable_AddressBit (BITBAND_PERI(DMA1_Channel5_BASE, DMA_CCR1_EN_bit))


#define  SPI_CR2_TXDMAEN_bit         GetNumBit(SPI_CR2_TXDMAEN)      //SPI2 Tx buffer DMA enabled
//bit SPI_CR2_TXDMAEN reg. SPI_CR2 for SPI2 Tx buffer DMA enabled,
#define SPI2_TX_DMA_Enable_Bit  *((volatile unsigned char *)(BITBAND_PERI(SPI2_BASE, SPI_CR2_TXDMAEN_bit)))
#define SPI2_TX_DMA_Enable_AddressBit_Macros  (BITBAND_PERI(SPI2_BASE, SPI_CR2_TXDMAEN_bit))

//--------------------------------------------------------------
//Temp_for_Enable_TXDMAEN_SPI2 ('1') to address of the TXDMAEN bit reg. SPI_CR2 for Enable Tx buffer SPI2
//u32 Temp_for_Enable_TXDMAEN_SPI2, temp1;
//u32 SPI2_TX_DMA_Enable_AddressBit;


//for system timer
//#define Koef_SysClock                          SystemCoreClock/8000000U
//u32 tt;
//u32 Koef_SysClock;
#define Koef_SysClock                                (SystemCoreClock/8000000U)
//static volatile u32 usTicks, startTicks;
void SysTick_Handler(void);
void delay_us(u32 inValue);
void Setdelay_us(u32 inValue);
void delay_us1(u32 inValue);

#define EXTI9_5_IRQn                 23     /*!< External Line[9:5] Interrupts
                  */
#define SizeDataPauseMax            4802//2 слова на команду и кол-во
void NVIC_GenerateSystemReset(void);
void NVIC_GenerateCoreReset(void);

void Init_System_Clock(void);

void NVIC_Configuration(void);

//u32 MemoryBaseAddr_DMA;

extern volatile bool FlagDMA1_Channel3_Trans_Yes;
extern volatile bool FlagDMA1_Channel2_Trans_Yes;
extern volatile bool FlagDMA1_Channel6_Trans_Yes;
extern volatile bool FlagDMA1_Channel5_Trans_Yes;
extern volatile bool FlagDMA1_Channel4_Trans_Yes;
extern volatile bool FlagDMA2_Channel2_Trans_Yes;
extern volatile bool FlagDMA2_Channel1_Trans_Yes;

void DMA1_Chan1_init(void);
void DMA1_Chan1_init_set(u32 MemoryBaseAddr,u16 BufferSize);
void DMA1_Chan3_init_set8_16(u32 MemoryBaseAddr, u16 BufferSize, u16 DataSizePer, u16 DataSizeMem);

void DMA2_Chan5_init(void);
void DMA2_Chan5_init_set(u32 MemoryBaseAddr,u16 BufferSize);

void DMA1_Chan3_init(void);
//void DMA1_Chan3_init_set(u16 BufferSize);
void DMA1_Chan3_init_set(u32 MemoryBaseAddr, u16 BufferSize);
void DMA1_Channel3_IRQHandler(void);

void DMA1_Chan2_init_set(u32 MemoryBaseAddr, u16 BufferSize, bool MemoryInc);
void DMA1_Chan2_init_set8_16(u32 MemoryBaseAddr, u16 BufferSize, bool MemoryInc, u16 DataSizePer, u16 DataSizeMem);

void DMA1_Chan6_init(void);
void DMA1_Channel6_IRQHandler(void);

extern volatile u16 Count_DMA_TIM3;

void DMA1_Chan5_init(void);

void DMA1_Chan4_init(void);

void DMA2_Chan1_init(void);
void DMA2_Chan2_init(void);

extern volatile u32 indexForDataPause;
extern volatile u32 indexForDataPause1;

extern volatile bool FlagForDataPause;

#endif
