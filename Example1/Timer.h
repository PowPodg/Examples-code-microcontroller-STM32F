#ifndef _Timers_H_
#define _Timers_H_

#include "main.h"


#define WIDTH_PULSE_TIM1_TIM3    488



//u16 Arr_SetTimer_for_Sampling_U_I[SizeArray_U_I];
extern volatile u8 Arr_SetTimer_for_Sampling_U_I[10000];
extern volatile u16 CountInterruptCapture_Timer1;

//u8  *Arr_SetTimer_for_Sampling_Acceler;
extern volatile u8  Arr_SetTimer_for_Sampling_Acceler[2000];
extern volatile u16 CountInterruptCapture_Timer4;


void Set_Data_for_Timer(void);

void Timer1_Init(void);
void Arr_SetTimer_Default(void);

void Timer3_Init(void);
void Timer4_Init(void);

extern volatile bool FlagInterruptTimer3;
extern volatile bool FlagInterruptTimer3_OC3_OC4;

extern volatile u16 DataCNT_TIM8;

//u16 ArrX_SetTimer_for_Sampling_Accelerometer[SizeArray_Accelerometer];
//u16 ArrY_SetTimer_for_Sampling_Accelerometer[SizeArray_Accelerometer];
//u16 ArrZ_SetTimer_for_Sampling_Accelerometer[SizeArray_Accelerometer];

extern volatile u16 ArrayForOutDataPuse[10000];//введен дополнительнло массив для записи данных Гироскопа (потом иакселерометра)
                                            //в паузе межу окончанием спада и началом токовой паузы
extern volatile u16 ArrayForOutDataPuse1[10000];

extern volatile u8 CountBuffForDataPause;

extern volatile u16 CountInterruptCapture_Timer4;

extern volatile u16 CountTotal_DataOut;

extern volatile u16 *temp_Value_for_Timer1_pointer, *temp_Value_for_Timer4_pointer;
extern volatile s16 *temp_Value_for_DataOut_pointer;
extern volatile s32 tempSum_forSpeed_X, tempSum_forSpeed_Y, tempSum_forSpeed_Z;
extern volatile s32 *temp_SumSpeed_pointer;
//int_fast64_t tempSum_forMove_X, tempSum_forMove_Y, tempSum_forMove_Z;
extern volatile int64_t tempSum_forMove_X, tempSum_forMove_Y, tempSum_forMove_Z;
extern volatile int_fast64_t *temp_SumMove_pointer;
extern volatile s32 *temp_SumMove_pointer32bit_H, *temp_SumMove_pointer32bit_L;

extern volatile u16 tempTemperature_Acceler;
extern volatile u16 *tempTemperature_pointer;

extern volatile s16 tempData_Giro_Gyroscope;
extern volatile s32 tempSum_Giro_Gyroscope;
extern volatile s32 *tempGiro_Gyroscope_pointer;
extern volatile u16 temp_Temperature_Gyroscope;
extern volatile u16 *temp_Temper_Gyroscope_pointer;

//u16 temp_GK_Data, temp_GK_Time_count;
extern volatile u16 *temp_GK_Data_pointer;
extern volatile u16 *temp_GK_Time_count;
extern volatile u16 temp_GK_Data;
extern volatile u16 temp_GK_Time;

void Timer2_Init(void);
void Delay_by_Timer6(u16 inValue);

extern volatile bool Flag_TIM2_IRQHandler;
extern volatile u8 Count_TIM2_IRQHandler;

void Timer8_Init(void);


#endif
