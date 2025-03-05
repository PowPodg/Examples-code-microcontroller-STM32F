/*
 * main.h
 *
 * Created: 04.03.2013 16:47:13
 *  Author: Olegiv
 */


#ifndef MAIN_H_
#define MAIN_H_
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
//#include <stdint-gcc.h>
//#include <parallel/types.h>

#include <stdint.h>
//#include "stm32f10x_conf.h"

#include "stm32f10x.h"
#include "compiler.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_crc.h"

#include "math.h"

#include "CRC32.h"
#define SizeArray_U_I                         500
#define SizeArray_Accelerometer               150

#include "SampleFilter.h"
#include "systemM.h"
#include "ports.h"
#include "spi.h"
#include "usart.h"
#include "analog_in.h"
#include "Timer.h"

extern int firDecimationCount;

extern int filtredValue;








#endif /* MAIN_H_ */
