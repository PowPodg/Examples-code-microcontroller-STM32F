#include "spi.h"
//u8 i, temp2=0;
u32 i,j;
u32 Temp_start_addr;
GPIO_InitTypeDef GPIO_InitStructure;
volatile s32 Sum_forSpeed_X, Sum_forSpeed_Y, Sum_forSpeed_Z;
volatile int64_t Sum_forMove_X, Sum_forMove_Y, Sum_forMove_Z;
volatile u16 Temperature_Acceler;
volatile s32  Sum_Giro_Gyroscope;
volatile u16  Data_Temperature_Gyroscope;
volatile bool  Flag_OverflowAcceler;

volatile u8 Count_Gyroscope_interrupt;



volatile u32  Data_Giro_Gyroscope_temp;

volatile s16  Data_Giro_Gyroscope;


volatile u32 *Data_fromGyroscope;
volatile u8 Command_to_Gyroscope_Choice_Channel_Byte[Quantity_DataGyroscope*6];
volatile u8 Data_from_Gyroscope_Byte[Quantity_DataGyroscope+1];
volatile u16 Temperature_Acceler;
volatile u8 *Temperature_LSB, *Temperature_MSB, *INT_STATUS_Acceler;
volatile u8 Count_Overflow_Z_PLUS, Count_Overflow_Z_MINUS;
volatile u8 Count_Overflow_Y_PLUS, Count_Overflow_Y_MINUS;
volatile u8 Count_Overflow_X_PLUS, Count_Overflow_X_MINUS;
volatile s16 temp_DataAcceler_X, temp_DataAcceler_Y, temp_DataAcceler_Z;
volatile s16 *DataAcceler_Z, *DataAcceler_Y, *DataAcceler_X;
volatile u8 Command_to_Acceler_forRead_Byte[Quantity_DataAccelerometer];
 volatile u8 Data_from_Accelerometer_Byte[Quantity_DataAccelerometer];
 volatile u8 AddCommand_to_Acceler_forRead_Byte[14];
 volatile u8 AddCommand_to_Acceler_forWtite_Byte[14];
 volatile u8 AddData_from_Accelerometer_Byte[14];
  volatile u16 QuantityTransmitByte_SPI1;

  volatile u16 QuantityTransmitWord16DataPause_SPI1;

  volatile u16 CountTransmitData_SPI1;
   volatile bool Flag_Command_InitModulGyroscopeAcseler;
   volatile bool Flag_Command_Reset_SpeedMove_Accelerometer;
   volatile bool Flag_Command_Reset_Azimuth_Giroscope;
   volatile bool Flag_Command_DataRequest;


   volatile u32  CRC_Data, CRC_Command;
    volatile u32  Quantity_Data_forCRC32;

    volatile u32  Quantity_PauseData_forCRC32;
    volatile u32  CRC_PauseData;

    volatile u16 Quantity_Total_Data_Byte_Out;
    volatile u16 QuantityCommandBytesInSPI1;
    volatile u16 QuantityCommandBytesOutSPI1;

    volatile u8  Array_CommandInForCRC[10];
     volatile u8  Array_CommandIn[20];
     volatile u8  Array_CommandOut[20];
     volatile u8 Array_Total_DataOut[20000];

     volatile u16 ArrayForOutDataPuse[10000];
                                             
     volatile u16 ArrayForOutDataPuse1[10000];


      volatile u16 QuantityPointsMainSonde_SPI;
      volatile u32 Total_TimePollingAccelerGyrosk;
      volatile u32 QuantityIntervalAcseler;
       volatile u16 Count_SPI1;
       volatile u16 Count_Tx_SPI1;
       volatile U8 Comand_Receive_SPI1;
       volatile u16 QuantityReceiveDataUI_SPI, QuantityReceiveByteUI_SPI;
       volatile u16 QuantityReceiveDataAcceler_SPI, QuantityReceiveByteAcceler_SPI;
       volatile u16 TimeCurretPause_SPI;
       volatile u16 Time_BeforeStartADC24_SPI;

        volatile u16 Quantity_Total_Data_Byte_U_I;
        volatile u16 Quantity_Total_Data_Byte_Accelerometer;
        volatile u16 Quantity_Total_Data_Byte_Gyroscope;
        volatile u16 Quantity_Total_Data_Byte_GK;

//-----------------------------------------------------------
void ZeroSumSpeedMove(void)
{
	Sum_forSpeed_X=0;
	Sum_forSpeed_Y=0;
	Sum_forSpeed_Z=0;
	Sum_forMove_X=0;
	Sum_forMove_Y=0;
	Sum_forMove_Z=0;

	Count_Overflow_X_PLUS=0; Count_Overflow_X_MINUS=0;
	Count_Overflow_Y_PLUS=0; Count_Overflow_Y_MINUS=0;
	Count_Overflow_Z_PLUS=0; Count_Overflow_Z_MINUS=0;

	DataAcceler_Z = (s16*)&Data_from_Accelerometer_Byte[1];
	DataAcceler_Y = (s16*)&Data_from_Accelerometer_Byte[3];
	DataAcceler_X = (s16*)&Data_from_Accelerometer_Byte[5];

}
/*
 * Array for read data accelerometer in a cycle, with the timer3_Ch3 through SPI2
 */
void SET_Command_to_ACCELER_Init_ZXY(void) {
	Command_to_Acceler_forRead_Byte[0] = 0x25; //command - begin address Z_MSB
	Command_to_Acceler_forRead_Byte[1] = 0x00; //byte for Read data Z_MSB acceleration on Z-axis, MSB
	Command_to_Acceler_forRead_Byte[2] = 0x00; //byte for Read data Z_LSB acceleration on Z-axis, LSB
	Command_to_Acceler_forRead_Byte[3] = 0x00; //byte for Read data Y_MSB acceleration on Y-axis, MSB
	Command_to_Acceler_forRead_Byte[4] = 0x00; //byte for Read data Y_MSB acceleration on Y-axis, LSB
	Command_to_Acceler_forRead_Byte[5] = 0x00; //byte for Read data X_MSB acceleration on X-axis, MSB
	Command_to_Acceler_forRead_Byte[6] = 0x00; //byte for Read data X_LSB acceleration on X-axis, LSB
}

//------------------------Add Command Out Accelerometer------------------------------------------
void SET_Command_to_ACCELER_forRead_Init_Add(void) {
//---------------MOSI data during SPI read command ----------------------
AddCommand_to_Acceler_forRead_Byte[0] = 0x01; //1 byte - Address Command for Read ASIC revision ID (16 bit format accelerometer)
AddCommand_to_Acceler_forRead_Byte[1] = 0x00; //2 byte for Read ASIC revision ID

AddCommand_to_Acceler_forRead_Byte[2] = 0x04;  //1 byte - Address Command for Read CTRL register (16 bit format accelerometer)
AddCommand_to_Acceler_forRead_Byte[3] = 0x00; //2 byte for Read CTRL register

AddCommand_to_Acceler_forRead_Byte[4] = 0x08;  //1 byte - Address Command for Read Status register (16 bit format accelerometer)
AddCommand_to_Acceler_forRead_Byte[5] = 0x00;  //2 byte for Read Status register

AddCommand_to_Acceler_forRead_Byte[6] = 0x49;  //1 byte - Address Command for Read temperature, LSB (16 bit format accelerometer)
AddCommand_to_Acceler_forRead_Byte[7] = 0x01;  //2 Read temperature, LSB

AddCommand_to_Acceler_forRead_Byte[8] = 0x4C;  //1 byte - Address Command for Read temperature, MSB (16 bit format accelerometer)
AddCommand_to_Acceler_forRead_Byte[9] = 0x02;  //2 Read temperature, MSB

AddCommand_to_Acceler_forRead_Byte[10] = 0x58;  //1 byte - Address Command for Read INT_STATUS register (16 bit format accelerometer)
AddCommand_to_Acceler_forRead_Byte[11] = 0x00;  //2 byte for Read INT_STATUS register

AddCommand_to_Acceler_forRead_Byte[12] = 0x9D;  //1 byte - Address Command for Read product ID number (16 bit format accelerometer)
AddCommand_to_Acceler_forRead_Byte[13] = 0x00;  //2 byte for Read product ID number
}
//------------------
//--------MOSI data during write command----------------------------------
void SET_Command_to_ACCELER_forWtite_Init_Add(void) {
AddCommand_to_Acceler_forWtite_Byte[0] = 0x0E;  //Reset component (data C'hex )
AddCommand_to_Acceler_forWtite_Byte[1] = 0x0C;  //

AddCommand_to_Acceler_forWtite_Byte[2] = 0x0E;  //Reset component (data 5'hex )
AddCommand_to_Acceler_forWtite_Byte[3] = 0x05;  //

AddCommand_to_Acceler_forWtite_Byte[4] = 0x0E;  //Reset component (data F'hex )
AddCommand_to_Acceler_forWtite_Byte[5] = 0x0F;  //

AddCommand_to_Acceler_forWtite_Byte[6] = 0x07;  //Set PORST to zero
AddCommand_to_Acceler_forWtite_Byte[7] = 0x00;  //

AddCommand_to_Acceler_forWtite_Byte[8] = 0x07;  //Set chip to power down mode
AddCommand_to_Acceler_forWtite_Byte[9] = 0x20;  //

AddCommand_to_Acceler_forWtite_Byte[10] = 0x07;  //Start self-diagnostic
AddCommand_to_Acceler_forWtite_Byte[11] = 0x08;  //

AddCommand_to_Acceler_forWtite_Byte[12] = 0x07;  //Start memory self-test
AddCommand_to_Acceler_forWtite_Byte[13] = 0x04;  //
}
//--------------------------
void ZeroAddData_from_Accelerometer(void)
{
	AddData_from_Accelerometer_Byte[0] = 0x00; //1 byte for read service data (FRME, PORST, ST, SAT, dPAR bits), (16 bit format accelerometer)
	AddData_from_Accelerometer_Byte[1] = 0x00; //2 byte for Read ASIC revision ID

	AddData_from_Accelerometer_Byte[2] = 0x00;  //1 byte for read service data (FRME, PORST, ST, SAT, dPAR bits), (16 bit format accelerometer)
	AddData_from_Accelerometer_Byte[3] = 0x00; //2 byte for Read CTRL register

	AddData_from_Accelerometer_Byte[4] = 0x00;  //1 byte for read service data (FRME, PORST, ST, SAT, dPAR bits), (16 bit format accelerometer)
	AddData_from_Accelerometer_Byte[5] = 0x00;  //2 byte for Read Status register

	AddData_from_Accelerometer_Byte[6] = 0x00;  //1 byte for read service data (FRME, PORST, ST, SAT, dPAR bits), (16 bit format accelerometer)
	AddData_from_Accelerometer_Byte[7] = 0x00;  //2 Read temperature, LSB
	Temperature_LSB = &AddData_from_Accelerometer_Byte[7];

	AddData_from_Accelerometer_Byte[8] = 0x00;  //1 byte for read service data (FRME, PORST, ST, SAT, dPAR bits), (16 bit format accelerometer)
	AddData_from_Accelerometer_Byte[9] = 0x00;  //2 Read temperature, MSB
	Temperature_MSB = &AddData_from_Accelerometer_Byte[9];

	AddData_from_Accelerometer_Byte[10] = 0x00;  //1 byte for read service data (FRME, PORST, ST, SAT, dPAR bits), (16 bit format accelerometer)
	//INT_STATUS_AccelerLSB = &AddData_from_Accelerometer_Byte[11];
	AddData_from_Accelerometer_Byte[11] = 0x00;  //2 byte for Read INT_STATUS register
	INT_STATUS_Acceler = &AddData_from_Accelerometer_Byte[11];

	AddData_from_Accelerometer_Byte[12] = 0x00;  //1 byte for read service data (FRME, PORST, ST, SAT, dPAR bits), (16 bit format accelerometer)
	AddData_from_Accelerometer_Byte[13] = 0x00;  //2 byte for Read product ID number

	Flag_OverflowAcceler=false;
}
//----------------------------------------------------------
void Set_Data_for_Accelerometer(void)
{
	SET_Command_to_ACCELER_Init_ZXY();
	SET_Command_to_ACCELER_forRead_Init_Add();
	SET_Command_to_ACCELER_forWtite_Init_Add();
	ZeroSumSpeedMove();
	ZeroAddData_from_Accelerometer();


	Quantity_Total_Data_Byte_Out=3;
	Quantity_Total_Data_Byte_Accelerometer=0;
	Error_AllocationMemory=0;
}
//-------------------------------------------------------------------------------------------------------------------------------
void SET_Command_to_GYROSCOPE_Init(void) {
	Command_to_Gyroscope_Choice_Channel_Byte[0] = 0x00;  //1 byte for 8 clock cycles
	Command_to_Gyroscope_Choice_Channel_Byte[1] = 0x00;  //2 byte for 16 clock cycles
	Command_to_Gyroscope_Choice_Channel_Byte[2] = 0x20;  //3 byte for 24 clock cycles - Measure Angular Rate (Gyro)

	Command_to_Gyroscope_Choice_Channel_Byte[3] = 0x00;  //1 byte for 8 clock cycles
	Command_to_Gyroscope_Choice_Channel_Byte[4] = 0x00;  //2 byte for 16 clock cycles
	Command_to_Gyroscope_Choice_Channel_Byte[5] = 0x10;  //3 byte for 24 clock cycles - Measure Temperature

	Command_to_Gyroscope_Choice_Channel_Byte[6] = 0x00;  //1 byte for 8 clock cycles
	Command_to_Gyroscope_Choice_Channel_Byte[7] = 0x00;  //2 byte for 16 clock cycles
	Command_to_Gyroscope_Choice_Channel_Byte[8] = 0x80;  //3 byte for 24 clock cycles - Measure AIN2

	Command_to_Gyroscope_Choice_Channel_Byte[9] = 0x00;  //1 byte for 8 clock cycles
	Command_to_Gyroscope_Choice_Channel_Byte[10] = 0x00; //2 byte for 16 clock cycles
	Command_to_Gyroscope_Choice_Channel_Byte[11] = 0x40; //3 byte for 24 clock cycles - Measure AIN1

	Command_to_Gyroscope_Choice_Channel_Byte[12] = 0x00; //1 byte for 8 clock cycles
	Command_to_Gyroscope_Choice_Channel_Byte[13] = 0x00; //2 byte for 16 clock cycles
	Command_to_Gyroscope_Choice_Channel_Byte[14] = 0x22; //3 byte for 24 clock cycles - Set Positive Self-Test and Output for Angular Rate

	Command_to_Gyroscope_Choice_Channel_Byte[15] = 0x00; //1 byte for 8 clock cycles
	Command_to_Gyroscope_Choice_Channel_Byte[16] = 0x00; //2 byte for 16 clock cycles
	Command_to_Gyroscope_Choice_Channel_Byte[17] = 0x21; //3 byte for 24 clock cycles - Set Negative Self-Test and Output for Angular Rate
}
void Set_Data_for_Gyroscope(void)
{
	Sum_Giro_Gyroscope=0;
	SET_Command_to_GYROSCOPE_Init();
	//1 and 2 bytes 'Data_from_Gyroscope_Byte' - data [1] - MSB, data [2] - LSB

	Data_fromGyroscope = (u32*)&Data_from_Gyroscope_Byte[0]; //Then will need to swap the bytes [0]->[1], [1]->[0]!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*Data_fromGyroscope=0;
}
//----------------------------------------------------------------------
void SPI2_ACCELER_Init(void) {
SPI_InitTypeDef SPI_InitStructure;

SPI_I2S_DeInit(SPI2);
SPI_Cmd(SPI2, DISABLE);
SPI_StructInit(&SPI_InitStructure);
/* SPI configuration -------------------------------------------------------*/
SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
SPI_Init(SPI2, &SPI_InitStructure);

SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);
SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,ENABLE);

/* Enable SPI */
SPI_Cmd(SPI2, ENABLE);

}

//-------------------------------------------------------------------------
void SPI1_MAIN_Init(void) {
SPI_InitTypeDef SPI_InitStructure;
/* SPI configuration -------------------------------------------------------*/
SPI_StructInit(&SPI_InitStructure);



SPI_Init(SPI1, &SPI_InitStructure);
// Enable SPI1 DMA
SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);// DMA immediately writes 1 element array data into  SPI_DR  ??????
SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE);
/* Enable SPI */
SPI_Cmd(SPI1, ENABLE);
}

void SPI1_MAIN_Init_16(void) {
SPI_InitTypeDef SPI_InitStructure;
SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//SPI_SSOutputCmd(SPI1,DISABLE);
SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;//SPI_FirstBit_MSB;
//The CRC polynomial (0007h) is the reset value of this register.
SPI_InitStructure.SPI_CRCPolynomial = 0x07;
SPI_Init(SPI1, &SPI_InitStructure);

// Enable SPI1 DMA
SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE);

/* Enable SPI */
SPI_Cmd(SPI1, ENABLE);

}

void Set_Data_for_SPI1_MAIN(void)
{
	Flag_Start_Cycle_DataReceive_SPI1=false;
	Flag_End_Cycle_DataReceive_SPI1=false;

	Count_SPI1=0;
	Count_Tx_SPI1=0;
	CountTransmitData_SPI1=0;
	CountInterruptCapture_Timer1=0;
	Comand_Receive_SPI1=0;
}
//---------------------------------------------------------------------------------------
void SPI3_HYROSCOPE_Init(void) {
SPI_InitTypeDef SPI_InitStructure;
/* SPI configuration -------------------------------------------------------*/
SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//choice 36 MHz/16 = 2.25 MHz, because for ADIS16060 - max Serial Clock Frequency - 2.9 MHz
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//The CRC polynomial (0007h) is the reset value of this register.
SPI_InitStructure.SPI_CRCPolynomial = 0x07;
SPI_Init(SPI3, &SPI_InitStructure);

/* Enable the RxNE interrupt */

SPI_I2S_DMACmd(SPI3,SPI_I2S_DMAReq_Tx,ENABLE);
SPI_I2S_DMACmd(SPI3,SPI_I2S_DMAReq_Rx,ENABLE);

/* Enable SPI */
SPI_Cmd(SPI3, ENABLE);
}

//---------------Recommended start-up sequence accelerometer-------------------------------
void Start_Up_Accelerometer(void)
{
	delay_us(35000); //Wait 35ms
	/* Wait for SPI2 Tx buffer empty */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	//---1 byte--------------------------Read INT_STATUS-----------------------------------------------------
	    GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET); //'SS' SPI2 -> '0' - begin request data from accelerometer by SPI

		SPI_I2S_SendData(SPI2, 0x58);//AddCommand_to_Acceler_forRead_Byte[10]); //for Read INT_STATUS
	    // Wait for SPI2 data reception
	    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	    SPI_I2S_ClearFlag(SPI2, SPI_I2S_FLAG_TXE | SPI_I2S_FLAG_RXNE);
		AddData_from_Accelerometer_Byte[10] = SPI_I2S_ReceiveData(SPI2);
	//---2 byte---------------
		SPI_I2S_SendData(SPI2, 0x00);//AddCommand_to_Acceler_forRead_Byte[11]); //for Read INT_STATUS
	    // Wait for SPI2 data reception
	    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	    SPI_I2S_ClearFlag(SPI2, SPI_I2S_FLAG_TXE | SPI_I2S_FLAG_RXNE);
		AddData_from_Accelerometer_Byte[11] = SPI_I2S_ReceiveData(SPI2);

		GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);  //'SS' SPI2 -> '1'
	//-------------------------------------------------------------------------------------------------------------
		delay_us(1);

		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_ClearFlag(SPI2, SPI_I2S_FLAG_TXE | SPI_I2S_FLAG_RXNE);

//---------------------------------Set PORST to zero-----------------------------------------------------
       GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET); //'SS' SPI2 -> '0' - begin request data from accelerometer by SPI

	//---1 byte-----------------------
		SPI_I2S_SendData(SPI2, 0x07);//AddCommand_to_Acceler_forWtite_Byte[6]);
		// Wait for SPI2 Tx buffer empty
		  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		  SPI_I2S_ClearFlag(SPI2, SPI_I2S_FLAG_TXE | SPI_I2S_FLAG_RXNE);
		  SPI_I2S_ReceiveData(SPI2);
	//---2 byte---------------
		SPI_I2S_SendData(SPI2, 0x00);//AddCommand_to_Acceler_forWtite_Byte[7]);
		/// Wait for SPI2 Tx buffer empty
		  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		  SPI_I2S_ClearFlag(SPI2, SPI_I2S_FLAG_TXE | SPI_I2S_FLAG_RXNE);
		  SPI_I2S_ReceiveData(SPI2);
		  GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET); //'SS' SPI2 -> '1'
//-----------------------------------------------------------------------------------------------------------------
	 delay_us(10000); //Wait 10ms
	//---1 byte--------------------------Read CTRL-----------------------------------------------------
	    GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET); //'SS' SPI2 -> '0' - begin request data from accelerometer by SPI

		SPI_I2S_SendData(SPI2,AddCommand_to_Acceler_forRead_Byte[2]); //for Read CTRL
	    // Wait for SPI2 data reception
	    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		AddData_from_Accelerometer_Byte[2] = SPI_I2S_ReceiveData(SPI2);
	//---2 byte---------------
		SPI_I2S_SendData(SPI2,AddCommand_to_Acceler_forRead_Byte[3]); //for Read CTRL
	    // Wait for SPI2 data reception
	    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		AddData_from_Accelerometer_Byte[3] = SPI_I2S_ReceiveData(SPI2);

		GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);  //'SS' SPI2 -> '1'

	//---------------------------------Set PORST to zero-----------------------------------------------------
}
