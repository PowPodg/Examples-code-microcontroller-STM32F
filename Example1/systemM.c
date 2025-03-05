#include "systemM.h"

u8 temp3=0;
GPIO_InitTypeDef GPIO_InitStructure;
volatile u16 Count_DMA_TIM3;

 volatile bool FlagDMA1_Channel3_Trans_Yes;
 volatile bool FlagDMA1_Channel2_Trans_Yes;
 volatile bool FlagDMA1_Channel6_Trans_Yes;
 volatile bool FlagDMA1_Channel5_Trans_Yes;
 volatile bool FlagDMA1_Channel4_Trans_Yes;
 volatile bool FlagDMA2_Channel2_Trans_Yes;
 volatile bool FlagDMA2_Channel1_Trans_Yes;

 volatile bool FlagForDataPause=false; 

 u8 TempForDataGyr;

 volatile u8 DiffCount_Gyroscope_interrupt=0;
 volatile s16  Data_Giro_Gyroscope1=0;
 volatile s16  Data_Giro_Gyroscope2=0;
 volatile u32  CountData_Giro_Gyroscope=0;
 volatile u32 indexForDataPause=2;
 volatile u32 indexForDataPause1=2;



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define AIRCR_VECTKEY_MASK    ((u32)0x05FA0000)

/*******************************************************************************
* Function Name  : NVIC_GenerateSystemReset
* Description    : Generates a system reset.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_GenerateSystemReset(void)
{
  SCB->AIRCR = AIRCR_VECTKEY_MASK | (u32)0x04;
}
/*******************************************************************************
* Function Name  : NVIC_GenerateCoreReset
* Description    : Generates a Core (Core + NVIC) reset.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_GenerateCoreReset(void)
{
  SCB->AIRCR = AIRCR_VECTKEY_MASK | (u32)0x01;
}

//-------------------------------------------------------------------------------
static volatile ErrorStatus HSEStartUpStatus = SUCCESS;

//------------------------------------------------------------------------
void Init_System_Clock(void)
{
	  RCC_DeInit();
	 /* Enable HSE */
	  RCC_HSEConfig(RCC_HSE_ON);
	  /* Wait till HSE is ready */
	  HSEStartUpStatus = RCC_WaitForHSEStartUp();

	  if(HSEStartUpStatus == SUCCESS)
	   {
	    /* Enable Prefetch Buffer */
		 FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        /* Flash 2 wait state */
	     FLASH_SetLatency(FLASH_Latency_2);
	    /* HCLK = SYSCLK */
	     RCC_HCLKConfig(RCC_SYSCLK_Div1);
	    /* PCLK2 = HCLK */
	     RCC_PCLK2Config(RCC_HCLK_Div1);
	     /* PCLK1 = HCLK/2 */
	     RCC_PCLK1Config(RCC_HCLK_Div2);
	     /* PLLCLK = 8MHz * 9 = 72 MHz */
	     RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	     /* Enable PLL */
	     RCC_PLLCmd(ENABLE);
	     /* Wait till PLL is ready */
	     /* Wait till PLL is ready */
	     while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	     {
	     }
         /* Select PLL as system clock source */
	     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
         /* Wait till PLL is used as system clock source */
	     while(RCC_GetSYSCLKSource() != 0x08)
	     {
	     }
	   }

		  /* Enable GPIOA, GPIOB, GPIOC, GPIOD and AFIO clocks */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
	         | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	  //clock for ADC (max 14MHz --> 72/6=12MHz)
	  RCC_ADCCLKConfig (RCC_PCLK2_Div6);
	  /* TIM1, SPI1, ADC1, ADC3 Periph clock enable */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM8 | RCC_APB2Periph_SPI1 | RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC3, ENABLE); // RCC_APB2Periph_ADC3
	  /* TIM3, TIM4, TIM5, TIM2, SPI2, SPI3, WWDG, USART3 Periph clock enable */
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM5 | RCC_APB1Periph_TIM6 | RCC_APB1Periph_TIM2 |
	 RCC_APB1Periph_SPI2 | RCC_APB1Periph_SPI3 | RCC_APB1Periph_WWDG | RCC_APB1Periph_USART3, ENABLE);//
	  // Enable DMA1, DMA2 clock
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

	  /*------------------- Resources Initialization -----------------------------*/
	    /* GPIO Configuration */
//Koef_SysClock = SystemCoreClock/8000000U;


}
//-------without interrupt--------------us=inValue
void delay_us(u32 inValue)
{
	   SysTick->LOAD  = ((inValue*Koef_SysClock) & SysTick_LOAD_RELOAD_Msk) - 1;
	   SysTick->VAL=0;
	   SysTick->CTRL  = SysTick_CTRL_ENABLE_Msk; //enable count
	   while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));
	   SysTick->CTRL=0;
}
//---------------------------------------------------------------------------------
//-------without interrupt--------------us=inValue
void Setdelay_us(u32 inValue)
{
	   SysTick->LOAD  = ((inValue*Koef_SysClock) & SysTick_LOAD_RELOAD_Msk) - 1;
	   SysTick->VAL=0;
	   SysTick->CTRL  = SysTick_CTRL_ENABLE_Msk; //enable count
}

//--------------------------------------------------------------------------------------
//***************************************************************************
//Configure the nested vectored interrupt controller.
//***************************************************************************
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    // for PC4 pin4 - enable the EXTI4_IRQHandler Interrupt vector - äëÿ ïðåðûâàíèÿ îò ñèãíàëà òîêîâîé ïàóçû (ëèíèÿ 'Res')
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // for PC5 pin5 - enable the EXTI9_5 (23) Interrupt vector

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
   //  enable the TIM1 Capture Compare Interrupt vector
     NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);

  //  enable the DMA1_Channel6 for TIM3 CH! Interrupt vector
     NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);

     //  enable the DMA1_Channel3 for SPI1 Command transmit!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        //  enable the DMA1_Channel3 for SPI1 Rx!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
           NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
           NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
           NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
           NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
           NVIC_Init(&NVIC_InitStructure);

     //  enable the DMA1_Channel5 for SPI2 Tx Interrupt vector
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
     //  enable the DMA1_Channel4 for SPI2 Rx Interrupt vector
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        //  enable the DMA2_Channel2 for SPI3 Tx Interrupt vector
           NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel2_IRQn;
           NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
           NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
           NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
           NVIC_Init(&NVIC_InitStructure);
        //  enable the DMA2_Channel1 for SPI3 Rx Interrupt vector
           NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel1_IRQn;
           NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
           NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
           NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
           NVIC_Init(&NVIC_InitStructure);


     //  enable the TIM3_IRQn for TIM3 Interrupt vector
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        //  enable the TIM4_IRQn for TIM4 Interrupt vector
           NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
           NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
           NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
           NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
           NVIC_Init(&NVIC_InitStructure);

           //  enable the TIM2_IRQn for TIM2 Interrupt vector
              NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
              NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
              NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
              NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
              NVIC_Init(&NVIC_InitStructure);
}
//------------------------------------------------------------------------------------------
void DMA1_Chan1_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	  DMA_DeInit(DMA1_Channel1);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	//  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&Data_ADC1_Data_I_DMA[0];
	  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Data_ADC_Data_IU_DMA[0];
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //ADC1 -> memory
	  DMA_InitStructure.DMA_BufferSize = 1;//QuantityReceiveDataUI_SPI;   //Quantity of data to be transferred
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //Peripheral data size - 16 bits
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //Memory data size - 16 bits
	  //In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //Memory to memory mode disabled
	  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	  /* Enable DMA1 channel1 */
	  DMA_Cmd(DMA1_Channel1, ENABLE);


}
//------------------------------------------------------------------------------------------
void DMA1_Chan1_init_set(u32 MemoryBaseAddr, u16 BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;

	  DMA_DeInit(DMA1_Channel1);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
	//  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&Data_ADC1_Data_I_DMA[0];
	  DMA_InitStructure.DMA_MemoryBaseAddr = MemoryBaseAddr;//(u32)&Data_ADC_Data_IU_DMA[0];
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //ADC1 -> memory
	  DMA_InitStructure.DMA_BufferSize = BufferSize;//QuantityReceiveDataUI_SPI;   //Quantity of data to be transferred
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //Peripheral data size - 16 bits
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //Memory data size - 16 bits
	  //In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //Memory to memory mode disabled
	  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	  /* Enable DMA1 channel1 */
	  DMA_Cmd(DMA1_Channel1, ENABLE);

	  // Enable ADC1 DMA
	//  ADC_DMACmd(ADC1, ENABLE);
}
//---------------------------initial DMA2 for data U---------------------------------
void DMA2_Chan5_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	  DMA_DeInit(DMA2_Channel5);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC3->DR;
	//  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&Data_ADC3_Data_U_DMA[0];
	  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&Data_ADC_Data_IU_DMA[QuantityReceiveDataUI_SPI];
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //ADC3 -> memory
	  DMA_InitStructure.DMA_BufferSize = QuantityReceiveDataUI_SPI;//Quantity of data to be transferred
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //Peripheral data size - 16 bits
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //Memory data size - 16 bits
	  //In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //Memory to memory mode disabled
	  DMA_Init(DMA2_Channel5, &DMA_InitStructure);
}
//----------------------------------------
void DMA2_Chan5_init_set(u32 MemoryBaseAddr, u16 BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;

	  DMA_DeInit(DMA2_Channel5);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC3->DR;
	  DMA_InitStructure.DMA_MemoryBaseAddr = MemoryBaseAddr;//(u32)&Data_ADC_Data_IU_DMA[QuantityReceiveDataUI_SPI];
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //ADC3 -> memory
	  DMA_InitStructure.DMA_BufferSize = BufferSize;//QuantityReceiveDataUI_SPI;//Quantity of data to be transferred
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //Peripheral data size - 16 bits
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //Memory data size - 16 bits
	  //In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //Memory to memory mode disabled
	  DMA_Init(DMA2_Channel5, &DMA_InitStructure);

	  /* Enable DMA2 channel5 */
	  DMA_Cmd(DMA2_Channel5, ENABLE);

	//  ADC_DMACmd(ADC3, ENABLE);// Enable ADC3 DMA
}

//-----------------------------------------------------------------------------------------------------
void DMA1_Chan3_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	  DMA_DeInit(DMA1_Channel3);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI1->DR;
	  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&Data_ADC_Data_IU_DMA[0];  //Set MemoryBaseAddr_DMA?????????????????????
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //memory -> SPI1
//Quantity of data to be transferred from memory to SPI1 (two array of 16 bit, and need transmit of 8 bit, so 4*QuantityReceiveDataUI_SPI)
	  DMA_InitStructure.DMA_BufferSize = 4*QuantityReceiveDataUI_SPI;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //Peripheral data size - 8 bits
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;  //Memory data size - 8 bits
//In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular ;//DMA_Mode_Normal;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //Memory to memory mode disabled
	  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
}
//////--------------------------------------------------------------------------------------
void DMA1_Chan3_init_set(u32 MemoryBaseAddr, u16 BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;

	  DMA_DeInit(DMA1_Channel3);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI1->DR; // &ADC3->DR;
	  DMA_InitStructure.DMA_MemoryBaseAddr = MemoryBaseAddr;//(u32)&Data_ADC_Data_IU_DMA[0];  //Set MemoryBaseAddr_DMA?????????????????????
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //memory -> SPI1
//Quantity of data to be transferred from memory to SPI1 (two array of 16 bit, and need transmit of 8 bit, so 4*QuantityReceiveDataUI_SPI)
	  DMA_InitStructure.DMA_BufferSize = BufferSize;//if U and I then 4*QuantityReceiveDataUI_SPI;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //Peripheral data size - 8 bits
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  //Memory data size - 8 bits
//In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// DMA_Mode_Circular
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //Memory to memory mode disabled
	  DMA_Init(DMA1_Channel3, &DMA_InitStructure);

	  //Enable interrupt DMA1 channel3
      DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);

    //  GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
	  /* Enable DMA1 channel3 */
	  DMA_Cmd(DMA1_Channel3, ENABLE);

}
//-----------------------------------------------------------------------------------------------------
void DMA1_Chan3_init_set8_16(u32 MemoryBaseAddr, u16 BufferSize, u16 DataSizePer, u16 DataSizeMem)
{
	DMA_InitTypeDef DMA_InitStructure;

	  DMA_DeInit(DMA1_Channel3);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI1->DR; // &ADC3->DR;
	  DMA_InitStructure.DMA_MemoryBaseAddr = MemoryBaseAddr;//(u32)&Data_ADC_Data_IU_DMA[0];  //Set MemoryBaseAddr_DMA?????????????????????
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //memory -> SPI1
//Quantity of data to be transferred from memory to SPI1 (two array of 16 bit, and need transmit of 8 bit, so 4*QuantityReceiveDataUI_SPI)
	  DMA_InitStructure.DMA_BufferSize = BufferSize;//if U and I then 4*QuantityReceiveDataUI_SPI;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DataSizePer;//DMA_PeripheralDataSize_Byte; //Peripheral data size - 8 bits
	  DMA_InitStructure.DMA_MemoryDataSize = DataSizeMem;//DMA_MemoryDataSize_Byte;  //Memory data size - 8 bits
//In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// DMA_Mode_Circular
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //Memory to memory mode disabled
	  DMA_Init(DMA1_Channel3, &DMA_InitStructure);

	  //Enable interrupt DMA1 channel3
      DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);

    //  GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
	  /* Enable DMA1 channel3 */
	  DMA_Cmd(DMA1_Channel3, ENABLE);

}

//-------------------------------------------------------------
void DMA1_Channel3_IRQHandler(void)
{
	//GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
	DMA_ClearITPendingBit(DMA1_IT_GL3);
	DMA_ClearITPendingBit(DMA1_IT_TC3);
	  FlagDMA1_Channel3_Trans_Yes =true;
	 // Count_SPI1=0;
	DMA_Cmd(DMA1_Channel3, DISABLE);
}

//////--------------------------------------------------------------------------------------
void DMA1_Chan2_init_set(u32 MemoryBaseAddr, u16 BufferSize, bool MemoryInc)
{
	DMA_InitTypeDef DMA_InitStructure;

	  DMA_DeInit(DMA1_Channel2);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI1->DR; // &ADC3->DR;
	  DMA_InitStructure.DMA_MemoryBaseAddr = MemoryBaseAddr;//(u32)&Data_ADC_Data_IU_DMA[0];  //Set MemoryBaseAddr_DMA?????????????????????
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //SPI1 ->memory
//Quantity of data to be transferred from memory to SPI1 (two array of 16 bit, and need transmit of 8 bit, so 4*QuantityReceiveDataUI_SPI)
	  DMA_InitStructure.DMA_BufferSize = BufferSize;//if U and I then 4*QuantityReceiveDataUI_SPI;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	  if(MemoryInc)DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	     else DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //Peripheral data size - 8 bits
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  //Memory data size - 8 bits
//In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
	  DMA_InitStructure.DMA_Mode =  DMA_Mode_Normal;//DMA_Mode_Circular;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //Memory to memory mode disabled
	  DMA_Init(DMA1_Channel2, &DMA_InitStructure);

	  //Enable interrupt DMA1 channel2
      DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

	  /* Enable DMA1 channel2 */
	  DMA_Cmd(DMA1_Channel2, ENABLE);
}
//-----------------------------------------------------------------------------------------------------
void DMA1_Chan2_init_set8_16(u32 MemoryBaseAddr, u16 BufferSize, bool MemoryInc, u16 DataSizePer, u16 DataSizeMem)
{
	DMA_InitTypeDef DMA_InitStructure;

	  DMA_DeInit(DMA1_Channel2);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI1->DR; // &ADC3->DR;
	  DMA_InitStructure.DMA_MemoryBaseAddr = MemoryBaseAddr;//(u32)&Data_ADC_Data_IU_DMA[0];  //Set MemoryBaseAddr_DMA?????????????????????
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //SPI1 ->memory
//Quantity of data to be transferred from memory to SPI1 (two array of 16 bit, and need transmit of 8 bit, so 4*QuantityReceiveDataUI_SPI)
	  DMA_InitStructure.DMA_BufferSize = BufferSize;//if U and I then 4*QuantityReceiveDataUI_SPI;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	  if(MemoryInc)DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	     else DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //Peripheral data size - 8 bits
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  //Memory data size - 8 bits
//In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
	  DMA_InitStructure.DMA_Mode =  DMA_Mode_Normal;//DMA_Mode_Circular;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //Memory to memory mode disabled
	  DMA_Init(DMA1_Channel2, &DMA_InitStructure);

	  //Enable interrupt DMA1 channel2
      DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

	  /* Enable DMA1 channel2 */
	  DMA_Cmd(DMA1_Channel2, ENABLE);
}

//-------------------------------------------------------------
void DMA1_Channel2_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_GL2);
	DMA_ClearITPendingBit(DMA1_IT_TC2);
	  FlagDMA1_Channel2_Trans_Yes =true;
	 DMA_Cmd(DMA1_Channel2, DISABLE);
}
//-----------------------------------------

void DMA1_Chan6_init(void)
{
	  DMA_InitTypeDef DMA_InitStructure;

	  DMA_DeInit(DMA1_Channel6);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&TIM8->CNT;//DMA1_Channel5->CCR;       //SPI2->CR2;//Set MemoryBaseAddr_DMA bit SPI_CR2_TXDMAEN reg. SPI_CR2 for SPI2 Tx buffer DMA enabled
	  DMA_InitStructure.DMA_MemoryBaseAddr =  (u32)&DataCNT_TIM8;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //TIM8(CNT) -> DataCNT_TIM8
	  DMA_InitStructure.DMA_BufferSize = 1;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//Peripheral data size - 16 bits
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;//Memory data size - 16 bits
//In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular ;//DMA_Mode_Normal;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMA_M2M_Enable; //Memory to memory mode Enable
	  DMA_Init(DMA1_Channel6, &DMA_InitStructure);
  //Enable interrupt DMA1 channel2
      DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
	  //Enable DMA1 channel2
	  DMA_Cmd(DMA1_Channel6, ENABLE); //
}
void DMA1_Channel6_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_GL6);
	FlagDMA1_Channel6_Trans_Yes=true;
	Count_DMA_TIM3++;


}


void DMA1_Chan5_init(void)
{
	  DMA_InitTypeDef DMA_InitStructure1;

	  DMA_DeInit(DMA1_Channel5);
	  DMA_InitStructure1.DMA_PeripheralBaseAddr = (u32)&SPI2->DR;//
	  DMA_InitStructure1.DMA_MemoryBaseAddr =  (u32)&Command_to_Acceler_forRead_Byte[0];
	  DMA_InitStructure1.DMA_DIR = DMA_DIR_PeripheralDST; //data from DataOut_Accelerometer -> to SPI2->DR
	  DMA_InitStructure1.DMA_BufferSize = Quantity_DataAccelerometer;
	  DMA_InitStructure1.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure1.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure1.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// //Peripheral data size - 16 bits
	  DMA_InitStructure1.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte ;////Memory data size - 16 bits
//In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
	  //But better use DMA_Mode_Normal (in high speed - 8MHz and more ERRORs by SPI)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	  DMA_InitStructure1.DMA_Mode =DMA_Mode_Normal;// DMA_Mode_Circular;//
	  DMA_InitStructure1.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure1.DMA_M2M = DMA_M2M_Disable;//DMA_M2M_Enable; //Memory to memory mode Enable
	  DMA_Init(DMA1_Channel5, &DMA_InitStructure1);
	  //Enable interrupt DMA1 channel5
      DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
}
//-------------------------------------------------------------
void DMA1_Channel5_IRQHandler(void)
{

	DMA_ClearITPendingBit(DMA1_IT_GL5);
	FlagDMA1_Channel5_Trans_Yes=true;
	DMA_Cmd(DMA1_Channel5, DISABLE);
}
//-----------------------------------------------------------------------------------------
/* Accelerometer Rx
 
 */

void DMA1_Chan4_init(void)
{
	  DMA_InitTypeDef DMA_InitStructure1;

	  DMA_DeInit(DMA1_Channel4);
	  DMA_InitStructure1.DMA_PeripheralBaseAddr = (u32)&SPI2->DR;
	  DMA_InitStructure1.DMA_MemoryBaseAddr =  (u32)&Data_from_Accelerometer_Byte[0];
	  DMA_InitStructure1.DMA_DIR = DMA_DIR_PeripheralSRC; //data from SPI2->DR to DataIn_Accelerometer
	  DMA_InitStructure1.DMA_BufferSize = Quantity_DataAccelerometer;
	  DMA_InitStructure1.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure1.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure1.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// //Peripheral data size - 16 bits
	  DMA_InitStructure1.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte ;////Memory data size - 16 bits
//In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
//But better use DMA_Mode_Normal (in high speed - 8MHz and more ERRORs by SPI)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	  DMA_InitStructure1.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;//
	  DMA_InitStructure1.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure1.DMA_M2M = DMA_M2M_Disable;//DMA_M2M_Enable; //Memory to memory mode Enable
	  DMA_Init(DMA1_Channel4, &DMA_InitStructure1);
	  //Enable interrupt DMA1 channel4
      DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
}
//-------------------------------------------------------------
void DMA1_Channel4_IRQHandler(void)
{
	//FlagDMA1_Channel4_Trans_Yes=false;
	//GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
	DMA_ClearITPendingBit(DMA1_IT_GL4);
	DMA_Cmd(DMA1_Channel4, DISABLE);

	if(!Flag_TIM2_IRQHandler)
	{
	temp_DataAcceler_X = Le16_ShR_2(*DataAcceler_X);
	temp_DataAcceler_Y = Le16_ShR_2(*DataAcceler_Y);
	temp_DataAcceler_Z = Le16_ShR_2(*DataAcceler_Z);
	Sum_forSpeed_X += temp_DataAcceler_X;
	Sum_forSpeed_Y += temp_DataAcceler_Y;
	Sum_forSpeed_Z += temp_DataAcceler_Z;

	Sum_forMove_X += Sum_forSpeed_X;
	Sum_forMove_Y += Sum_forSpeed_Y;
	Sum_forMove_Z += Sum_forSpeed_Z;

	TIM_Cmd(TIM2,ENABLE);
	Count_TIM2_IRQHandler=6;//for read temperature and Read INT_STATUS register (for reset SAT-bit - overflow) from accelerometer
	}
	 else
	    {
		  Flag_TIM2_IRQHandler = false;
		  if(Count_TIM2_IRQHandler>10)
		    {
			  LSB(Temperature_Acceler) = *Temperature_LSB;
			  MSB(Temperature_Acceler) = *Temperature_MSB;
			  Temperature_Acceler >>=1; //Temperature data is in unsigned format and 13 bits - only [13:1] bits

			    Flag_OverflowAcceler = (*INT_STATUS_Acceler&SAT_bit);

			    if(Flag_OverflowAcceler&&(temp_DataAcceler_X==MAX_Value_Acceler_PLUS))Count_Overflow_X_PLUS++;
			    if(Flag_OverflowAcceler&&(temp_DataAcceler_X==MAX_Value_Acceler_MINUS))Count_Overflow_X_MINUS++;
			    if(Flag_OverflowAcceler&&(temp_DataAcceler_Y==MAX_Value_Acceler_PLUS))Count_Overflow_Y_PLUS++;
			    if(Flag_OverflowAcceler&&(temp_DataAcceler_Y==MAX_Value_Acceler_MINUS))Count_Overflow_Y_MINUS++;
			    if(Flag_OverflowAcceler&&(temp_DataAcceler_Z==MAX_Value_Acceler_PLUS))Count_Overflow_Z_PLUS++;
			    if(Flag_OverflowAcceler&&(temp_DataAcceler_Z==MAX_Value_Acceler_MINUS))Count_Overflow_Z_MINUS++;
//-------------------------------------------------------------------------------------------------------------
					if(FlagForDataPause&&CountInterruptCapture_Timer4>=QuantityReceiveDataAcceler_SPI)//îãðàíè÷èì ê-âî			
							{

						if(CountBuffForDataPause==1&&indexForDataPause<SizeDataPauseMax)
						                {
							             ArrayForOutDataPuse[indexForDataPause]=Data_Giro_Gyroscope;
							             indexForDataPause++;
							             ArrayForOutDataPuse[indexForDataPause]=temp_DataAcceler_X;
							             indexForDataPause++;
							             ArrayForOutDataPuse[indexForDataPause]=temp_DataAcceler_Y;
							             indexForDataPause++;
							             ArrayForOutDataPuse[indexForDataPause]=temp_DataAcceler_Z;
							             indexForDataPause++;
						                }
						if(CountBuffForDataPause==0&&indexForDataPause1<SizeDataPauseMax)
						                {
							             ArrayForOutDataPuse1[indexForDataPause1]=Data_Giro_Gyroscope;
							             indexForDataPause1++;
							             ArrayForOutDataPuse1[indexForDataPause1]=temp_DataAcceler_X;
							             indexForDataPause1++;
							             ArrayForOutDataPuse1[indexForDataPause1]=temp_DataAcceler_Y;
							             indexForDataPause1++;
							             ArrayForOutDataPuse1[indexForDataPause1]=temp_DataAcceler_Z;
							             indexForDataPause1++;
						                }
							}
//-------------------------------------------------------------------------------------------------------------
		    }
		     else TIM_Cmd(TIM2,ENABLE);
	   }

}

//-----------------------------------------------------------------------------------------


void DMA2_Chan2_init(void)
{
	  DMA_InitTypeDef DMA_InitStructure1;

	  DMA_DeInit(DMA2_Channel2);
	  DMA_InitStructure1.DMA_PeripheralBaseAddr = (u32)&SPI3->DR;
	  DMA_InitStructure1.DMA_MemoryBaseAddr =  (u32)&Command_to_Gyroscope_Choice_Channel_Byte[0];
	  DMA_InitStructure1.DMA_DIR = DMA_DIR_PeripheralDST; ////data from  -> to SPI3->DR
	  DMA_InitStructure1.DMA_BufferSize = Quantity_DataGyroscope;
	  DMA_InitStructure1.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure1.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure1.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// //Peripheral data size - 16 bits
	  DMA_InitStructure1.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte ;////Memory data size - 16 bits
//In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
//But better use DMA_Mode_Normal (in high speed - 8MHz and more ERRORs by SPI)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	  DMA_InitStructure1.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;//
	  DMA_InitStructure1.DMA_Priority = DMA_Priority_Medium;
	  DMA_InitStructure1.DMA_M2M = DMA_M2M_Disable;//DMA_M2M_Enable; //Memory to memory mode Enable
	  DMA_Init(DMA2_Channel2, &DMA_InitStructure1);

	  //Enable interrupt DMA2 channel1
      DMA_ITConfig(DMA2_Channel2, DMA_IT_TC, ENABLE);

	  //Enable DMA2 channel1
	 // DMA_Cmd(DMA2_Channel2, ENABLE); //
}
//-------------------------------------------------------------
void DMA2_Channel2_IRQHandler(void)
{
	FlagDMA2_Channel2_Trans_Yes=true;
	DMA_ClearITPendingBit(DMA2_IT_GL2);
	DMA_Cmd(DMA2_Channel2, DISABLE);

}
//-----------------------------------------------------------------------------------------


void DMA2_Chan1_init(void)
{
	  DMA_InitTypeDef DMA_InitStructure1;

	  DMA_DeInit(DMA2_Channel1);
	  DMA_InitStructure1.DMA_PeripheralBaseAddr = (u32)&SPI3->DR;
	  DMA_InitStructure1.DMA_MemoryBaseAddr =  (u32)&Data_from_Gyroscope_Byte[0];
	  DMA_InitStructure1.DMA_DIR = DMA_DIR_PeripheralSRC; //data from SPI3->DR
	  DMA_InitStructure1.DMA_BufferSize = Quantity_DataGyroscope;
	  DMA_InitStructure1.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure1.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure1.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// //Peripheral data size - 16 bits
	  DMA_InitStructure1.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte ;////Memory data size - 16 bits
//In circular mode, after the last transfer, the DMA_CNDTRx register is automatically reloaded with the initially programmed value (BufferSize).
//But better use DMA_Mode_Normal (in high speed - 8MHz and more ERRORs by SPI)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	  DMA_InitStructure1.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;//
	  DMA_InitStructure1.DMA_Priority = DMA_Priority_Medium;
	  DMA_InitStructure1.DMA_M2M = DMA_M2M_Disable;//DMA_M2M_Enable; //Memory to memory mode Enable
	  DMA_Init(DMA2_Channel1, &DMA_InitStructure1);

	  //Enable interrupt DMA2 channel1
      DMA_ITConfig(DMA2_Channel1, DMA_IT_TC, ENABLE);

	  //Enable DMA2 channel1
	  //DMA_Cmd(DMA2_Channel1, ENABLE); //
}
//-------------------------------------------------------------
void DMA2_Channel1_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_IT_GL1);
//FlagDMA2_Channel1_Trans_Yes=false;

	DMA_Cmd(DMA2_Channel1, DISABLE);

	if(Count_Gyroscope_interrupt==3)
		{
		GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_SET);    //'MSEL2' -> '1' by SPI3
	//	DMA2_Channel2->CMAR=(u32)&Command_to_Gyroscope_Choice_Channel_Byte[Count_Gyroscope_interrupt];
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET); //'MSEL1' -> '0' - read data from gyroscope by SPI3
		}

  if(Count_Gyroscope_interrupt==6)
  {
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET); //'MSEL1' -> '1' - end read data from gyroscope by SPI3
	TempForDataGyr=Data_from_Gyroscope_Byte[0];Data_from_Gyroscope_Byte[0]= Data_from_Gyroscope_Byte[2]; Data_from_Gyroscope_Byte[2]=TempForDataGyr;
	Data_fromGyroscope = (u32*)&Data_from_Gyroscope_Byte[0];
	Data_Giro_Gyroscope_temp = *Data_fromGyroscope;
	Data_Giro_Gyroscope_temp >>= 4;
		Data_Giro_Gyroscope = (u32)Data_Giro_Gyroscope_temp - Zero_rate_condition;//+973;
		Sum_Giro_Gyroscope += Data_Giro_Gyroscope;
	DMA2_Channel2->CMAR=(u32)&Command_to_Gyroscope_Choice_Channel_Byte[3]; //[3,4,5] 2-byte - Measure Temperature
	GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET);//'MSEL2' -> '0' - choice channel in gyroscope by SPI3
  }

	if(Count_Gyroscope_interrupt==9)
		{
		GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_SET);    //'MSEL2' -> '1' by SPI3
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET); //'MSEL1' -> '0' - read data from gyroscope by SPI3
		}

  if(Count_Gyroscope_interrupt==12)
   {
 	  GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET); //'MSEL1' -> '1' - end read data from gyroscope by SPI3
 		TempForDataGyr=Data_from_Gyroscope_Byte[0];Data_from_Gyroscope_Byte[0]= Data_from_Gyroscope_Byte[2]; Data_from_Gyroscope_Byte[2]=TempForDataGyr;
 		Data_fromGyroscope = (u32*)&Data_from_Gyroscope_Byte[0];
 		Data_Giro_Gyroscope_temp = *Data_fromGyroscope;
 		Data_Giro_Gyroscope_temp >>= 4;
 		Data_Temperature_Gyroscope = Data_Giro_Gyroscope_temp;
 	   }
	DMA2_Channel2->CNDTR=Quantity_DataGyroscope;

	DMA2_Channel1->CMAR=(u32)&Data_from_Gyroscope_Byte[0];
	DMA2_Channel1->CNDTR=Quantity_DataGyroscope;

	 if(Count_Gyroscope_interrupt<12)//<12
	 {
	  DMA_Cmd(DMA2_Channel2, ENABLE);//Tx
      DMA_Cmd(DMA2_Channel1, ENABLE);//Rx
     }
	 Count_Gyroscope_interrupt += 3;

}
