#include "Timer.h"
u8 temp=0;
volatile u8 CountBuffForDataPause=0;

volatile u16 DataCNT_TIM8;
volatile u8 Arr_SetTimer_for_Sampling_U_I[10000];
 volatile u16 CountInterruptCapture_Timer1;

  volatile u8  Arr_SetTimer_for_Sampling_Acceler[2000];
  volatile u16 CountInterruptCapture_Timer4;

   volatile bool FlagInterruptTimer3;
   volatile bool FlagInterruptTimer3_OC3_OC4;

    volatile u16 CountTotal_DataOut;

    volatile u16 *temp_Value_for_Timer1_pointer, *temp_Value_for_Timer4_pointer;
    volatile s16 *temp_Value_for_DataOut_pointer;
    volatile s32 tempSum_forSpeed_X, tempSum_forSpeed_Y, tempSum_forSpeed_Z;
    volatile s32 *temp_SumSpeed_pointer;

    volatile int64_t tempSum_forMove_X, tempSum_forMove_Y, tempSum_forMove_Z;
    volatile int_fast64_t *temp_SumMove_pointer;
    volatile s32 *temp_SumMove_pointer32bit_H, *temp_SumMove_pointer32bit_L;

    volatile u16 tempTemperature_Acceler;
    volatile u16 *tempTemperature_pointer;

    volatile s16 tempData_Giro_Gyroscope;
    volatile s32 tempSum_Giro_Gyroscope;
    volatile s32 *tempGiro_Gyroscope_pointer;
    volatile u16 temp_Temperature_Gyroscope;
    volatile u16 *temp_Temper_Gyroscope_pointer;

     volatile u16 *temp_GK_Data_pointer;
    volatile u16 *temp_GK_Time_count;
    volatile u16 temp_GK_Data;
    volatile u16 temp_GK_Time;

     volatile bool Flag_TIM2_IRQHandler;
     volatile u8 Count_TIM2_IRQHandler;




void Set_Data_for_Timer(void)
{
	//Array_Total_DataOut=NULL;
//	Arr_SetTimer_for_Sampling_U_I = NULL;
	//Arr_SetTimer_for_Sampling_Acceler = NULL;
	temp_Value_for_Timer1_pointer = NULL;
	temp_Value_for_Timer4_pointer = NULL;
	temp_Value_for_DataOut_pointer = NULL;
	temp_SumSpeed_pointer = NULL;
	temp_SumMove_pointer = NULL;
}

void Timer1_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;


	TIM_TimeBaseStructure.TIM_Prescaler = 36;//720;//AHB_CLK =72Mhz, for time cycle input frequency Timer1 = 10us
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//division ratio between the timer clock (CK_INT) frequency and the dead-time and sampling clock
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Period = 65535;  //All time period = 655,35 ms

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	//Enable the Capture Compare 1 Interrupt source
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);

        TIM_OCStructInit(&TIM_OCInitStructure);//initial default
    /* Set the Capture Compare Register value */
    TIM_OCInitStructure.TIM_Pulse = WIDTH_PULSE_TIM1_TIM3;//max pulse value - a temporary value 500 us
    //TIM_SetCompare1(TIM1,uint16_t value)-then here you need to specify a value with an array of timer!!!!!!

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

}
//------------------------------------------------------------------------------------------

void TIM1_CC_IRQHandler(void)
{

	//if interrupt happens the do this
	if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{
	//clear interrupt and start counting again to get precise freq
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
	//TIM_ClearFlag(TIM1, TIM_FLAG_CC1);

    if(CountInterruptCapture_Timer1<QuantityReceiveDataUI_SPI)
  //set compare register for Timer1	from array for the SECOND and subsequent points voltage and current
     {
	  //For the second and subsequent points voltage and current start ADC1, ADC3!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //Start ADC1 Software Conversion
	ADC_SoftwareStartConvCmd(ADC3, ENABLE); //Start ADC3 Software Conversion
	// temp_Value_for_Timer1_pointer = (u16*)&Arr_SetTimer_for_Sampling_U_I[CountInterruptCapture_Timer1*2];
    TIM1->CNT=0;
     CountInterruptCapture_Timer1 ++;
     }
//----------------------
    if(CountInterruptCapture_Timer1>=QuantityReceiveDataUI_SPI)
    {
	//disabled timer TIM1,  if value 'CountInterruptCapture_Timer1' is more the last element of the array
	TIM_Cmd(TIM1,DISABLE); CountInterruptCapture_Timer1=0;
	TIM_SetCounter(TIM1, 0); //reset CNT
    }
  }
}
//Timer3 - for sampling data from accelerometer, gyroscope and Sync (use PWM) for module 'Data acquisition board' (communication through SPI1_Main)
//-------------------------------------------------------------------------------------------------
void Timer3_Init(void)
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Prescaler = 36;//APB1 AHB_CLK = 36MHz, for time cycle input frequency Timer3 = 1us
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//division ratio between the timer clock (CK_INT) frequency and the dead-time and sampling clock
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Period = 976;  //All time period = 500us or 0.5ms (2kHz)

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_SetCounter(TIM3, 0); //reset CNT

	//Selects the TIM3 Trigger Output Mode OC3REF (OC1REF) signal is used as the trigger output
	//TIM3 OC1 Master source -  for TIM4
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_OC2Ref); //TIM_TRGOSource_OC1Ref);!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    /* Timing Mode configuration: Channel3 */
    TIM_OCStructInit(&TIM_OCInitStructure);//initial defaul
    /* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = WIDTH_PULSE_TIM1_TIM3; //period impulses OC3
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

    //other Channel 2  for change polarity count for TIM4 - Almost change polarity (the difference is 20 units)
    //Timers do not have internal falling edge (only rising edge) - for external input, this option (falling edge) exists
     TIM_OCInitStructure.TIM_Pulse = 956; //period impulses OC1
     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    // TIM_OCInitStructure.TIM_OutputState = TIM._OutputState_Enable;
     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

     TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
     TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

     TIM_ARRPreloadConfig(TIM3, ENABLE);
     //Enables the TIM3 DMA Requests for GK. TIM3 DMA_sources is 'TIM_DMA_CC1'
    TIM_DMACmd(TIM3, TIM_DMA_CC1, ENABLE);

	//Enable the Capture Compare 3 Interrupt source

	TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);

	TIM_Cmd(TIM3,ENABLE);
}

//-----------------------------------------------------------------------
//Timer3 interrupt - for starts request 'sampling accelerometer' through
//DMA Channel5 - SPI2 Tx and DMA Channel4 - SPI2 Rx
void TIM3_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{

		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		FlagInterruptTimer3=true;
	}

	if(TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
	{

		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
		FlagInterruptTimer3=false;
		FlagInterruptTimer3_OC3_OC4=false;

		FlagDMA1_Channel5_Trans_Yes = false;//tx SPI2

    //-----accelerometer-----------------
    DMA1_Channel5->CMAR=(u32)&Command_to_Acceler_forRead_Byte[0];
    DMA1_Channel4->CMAR=(u32)&Data_from_Accelerometer_Byte[0];
	DMA1_Channel5->CNDTR=Quantity_DataAccelerometer;
	DMA1_Channel4->CNDTR=Quantity_DataAccelerometer;
	//not work PORTB.12 on OLIMEX - then to correct !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_RESET); //'SS' SPI2 -> '0' - begin request data from accelerometer by SPI
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);//'SS' SPI2 -> '0' - begin request data from accelerometer by SPI

	//SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);//Tx
	DMA_Cmd(DMA1_Channel4, ENABLE);//Rx

	//-----gyroscope-----------------

	FlagDMA2_Channel2_Trans_Yes=false;//tx SPI3

	Count_Gyroscope_interrupt=0;
	DMA2_Channel2->CMAR=(u32)&Command_to_Gyroscope_Choice_Channel_Byte[Count_Gyroscope_interrupt]; //[0,1,2] 2-byte - Measure Angular Rate (Gyro)

//	DMA2_Channel2->CMAR=(u32)&Command_to_Gyroscope_Choice_Channel_Byte[12]; //[12,13,14] 12-byte - Self positive test

	DMA2_Channel1->CMAR=(u32)&Data_from_Gyroscope_Byte[0];
	DMA2_Channel2->CNDTR=Quantity_DataGyroscope;
	DMA2_Channel1->CNDTR=Quantity_DataGyroscope;
	GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET);//'MSEL2' -> '0' - choice channel in gyroscope by SPI3

	DMA_Cmd(DMA2_Channel2, ENABLE);//Tx
	DMA_Cmd(DMA2_Channel1, ENABLE);//Rx
	Count_Gyroscope_interrupt += 3;
	}

}
//-------------------------------------------------------
void Timer4_Init(void)
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	//Rising edges of the selected trigger (TRGI) clock the counter
	TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_External1);

	//Configures the TIM4 Internal Trigger as External Clock
	// Internal Trigger 2 for TIM4 this is TRG TIM3 (out TIM3 (2kHz) -> input TIM4)
	TIM_ITRxExternalClockConfig(TIM4, TIM_TS_ITR2);
	//TIM4->SMCR |= TIM_ExtTRGPolarity_Inverted;


	TIM_TimeBaseStructure.TIM_Prescaler = 0;//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//division ratio between the timer clock (CK_INT) frequency and the dead-time and sampling clock
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Period = 65535;  //All time period = 655,35 ms

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//Enable the Capture Compare 1 Interrupt source
	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
      TIM_OCStructInit(&TIM_OCInitStructure);//initial default
    /* Set the Capture Compare Register value */
    TIM_OCInitStructure.TIM_Pulse = 1;//max pulse value - a temporary value !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     TIM_OC1Init(TIM4, &TIM_OCInitStructure);
}


void TIM4_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{
	    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);

	//    GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
   //set compare register for Timer4 from array for the SECOND and subsequent points of values accelerometer
   //(X,Y,Z, +X_Count_Error, -X_Count_Error, +Y_Count_Error, -Y_Count_Error, +Z_Count_Error, -Z_Count_Error, speed V, move S, Temperature)
	    if(CountInterruptCapture_Timer4==sizeof(*temp_Value_for_Timer4_pointer))
	    {
	    	temp_GK_Data = DataCNT_TIM8;
	    	temp_GK_Time = Count_DMA_TIM3;
	    	TIM8->CNT=0;
	    	Count_DMA_TIM3=0;

	    	 tempSum_forSpeed_X = Sum_forSpeed_X;
	    	 tempSum_forSpeed_Y = Sum_forSpeed_Y;
	    	 tempSum_forSpeed_Z = Sum_forSpeed_Z;

	    	 tempSum_forMove_X = Sum_forMove_X;
	    	 tempSum_forMove_Y = Sum_forMove_Y;
	    	 tempSum_forMove_Z = Sum_forMove_Z;

	    	 tempTemperature_Acceler = Temperature_Acceler;

	       	 tempSum_Giro_Gyroscope = Sum_Giro_Gyroscope;
	    	 temp_Temperature_Gyroscope = Data_Temperature_Gyroscope;

	    }
     //----------------------
     //-------------------------------< QuantityReceiveDataAcceler_SPI---------------------------------------------------------------------------------------
 if(CountInterruptCapture_Timer4<QuantityReceiveDataAcceler_SPI)
    {
	   //----------------Array Accelerometer--Flag_Array_Acceler-----------

	      temp_Value_for_DataOut_pointer = (s16*)&Array_Total_DataOut[CountTotal_DataOut];
	      *temp_Value_for_DataOut_pointer = temp_DataAcceler_X;  //data X
		   //first count //because format Array - 1 byte, and format Timer Compare - 2 byte (temp_Value_for_Timer4_pointer)
	       // so you want to make equal increment 2
	      CountTotal_DataOut +=sizeof(*temp_Value_for_DataOut_pointer);

	      temp_Value_for_DataOut_pointer = (s16*)&Array_Total_DataOut[CountTotal_DataOut];
	      *temp_Value_for_DataOut_pointer = temp_DataAcceler_Y;  //data Y
	      CountTotal_DataOut +=sizeof(*temp_Value_for_DataOut_pointer);


	      //((s16*) Array_Total_DataOut)[n]=temp_DataAcceler_Y;

	      temp_Value_for_DataOut_pointer = (s16*)&Array_Total_DataOut[CountTotal_DataOut];
	      *temp_Value_for_DataOut_pointer = temp_DataAcceler_Z;  //data Z
	      CountTotal_DataOut +=sizeof(*temp_Value_for_DataOut_pointer);

	      temp_Value_for_DataOut_pointer = (s16*)&Array_Total_DataOut[CountTotal_DataOut];
	      *temp_Value_for_DataOut_pointer = Data_Giro_Gyroscope;;  //data Gyroscope
	      CountTotal_DataOut +=sizeof(*temp_Value_for_DataOut_pointer);
	   TIM4->CNT=0;
	      CountInterruptCapture_Timer4++;

     }
	//----------------------


	if(CountInterruptCapture_Timer4>=(QuantityReceiveDataAcceler_SPI))
	  {
//----------------Count overflow X,Y,Z always must be with data X, Y, Z!!!!!!!!!!-Flag_Array_Acceler------------

	    	Array_Total_DataOut[CountTotal_DataOut] = Count_Overflow_X_PLUS;      //Count overflow X PLUS
	    	CountTotal_DataOut +=sizeof(u8);
	    	Array_Total_DataOut[CountTotal_DataOut] = Count_Overflow_X_MINUS;      //Count overflow X MINUS
	    	CountTotal_DataOut +=sizeof(u8);

	    	Array_Total_DataOut[CountTotal_DataOut] = Count_Overflow_Y_PLUS;      //Count overflow Y PLUS
	    	CountTotal_DataOut +=sizeof(u8);
	    	Array_Total_DataOut[CountTotal_DataOut] = Count_Overflow_Y_MINUS;      //Count overflow Y MINUS
	    	CountTotal_DataOut +=sizeof(u8);

	    	Array_Total_DataOut[CountTotal_DataOut] = Count_Overflow_Z_PLUS;      //Count overflow Z PLUS
	    	CountTotal_DataOut +=sizeof(u8);
	    	Array_Total_DataOut[CountTotal_DataOut] = Count_Overflow_Z_MINUS;      //Count overflow Z MINUS
	    	CountTotal_DataOut +=sizeof(u8);

//----------------Sum for Speed and Move-------------

	    	temp_SumSpeed_pointer = (s32*)&Array_Total_DataOut[CountTotal_DataOut];
	    	*temp_SumSpeed_pointer = tempSum_forSpeed_X;      //Sum for Speed X
	    	CountTotal_DataOut +=sizeof(*temp_SumSpeed_pointer);

	    	temp_SumSpeed_pointer = (s32*)&Array_Total_DataOut[CountTotal_DataOut];
	    	*temp_SumSpeed_pointer = tempSum_forSpeed_Y;      //Sum for Speed Y
	    	CountTotal_DataOut +=sizeof(*temp_SumSpeed_pointer);

	    	temp_SumSpeed_pointer = (s32*)&Array_Total_DataOut[CountTotal_DataOut];
	    	*temp_SumSpeed_pointer = tempSum_forSpeed_Z;      //Sum for Speed Z
	    	CountTotal_DataOut +=sizeof(*temp_SumSpeed_pointer);
	    	temp_SumMove_pointer32bit_L= (s32*)&Array_Total_DataOut[CountTotal_DataOut];
	    	*temp_SumMove_pointer32bit_L =  tempSum_forMove_X;       //Sum for Move X Low 32 bits
	    	CountTotal_DataOut +=sizeof(*temp_SumMove_pointer32bit_L);

	    	temp_SumMove_pointer32bit_H= (s32*)&Array_Total_DataOut[CountTotal_DataOut];
	    	*temp_SumMove_pointer32bit_H =  (tempSum_forMove_X>>32);       //Sum for Move X High 32 bits
	    	CountTotal_DataOut +=sizeof(*temp_SumMove_pointer32bit_H);
	    	temp_SumMove_pointer32bit_L = (s32*)&Array_Total_DataOut[CountTotal_DataOut];
	    	*temp_SumMove_pointer32bit_L =  tempSum_forMove_Y;       //Sum for Move Y Low 32 bits
	    	CountTotal_DataOut +=sizeof(*temp_SumMove_pointer32bit_L);

	    	temp_SumMove_pointer32bit_H = (s32*)&Array_Total_DataOut[CountTotal_DataOut];
	    	*temp_SumMove_pointer32bit_H =  (tempSum_forMove_Y>>32);       //Sum for Move Y High 32 bits
	    	CountTotal_DataOut +=sizeof(*temp_SumMove_pointer32bit_H);
	    	temp_SumMove_pointer32bit_L = (s32*)&Array_Total_DataOut[CountTotal_DataOut];
	    	*temp_SumMove_pointer32bit_L =  tempSum_forMove_Z;       //Sum for Move Z Low 32 bits
	    	CountTotal_DataOut +=sizeof(*temp_SumMove_pointer32bit_L);

	    	temp_SumMove_pointer32bit_H = (s32*)&Array_Total_DataOut[CountTotal_DataOut];
	    	*temp_SumMove_pointer32bit_H =  tempSum_forMove_Z;       //Sum for Move Z High 32 bits
	    	CountTotal_DataOut +=sizeof(*temp_SumMove_pointer32bit_H);
    //------------------------Gyro giroscope------------------------------------
            	tempGiro_Gyroscope_pointer = (s32*)&Array_Total_DataOut[CountTotal_DataOut];
    	    	*tempGiro_Gyroscope_pointer = tempSum_Giro_Gyroscope; // Sum Gyroscope
    	    	CountTotal_DataOut +=sizeof(*tempGiro_Gyroscope_pointer);
    //---------------------Temperature Accelerometer--------
	    	tempTemperature_pointer = (u16*)&Array_Total_DataOut[CountTotal_DataOut];
	    	*tempTemperature_pointer = tempTemperature_Acceler; // Temperature Accelerometer
	    	CountTotal_DataOut +=sizeof(*tempTemperature_pointer);
//------------------------Gyroscope temperature------------------------------------
              	temp_Temper_Gyroscope_pointer = (u16*)&Array_Total_DataOut[CountTotal_DataOut];
        	    	*temp_Temper_Gyroscope_pointer = temp_Temperature_Gyroscope; // Temperature Gyroscope
        	    	CountTotal_DataOut +=sizeof(*temp_Temper_Gyroscope_pointer);
//----------------data GK
                temp_GK_Data_pointer = (u16*)&Array_Total_DataOut[CountTotal_DataOut];
    			*temp_GK_Data_pointer = temp_GK_Data;  //data GK
    			CountTotal_DataOut +=sizeof(*temp_GK_Data_pointer);

    			temp_GK_Time_count = (u16*)&Array_Total_DataOut[CountTotal_DataOut];
    			*temp_GK_Time_count = temp_GK_Time;  //data Time for GK (count for 500 us)
    			CountTotal_DataOut +=sizeof(*temp_GK_Time_count);
//-----------------------------------------
		TIM_Cmd(TIM4,DISABLE);

		TIM_SetCounter(TIM4, 0); //reset CNT

		FlagForDataPause = true;

		CountBuffForDataPause++;
		if(CountBuffForDataPause>1)CountBuffForDataPause=0;

	    }

	}   //end if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
}
//-------------------------------------------------------------------------------------------------
void Timer2_Init(void)
{
	Flag_TIM2_IRQHandler=false; Count_TIM2_IRQHandler=0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;


	TIM_TimeBaseStructure.TIM_Prescaler = 10;////APB1 AHB_CLK = 36MHz, for time cycle input frequency Timer3 = 1us
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//division ratio between the timer clock (CK_INT) frequency and the dead-time and sampling clock
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Period = 65535;  //

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	//Enable the Capture Compare 1 Interrupt source
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

    TIM_OCStructInit(&TIM_OCInitStructure);//initial default
    /* Set the Capture Compare Register value */
    TIM_OCInitStructure.TIM_Pulse = 2;//2 us delay after 'SS' - SPI2 has been set '1'

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    //TIM_Cmd(TIM2,ENABLE);

}
//-------------------------------------
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
	    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	    Flag_TIM2_IRQHandler=true;
	    TIM_Cmd(TIM2,DISABLE);
	    TIM_SetCounter(TIM2, 0); //reset CNT
	    DMA1_Channel5->CMAR=(u32)&AddCommand_to_Acceler_forRead_Byte[Count_TIM2_IRQHandler];//for Read temperature LSB
	    DMA1_Channel4->CMAR=(u32)&AddData_from_Accelerometer_Byte[Count_TIM2_IRQHandler];
		DMA1_Channel5->CNDTR=2; //always two bytes, because format data single accelerometer 16 bit by SPI2
		DMA1_Channel4->CNDTR=2;
		//not work PORTB.12 on OLIMEX - then to correct !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_RESET); //'SS' SPI2 -> '0' - begin request data from accelerometer by SPI
		GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
		DMA_Cmd(DMA1_Channel5, ENABLE);
		DMA_Cmd(DMA1_Channel4, ENABLE);
		Count_TIM2_IRQHandler+=2;
	}
}
//-------------------------------------------------------------------------------------------------
// Timer For delay----------------inValue/100 = ms + 30 us--------------------------
//-------------------------------------------------------------------------------------------------
void Delay_by_Timer6(u16 inValue)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Prescaler = 720;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//division ratio between the timer clock (CK_INT) frequency and the dead-time and sampling clock
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Period = inValue;  //

	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    TIM_SetCounter(TIM6, 0); //reset CNT

    TIM_Cmd(TIM6,ENABLE);
    while((TIM_GetFlagStatus(TIM6, TIM_FLAG_Update) == RESET));
    TIM_ClearFlag(TIM6,TIM_FLAG_Update);
    while((TIM_GetFlagStatus(TIM6, TIM_FLAG_Update) == RESET));
    TIM_ClearFlag(TIM6,TIM_FLAG_Update);

    TIM_Cmd(TIM6,DISABLE);
}
//-------------------------------------------------------
/*
 * This timer use for GK channel
 *
 */
void Timer8_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ETRClockMode2Config(TIM8,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//division ratio between the timer clock (CK_INT) frequency and the dead-time and sampling clock
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Period = 65535;  //All time period = 655,35 ms

	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM8,ENABLE);
}

