#include "main.h"

volatile u16 *temp_Value_for_Timer_pointer;
volatile u32 i1, j, i=0;
volatile u32 Temp_start_addr;
int firDecimationCount=0;
int filtredValue=0;
u32 TimeDelay=0;


//------------------

int main(void)
{
	Set_Data_for_Timer();
	Flag_Command_Reset_SpeedMove_Accelerometer=false;
	Flag_Command_Reset_Azimuth_Giroscope = false;
	Flag_Command_InitModulGyroscopeAcseler=false;
	FlagDMA1_Channel3_Trans_Yes=false;

	Set_Data_for_Accelerometer();
	Set_Data_for_Gyroscope();
	Init_System_Clock();
	NVIC_Configuration();
	Ports_Config();

	DMA1_Chan6_init();  //for save CNT TIM8
	DMA1_Chan5_init();  //for Tx SPI2
	DMA1_Chan4_init();  //for Rx SPI2
	DMA2_Chan1_init();  //for Rx SPI3
	DMA2_Chan2_init();  //for Tx SPI3
		Temp_start_addr = (u32)&Array_CommandOut[0];
		DMA1_Chan3_init_set(Temp_start_addr, 3);  
	 Temp_start_addr = (u32)&Array_CommandIn[0];
	 DMA1_Chan2_init_set(Temp_start_addr,5,true);

	SPI1_MAIN_Init();
	SPI2_ACCELER_Init();
	SPI3_HYROSCOPE_Init();

	Timer2_Init();
	Timer4_Init();
	Timer3_Init();
	Timer8_Init(); // for GK
	Set_Data_for_SPI1_MAIN();
	USART_SetGain_Init();
	USART_SetGain_SendData(0);//- 5k
	Timer1_Init();

	ADC1_I_Init();
	ADC3_U_Init();

	 Start_Up_Accelerometer();

	 FlagDMA1_Channel2_Trans_Yes=false;
	 Quantity_Total_Data_Byte_Out=0;
	 Set_Data_for_Timer();
	// Arr_SetTimer_for_Sampling_U_I = NULL;

	 SampleFilter_init(&sf);
		while(1)//||Flag_Command_Reset_SpeedMove_Accelerometer
	    {
Beg_res:		
			 while(FlagDMA1_Channel3_Trans_Yes||FlagDMA1_Channel2_Trans_Yes);
			
			 while(!(GPIOB->IDR & GPIO_Pin_0));
			 while((GPIOB->IDR & GPIO_Pin_0));
				 u32 temp_start_addr = (u32)&Array_CommandIn[0];
				 DMA1_Chan2_init_set(temp_start_addr,7,true);
		    	 GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET);
		    	 delay_us(10);
		    	 GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);		    	
		    	 Setdelay_us(2000);
		    	 bool FlagTimeOut=FALSE;
             while(!FlagDMA1_Channel2_Trans_Yes&&!FlagTimeOut)//ожидаем пока примется входной массив через SPI1 или ждем 10 мс
            	 {
            	 FlagTimeOut= SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk;
            	 i=0;
            	 }
             SysTick->CTRL=0;
             if(FlagTimeOut) {goto Beg_res;}
             FlagDMA1_Channel2_Trans_Yes=false;
 		  	if(Array_CommandIn[0]==Command_PuseDataRequest)//&&CountTransmitData_SPI1=0)//&&Count_SPI1>0)//Count_SPI1>0????????????????????????
  		    	   {
 		  		if(CountBuffForDataPause==0)
 		  		{
 			  		QuantityTransmitWord16DataPause_SPI1=indexForDataPause;
 	 		  		indexForDataPause=2;
 		  		LSB(ArrayForOutDataPuse[0])=Command_PuseDataRequest; 
 		  		MSB(ArrayForOutDataPuse[0])=0x0;
 		  		ArrayForOutDataPuse[1]=QuantityTransmitWord16DataPause_SPI1-2;
 		  		}
 		  		if(CountBuffForDataPause==1)
 		  		  		{
 			  		QuantityTransmitWord16DataPause_SPI1=indexForDataPause1;
 	 		  		indexForDataPause1=2;
 		  		  		LSB(ArrayForOutDataPuse1[0])=Command_PuseDataRequest; 
 		  		  		MSB(ArrayForOutDataPuse1[0])=0x0; 
 		  		  		ArrayForOutDataPuse1[1]=QuantityTransmitWord16DataPause_SPI1-2;
 		  		  		}

  		    	    if((QuantityTransmitWord16DataPause_SPI1-2)%2==0) Quantity_PauseData_forCRC32 = (QuantityTransmitWord16DataPause_SPI1-2)/2;
  		    	    if((QuantityTransmitWord16DataPause_SPI1-2)%2!=0)
  		    	    	{
  		    	    	Quantity_PauseData_forCRC32 = ((QuantityTransmitWord16DataPause_SPI1-2)/2)+1;
  		    	    	if(CountBuffForDataPause==0)
  		    	    	for(i=QuantityTransmitWord16DataPause_SPI1;i<= (QuantityTransmitWord16DataPause_SPI1 + (4-((QuantityTransmitWord16DataPause_SPI1-2)%4)));i++)ArrayForOutDataPuse[i]=0;

 		    	    	if(CountBuffForDataPause==1)
  		    	    	for(i=QuantityTransmitWord16DataPause_SPI1;i<= (QuantityTransmitWord16DataPause_SPI1 + (4-((QuantityTransmitWord16DataPause_SPI1-2)%4)));i++)ArrayForOutDataPuse1[i]=0;
  		    	    	}
  		    	    CRC_ResetDR();
  		    	  if(CountBuffForDataPause==0)
  		    	  {
  		    	  CRC_PauseData = CRC_CalcBlockCRC((u32*)&ArrayForOutDataPuse[2],Quantity_PauseData_forCRC32);
  		    	    ArrayForOutDataPuse[QuantityTransmitWord16DataPause_SPI1]  = LSW(CRC_PauseData);
  		    	    ArrayForOutDataPuse[QuantityTransmitWord16DataPause_SPI1+1]= MSW(CRC_PauseData);
  		    	  }
  		    	  if(CountBuffForDataPause==1)
  	  		    	  {
  	  		    	  CRC_PauseData = CRC_CalcBlockCRC((u32*)&ArrayForOutDataPuse1[2],Quantity_PauseData_forCRC32);
 	  		    	    ArrayForOutDataPuse1[QuantityTransmitWord16DataPause_SPI1]  = LSW(CRC_PauseData);
  	  		    	    ArrayForOutDataPuse1[QuantityTransmitWord16DataPause_SPI1+1]= MSW(CRC_PauseData);
  	  		    	  }
   DMA1_Chan2_init_set8_16((u32)&Temp_start_addr,(QuantityTransmitWord16DataPause_SPI1+2),false, DMA_PeripheralDataSize_HalfWord, DMA_MemoryDataSize_HalfWord);						 
						 while(!(GPIOB->IDR & GPIO_Pin_0));
 						 while((GPIOB->IDR & GPIO_Pin_0));
 						 SPI1_MAIN_Init_16();
 						if(CountBuffForDataPause==0)Temp_start_addr = (u32)&ArrayForOutDataPuse[0];
 						if(CountBuffForDataPause==1)Temp_start_addr = (u32)&ArrayForOutDataPuse1[0];
   DMA1_Chan3_init_set8_16(Temp_start_addr, QuantityTransmitWord16DataPause_SPI1+2, DMA_PeripheralDataSize_HalfWord, DMA_MemoryDataSize_HalfWord); 

 				    	 GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET);
 				    	 delay_us(10);
 				    	 GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);

 						while(!FlagDMA1_Channel2_Trans_Yes);
 						SPI1_MAIN_Init();

 						FlagDMA1_Channel2_Trans_Yes =false;
 						FlagDMA1_Channel3_Trans_Yes=false;
  		    	   }
 			//###############################################Command_DataRequest#######################################
 		  	if(Array_CommandIn[0]==Command_DataRequest)//&&CountTransmitData_SPI1=0)//&&Count_SPI1>0)//Count_SPI1>0????????????????????????
 		    	   {

 		    		QuantityTransmitByte_SPI1=CountTotal_DataOut;
 		
 		    	    Array_Total_DataOut[0]=Command_DataRequest; 
 		    	    Array_Total_DataOut[1]=0x0; 
 		    	    Flag_Command_DataRequest=false;
 		    	    if((QuantityTransmitByte_SPI1-2)%4==0) Quantity_Data_forCRC32 = (QuantityTransmitByte_SPI1-2)/4;
 		    	    if((QuantityTransmitByte_SPI1-2)%4!=0)
 		    	    	{
 		    	    	Quantity_Data_forCRC32 = ((QuantityTransmitByte_SPI1-2)+(4-(QuantityTransmitByte_SPI1-2)%4))/4;

 		    	    	for(i=QuantityTransmitByte_SPI1;i<= (QuantityTransmitByte_SPI1 + (4-((QuantityTransmitByte_SPI1-2)%4)));i++)Array_Total_DataOut[i]=0;
 		    	    	}
 		    	    CRC_ResetDR();
 		    	    CRC_Data = CRC_CalcBlockCRC((u32*)&Array_Total_DataOut[2],Quantity_Data_forCRC32);
 		    	    Array_Total_DataOut[QuantityTransmitByte_SPI1]=LSB0(CRC_Data);
 		    	    Array_Total_DataOut[QuantityTransmitByte_SPI1+1]=LSB1(CRC_Data);
 		    	    Array_Total_DataOut[QuantityTransmitByte_SPI1+2]=LSB2(CRC_Data);
 		    	    Array_Total_DataOut[QuantityTransmitByte_SPI1+3]=LSB3(CRC_Data);
  DMA1_Chan2_init_set8_16((u32)&Temp_start_addr,(QuantityTransmitByte_SPI1+4)/2,false, DMA_PeripheralDataSize_HalfWord, DMA_MemoryDataSize_HalfWord);					
						 while(!(GPIOB->IDR & GPIO_Pin_0));
						 while((GPIOB->IDR & GPIO_Pin_0));
						 SPI1_MAIN_Init_16();
			     		Temp_start_addr = (u32)&Array_Total_DataOut[0];
			     		//DMA to 16 bit
  DMA1_Chan3_init_set8_16(Temp_start_addr, (QuantityTransmitByte_SPI1+4)/2, DMA_PeripheralDataSize_HalfWord, DMA_MemoryDataSize_HalfWord); 	     		                                                             
				    	 GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET);
				    	 delay_us(10);
				    	 GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);
						while(!FlagDMA1_Channel2_Trans_Yes);
						SPI1_MAIN_Init();

						FlagDMA1_Channel2_Trans_Yes =false;
						FlagDMA1_Channel3_Trans_Yes=false;
 		    	   }
 //#######################################Command_InitModulGyroscopeAcseler##########################################################################
             if(Array_CommandIn[0]==Command_InitModulGyroscopeAcseler)
             {
            	// Flag_Command_InitModulGyroscopeAcseler=true;
                 LSB(TimeCurretPause_SPI) = Array_CommandIn[1];
                 Array_CommandInForCRC[0] = Array_CommandIn[1];

                 MSB(TimeCurretPause_SPI) = Array_CommandIn[2];
                 Array_CommandInForCRC[1] = Array_CommandIn[2];

                 LSB(QuantityPointsMainSonde_SPI) = Array_CommandIn[3];
                 Array_CommandInForCRC[2] = Array_CommandIn[3];

                 MSB(QuantityPointsMainSonde_SPI) = Array_CommandIn[4];
                 Array_CommandInForCRC[3] = Array_CommandIn[4];

                 LSB(Time_BeforeStartADC24_SPI) = Array_CommandIn[5];
                 Array_CommandInForCRC[4] = Array_CommandIn[5];

                 MSB(Time_BeforeStartADC24_SPI) = Array_CommandIn[6];
                 Array_CommandInForCRC[5] = Array_CommandIn[6];

                 Array_CommandInForCRC[6] = 0;
                 Array_CommandInForCRC[7] = 0;

		    		QuantityCommandBytesInSPI1=7; 
		    		QuantityCommandBytesOutSPI1=7; 

		               QuantityReceiveDataUI_SPI = (10*TimeCurretPause_SPI)/500;
		                                                                 
		               QuantityReceiveByteUI_SPI = QuantityReceiveDataUI_SPI * 2;  //Because QuantityReceiveData - words 2 byte, and data receive SPI1 - 1 byte

		               Total_TimePollingAccelerGyrosk=8*QuantityPointsMainSonde_SPI;
		               Array_CommandOut[0] = 0x77;
		               Array_CommandOut[1]=Command_InitModulGyroscopeAcseler; 
		               Array_CommandOut[2]=0x0; 

		               CRC_ResetDR();
		               CRC_Command = CRC_CalcBlockCRC((u32*)&Array_CommandInForCRC[0],2);
		               Array_CommandOut[3]=LSB0(CRC_Command);
		               Array_CommandOut[4]=LSB1(CRC_Command);
		               Array_CommandOut[5]=LSB2(CRC_Command);
		               Array_CommandOut[6]=LSB3(CRC_Command);
		               //-----------------------

		     
		           	j=0;
		           	for(i=0;i< QuantityReceiveByteUI_SPI;i+=sizeof(*temp_Value_for_Timer_pointer))
		           		{
		      	      temp_Value_for_Timer_pointer = (u16*)&Arr_SetTimer_for_Sampling_U_I[i];
		      	      *temp_Value_for_Timer_pointer = 50 +j*50; //заполнение  (один отсчет - 10us - TIM1)500 us
		      	      j++;
		           		}

		   			Quantity_Total_Data_Byte_U_I = QuantityReceiveByteUI_SPI + QuantityReceiveByteUI_SPI;// for U and I
		   			Quantity_Total_Data_Byte_Out = Quantity_Total_Data_Byte_Out + Quantity_Total_Data_Byte_U_I;
		           	QuantityReceiveDataAcceler_SPI = (10*Time_BeforeStartADC24_SPI+8*QuantityPointsMainSonde_SPI)/500; //общее время токовая пауза + время для спада с зонда; 500 мкс - миним. время опроса в этом модуле
		            QuantityReceiveByteAcceler_SPI = QuantityReceiveDataAcceler_SPI * 2;
		         	Quantity_Total_Data_Byte_Accelerometer = QuantityReceiveDataAcceler_SPI * 8 + 6 + 36 + 2;
		         	Quantity_Total_Data_Byte_Gyroscope = 4+2;
		         	Quantity_Total_Data_Byte_GK = 2+2;
		         	Quantity_Total_Data_Byte_Out = Quantity_Total_Data_Byte_Out + Quantity_Total_Data_Byte_Accelerometer+Quantity_Total_Data_Byte_Gyroscope+Quantity_Total_Data_Byte_GK+3+4;
			     		DMA1_Chan2_init_set((u32)&Temp_start_addr,QuantityCommandBytesOutSPI1,false);
						 while(!(GPIOB->IDR & GPIO_Pin_0));
						 while((GPIOB->IDR & GPIO_Pin_0));
			     		Temp_start_addr = (u32)&Array_CommandOut[0];
			     		DMA1_Chan3_init_set(Temp_start_addr, QuantityCommandBytesOutSPI1);  
				    	 GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET);
				    	 delay_us(10);
				    	 GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);

						while(!FlagDMA1_Channel2_Trans_Yes);
						FlagDMA1_Channel2_Trans_Yes =false;
						FlagDMA1_Channel3_Trans_Yes=false;
             }
 //###############################################Command_Reset_Azimuth_Giroscope#######################################################
 			      if(Array_CommandIn[0]==Command_Reset_Azimuth_Giroscope)
 			   {
 				     Sum_Giro_Gyroscope=0;

 			          Array_CommandOut[0] = 0x77;
 				      Array_CommandOut[1]=Command_Reset_Azimuth_Giroscope; // номер команды
 					  Array_CommandOut[2]=0x0; 
 					 Array_CommandOut[3]=0x0;
 					Array_CommandOut[4]=0x0;
 					Array_CommandOut[5]=0x0;
 					Array_CommandOut[6]=0x0;
 
				     		DMA1_Chan2_init_set((u32)&Temp_start_addr,7,false);						
							 while(!(GPIOB->IDR & GPIO_Pin_0));
							 while((GPIOB->IDR & GPIO_Pin_0));
				     		Temp_start_addr = (u32)&Array_CommandOut[0];
				     		DMA1_Chan3_init_set(Temp_start_addr, 7);  
                      
					    	 GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET);
					    	 delay_us(10);
					    	 GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);

							while(!FlagDMA1_Channel2_Trans_Yes);

							FlagDMA1_Channel2_Trans_Yes =false;
							FlagDMA1_Channel3_Trans_Yes=false;
 		     }


//#################################################################Command_Reset_SpeedMove_Accelerometer#########################################
             if(Array_CommandIn[0]==Command_Reset_SpeedMove_Accelerometer)
              {
 
					     		Sum_forSpeed_X = 0;
					    		Sum_forSpeed_Y = 0;
					    		Sum_forSpeed_Z = 0;

					    		Sum_forMove_X = 0;
					    		Sum_forMove_Y = 0;
					    		Sum_forMove_Z = 0;

					    	    Array_CommandOut[0] = 0x77;
					    	    Array_CommandOut[1]=Command_Reset_SpeedMove_Accelerometer; 
					    	    Array_CommandOut[2]=0x0; 
					    		 Array_CommandOut[3]=0x0;
					    	 					Array_CommandOut[4]=0x0;
					    	 					Array_CommandOut[5]=0x0;
					    	 					Array_CommandOut[6]=0x0;
					     		DMA1_Chan2_init_set((u32)&Temp_start_addr,7,false);

						
								 while(!(GPIOB->IDR & GPIO_Pin_0));
								 while((GPIOB->IDR & GPIO_Pin_0));


					     		Temp_start_addr = (u32)&Array_CommandOut[0];
					     		DMA1_Chan3_init_set(Temp_start_addr, 7);  

						    	 GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET);
						    	 delay_us(10);
						    	 GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);

								while(!FlagDMA1_Channel2_Trans_Yes);

								FlagDMA1_Channel2_Trans_Yes =false;
								FlagDMA1_Channel3_Trans_Yes=false;

              }

	    }
}

