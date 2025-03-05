#ifndef _analog_in_H_
#define _analog_in_H_

#include "main.h"

//U16 Data_ADC1_Data_I_DMA[SizeArray_U_I];
//U16 Data_ADC3_Data_U_DMA[SizeArray_U_I];

extern volatile u16 Data_ADC_Data_IU_DMA[SizeArray_U_I*2]; //total array for voltage and current

void ADC1_I_Init(void);
void ADC3_U_Init(void);

#endif
