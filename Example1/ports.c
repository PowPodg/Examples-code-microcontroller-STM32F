#include "ports.h"

U32 i,j=0;
volatile u32 temp_start_addr,  temp_start_addr1;

 volatile bool Flag_Start_Cycle_DataReceive_SPI1;
 volatile bool Flag_End_Cycle_DataReceive_SPI1;
//u16 *temp_Value_for_Timer4_pointer;
GPIO_InitTypeDef GPIO_InitStructure;

/*******************************************************************************
* Function Name  : Ports_Config
* Description    : Configures the different GPIO ports pins.
*******************************************************************************/
void Ports_Config(void)
{
	//RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //Without this does not work SPI3
	//AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;// | AFIO_MAPR_TIM3_REMAP_FULLREMAP;//If you do not disable JTAG, it does not work SPI3
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // Disable JTAG/SWD so pins are availble

   // GPIO_InitTypeDef GPIO_InitStructure;
    //-=--------------------PORT PA------------------------------------------------
    /* Configure PA.11, as with Mode_Out output Push-pull, because use Answer for Command throw SPI1*/
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
       GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP; //output Push-pull for
       GPIO_Init(GPIOA , &GPIO_InitStructure);
       GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);//Bit_SET);//default

  /* Configure PA.01, PA.02, as input Analog mode */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //input Analog mode for PA.01 - ADC1, PA.02 - ADC2
   GPIO_Init(GPIOA , &GPIO_InitStructure);

  /* Configure PA.00, as input Input with pull-up  */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //input pull-up for PA0 - GK;
   GPIO_Init(GPIOA , &GPIO_InitStructure);

   /* Configure  PA.05, PA.07, as input Input with GPIO_Mode_IN_FLOATING  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //input GPIO_Mode_IN_FLOATING   PA5, PA7 - SPI1 SCK, MOSI
    GPIO_Init(GPIOA , &GPIO_InitStructure);

    /* Configure PA.04,  as input Input with GPIO_Mode_IPU   */

     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; //input GPIO_Mode_IPU   PA4,
     GPIO_Init(GPIOA , &GPIO_InitStructure);


  /* Configure PA.06, as with Alternate function output Push-pull, because use SPI, and not GPIO */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //output Push-pull for PA06 - MISO;
   GPIO_Init(GPIOA , &GPIO_InitStructure);

   /* Configure PA.15, as with General purpose output Push-pull, because use GPIO */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP; //output Push-pull for PA15 - MSELK1 -for gyroscope
   GPIO_Init(GPIOA , &GPIO_InitStructure);
   GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);// MSEL1 -> '0'

   //-=--------------------PORT PB------------------------------------------------
   /* Configure PB.04, PB.14, as input Input with pull-up  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;// GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING; //input Mode_IN_FLOATING for PB04 - MISO SPI3; PB14 - MISO SPI2
    GPIO_Init(GPIOB , &GPIO_InitStructure);

    /* Configure PB.14, as input Input with pull-up  */
     GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //input Mode_IN_FLOATING for  PB14 - MISO SPI2
     GPIO_Init(GPIOB , &GPIO_InitStructure);

    /* Configure PB.06,  as with General purpose output push-pull, because use GPIO  */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //output Push-pull for PB6 - MSEL2 -for gyroscope,
    GPIO_Init(GPIOB , &GPIO_InitStructure);

    GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_SET);

    /* Configure  PB.12 as with General purpose output push-pull, because use GPIO */
     GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //output Push-pull PB12 - SS SPI2 (software mode)
     GPIO_Init(GPIOB , &GPIO_InitStructure);

    GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);

    /* Configure PB.00,as with Alternate function output Push-pull, because use out TIM3 PWM, and not GPIO*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;//GPIO_Mode_AF_PP; //output Push-pull for PB0 - TIM3 CH3 PWM
    GPIO_Init(GPIOB , &GPIO_InitStructure);
  //  GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);

    /* Configure PB.13, PB.15,as with Alternate function output Push-pull, because use SPI, and not GPIO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //output Push-pull for  PB13 - SCK SPI2, PB15 - MOSI SPI2
    GPIO_Init(GPIOB , &GPIO_InitStructure);

    /* Configure PB.03, PB.05,,as with Alternate function Push-pull, because use SPI, and not GPIO */
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //output Push-pull for PB03 - SCK SPI3, PB05 - MOSI SPI3
     GPIO_Init(GPIOB , &GPIO_InitStructure);

//-=--------------------PORT PC------------------------------------------------
    /* Configure PC.10, PC.12,as with Alternate function output Push-pull, because use USART, and not GPIO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //output Push-pull for PC10 - TX USART3, PC15 - CK USART3
    GPIO_Init(GPIOC , &GPIO_InitStructure);
    /* Configure PC.04, PC.05  as input Input with IN_FLOATING  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;// | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // GPIO_Mode_IN_FLOATING; //input FLOATING for PA5 - 'Res1' (see electrical schematic) for synchronization receive data by SPI3
    GPIO_Init(GPIOC , &GPIO_InitStructure);
    //Configure EXTI Line5 PC.04, PC.05 to generate an interrupt on rising or falling edge
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4); //Res
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5); //Res1

    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line4|EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

//-=--------------------PORT PD------------------------------------------------
    /* Configure PD.02, as with General purpose output push-pull, because use GPIO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //output Push-pull for PD2 - Gain_Cur_CS -for Gain operational amplifier U6A current values generator coil
    GPIO_Init(GPIOD , &GPIO_InitStructure);

    GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_SET); //SC to "1" for U1 Digital Potentiometer AD5160

}
//-------------------------------------------------------------------
/* Configure PA.04,  as input Input with GPIO_Mode_IPU   */

void GPIO_Pin_4_Mode_IPUp(void)
{
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; //input GPIO_Mode_IPU   PA4,
GPIO_Init(GPIOA , &GPIO_InitStructure);
}
/* Configure PA.04,  as input Input with GPIO_Mode_IPD   */

void GPIO_Pin_4_Mode_IPDown(void)
{
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ; //input GPIO_Mode_IPU   PA4,
GPIO_Init(GPIOA , &GPIO_InitStructure);
}
//--------------------------------------------------------------------

void EXTI4_IRQHandler(void)
{
	EXTI_ClearFlag(EXTI_Line4);

	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)==Bit_SET)
	{

	    Count_Overflow_X_PLUS=0; Count_Overflow_X_MINUS=0;
	    Count_Overflow_Y_PLUS=0; Count_Overflow_Y_MINUS=0;
	    Count_Overflow_Z_PLUS=0; Count_Overflow_Z_MINUS=0;

	    CountTotal_DataOut=2; 
	    for(j=2;j<1+(QuantityReceiveByteUI_SPI*2);j++)Array_Total_DataOut[j]=0;

		temp_start_addr = (u32)&Array_Total_DataOut[CountTotal_DataOut];
		DMA1_Chan1_init_set(temp_start_addr, QuantityReceiveDataUI_SPI); //initial DMA1 for ADC1 for I
	    ADC_SoftwareStartConvCmd(ADC3, ENABLE); //Start ADC3 Software Conversion
	    CountTotal_DataOut +=  QuantityReceiveByteUI_SPI;

		temp_start_addr = (u32)&Array_Total_DataOut[CountTotal_DataOut];
	    DMA2_Chan5_init_set(temp_start_addr, QuantityReceiveDataUI_SPI); //initial DMA2 for ADC3 for U
	    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //Start ADC1 Software Conversion
	    CountTotal_DataOut +=  QuantityReceiveByteUI_SPI;

	    TIM_SetCounter(TIM1, 0); //reset CNT
	    TIM_Cmd(TIM1,ENABLE);
	    FlagForDataPause =  false;
	    CountInterruptCapture_Timer4=0;

        TIM_Cmd(TIM4,ENABLE);//Note that the counter starts counting 1 clock cycle after setting the CEN bit
	}
}

void EXTI9_5_IRQHandler(void)
{
	EXTI_ClearFlag(EXTI_Line5);
	if(!(GPIOC->IDR & GPIO_Pin_5))
	{
   		FlagDMA1_Channel3_Trans_Yes=false;
     	FlagDMA1_Channel2_Trans_Yes=false;

	}
	else
	    {
  		FlagDMA1_Channel3_Trans_Yes=false;
     	FlagDMA1_Channel2_Trans_Yes=false;
     	Array_CommandIn[0]=0;

         } 
}


