#ifndef _spi_H_
#define _spi_H_


#include "main.h"


//---------------------------------
extern volatile u16 Quantity_Total_Data_Byte_U_I;
extern volatile u16 Quantity_Total_Data_Byte_Accelerometer;
extern volatile u16 Quantity_Total_Data_Byte_Gyroscope;
extern volatile u16 Quantity_Total_Data_Byte_GK;

extern volatile u16 Count_SPI1;
extern volatile u16 Count_Tx_SPI1;
extern volatile U8 Comand_Receive_SPI1;
extern volatile u16 QuantityReceiveDataUI_SPI, QuantityReceiveByteUI_SPI;
extern volatile u16 QuantityReceiveDataAcceler_SPI, QuantityReceiveByteAcceler_SPI;
extern volatile u16 TimeCurretPause_SPI;
extern volatile u16 Time_BeforeStartADC24_SPI;

extern volatile u16 QuantityPointsMainSonde_SPI;
extern volatile u32 Total_TimePollingAccelerGyrosk;
extern volatile u32 QuantityIntervalAcseler;

//u8  *Array_Total_DataOut;
extern volatile u8 		         	Array_Total_DataOut[20000];
//u8  *Array_CommandOut;
extern volatile u8  Array_CommandIn[20];
extern volatile u8  Array_CommandOut[20];

//u8  *Array_CommandInForCRC;
extern volatile u8  Array_CommandInForCRC[10];

extern volatile u32  CRC_Data, CRC_Command;
extern volatile u32  Quantity_Data_forCRC32;

extern volatile u32  Quantity_PauseData_forCRC32;
extern volatile u32  CRC_PauseData;

extern volatile u16 Quantity_Total_Data_Byte_Out;
extern volatile u16 QuantityCommandBytesInSPI1;
extern volatile u16 QuantityCommandBytesOutSPI1;


u8  Error_AllocationMemory;

#define Number_Error_AllocationMemory_Acceler                  0x01
#define Number_Error_AllocationMemory_UI                       0x02
#define Not_Error_AllocationMemory                             0x00

extern volatile bool Flag_Command_InitModulGyroscopeAcseler;
extern volatile bool Flag_Command_Reset_SpeedMove_Accelerometer;
extern volatile bool Flag_Command_Reset_Azimuth_Giroscope;
extern volatile bool Flag_Command_DataRequest;


//u16 CountReceiveData_SPI1;
extern volatile u16 QuantityTransmitByte_SPI1;

extern volatile u16 QuantityTransmitWord16DataPause_SPI1;

extern volatile u16 CountTransmitData_SPI1;
//u8 ReceiveData_SPI1[SizeArray_U_I*2];

//#define Quantity_CommandsAccelerometer              8
//u16 DataOut_Accelerometer_HalfWord[Quantity_CommandsAccelerometer];


//1 byte - begin address for cycle sampling accelerometer (beginning from address 'Z')
//2 - byte Z_MSB
//3 - byte Z_LSB
//4 - byte Y_MSB
//5 - byte Y_LSB
//6 - byte X_MSB
//7 - byte X_LSB
#define Quantity_DataAccelerometer                  7
extern volatile u8 Command_to_Acceler_forRead_Byte[Quantity_DataAccelerometer];
extern volatile u8 Data_from_Accelerometer_Byte[Quantity_DataAccelerometer];
extern volatile u8 AddCommand_to_Acceler_forRead_Byte[14];
extern volatile u8 AddCommand_to_Acceler_forWtite_Byte[14];
extern volatile u8 AddData_from_Accelerometer_Byte[14];

extern volatile s16 *DataAcceler_Z, *DataAcceler_Y, *DataAcceler_X;
extern volatile s16 temp_DataAcceler_X, temp_DataAcceler_Y, temp_DataAcceler_Z;
extern volatile u8 Count_Overflow_X_PLUS, Count_Overflow_X_MINUS;
extern volatile u8 Count_Overflow_Y_PLUS, Count_Overflow_Y_MINUS;
extern volatile u8 Count_Overflow_Z_PLUS, Count_Overflow_Z_MINUS;

extern volatile s32 Sum_forSpeed_X, Sum_forSpeed_Y, Sum_forSpeed_Z;
extern volatile u8 *Temperature_LSB, *Temperature_MSB, *INT_STATUS_Acceler;
extern volatile u16 Temperature_Acceler;

#define Quantity_DataGyroscope                     3

extern volatile u8 Data_from_Gyroscope_Byte[Quantity_DataGyroscope+1]; //+1, because - u32 *Data_fromGyroscope
/*it need for shift >> 4 data from gyroscope ADIS16060
 * The internal successive approximation ADC begins the conversion process on the falling edge of MSEL1
 * and starts to place data MSB first on the DOUT line at the 6-th falling edge of SCLK
 */

/* command to Gyroscope:
 * Measure Angular Rate (Gyro) -                        0x20
 * Measure Temperature -                                0x10
 * Measure AIN2  -                                      0x80
 * Measure AIN1  -                                      0x40
 * Set Positive Self-Test and Output for Angular Rate - 0x22
 * Set Negative Self-Test and Output for Angular Rate - 0x21
 */
extern volatile u8 Command_to_Gyroscope_Choice_Channel_Byte[Quantity_DataGyroscope*6]; //6 - total commands for 3 byte
extern volatile u32 *Data_fromGyroscope;
extern volatile s16  Data_Giro_Gyroscope;
extern volatile u32  Data_Giro_Gyroscope_temp;
extern volatile s32  Sum_Giro_Gyroscope;
extern volatile u16  Data_Temperature_Gyroscope;




#define Zero_rate_condition                8192


extern volatile u8 Count_Gyroscope_interrupt;
void SET_Command_to_GYROSCOPE_Init(void);
void Set_Data_for_Gyroscope(void);

#define SAT_bit                            0x40
#define STS_bit                            0x20
#define STC_bit                            0x10

extern volatile bool  Flag_OverflowAcceler;

//2g products accelerometer
//+Full-scale max value
#define MAX_Value_Acceler_PLUS            0x7FF
#define MAX_Value_Acceler_MINUS           0x3800


//Distance passed by the device
extern volatile int64_t Sum_forMove_X, Sum_forMove_Y, Sum_forMove_Z;

void ZeroSumSpeedMove(void);



void SET_Command_to_ACCELER_Init_ZXY(void);
void SET_Command_to_ACCELER_forRead_Init_Add(void);
void ZeroAddData_from_Accelerometer(void);
void SET_Command_to_ACCELER_forWtite_Init_Add(void);
void Set_Data_for_Accelerometer(void);
//-----------------------------------



#define Command_InitArrayTime_Data_UI                          11
#define Command_FillingArray_Time_UI                           12
#define Command_StartMeasurement                               13



#define Command_InitArrayTime_Data_Accelerometer               17
#define Command_FillingArray_Time_Accelerometer                18
#define Command_Query_CodeError                                19
#define Command_InitGyroscope                                  20


#define Command_InitModulGyroscopeAcseler                      1
#define Command_DataRequest                                    2
#define Command_Reset_SpeedMove_Accelerometer                  3
#define Command_Reset_Azimuth_Giroscope                        4
#define Command_SystemReset                                    5
#define Command_PuseDataRequest                                6



//U8 DataFlags_Command_StartMeasurement;
#define Flag_Array_I                             0x01
#define Flag_Array_U                             0x02
//#define Flag_I_U                                 0x03
#define Flag_Array_Acceler                       0x04
#define Flag_SpeedMove_Acceler                   0x08
#define Flag_Temperature_Acceler                 0x10
#define Flag_AzimuthGyroscope                    0x20
#define Flag_Temperature_Gyroscope               0x40



void SPI2_IRQHandler(void);
void SPI1_IRQHandler(void);

void SET_DataOut_ACCELER_Init(void);
void SPI2_ACCELER_Init(void);
void SPI1_MAIN_Init(void);
void SPI1_MAIN_Init_16(void);
void Set_Data_for_SPI1_MAIN(void);
void SPI3_HYROSCOPE_Init(void);
void Start_Up_Accelerometer(void);




#endif
