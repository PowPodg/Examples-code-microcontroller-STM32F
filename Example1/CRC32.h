#ifndef __CRC32
#define __CRC32

#include "main.h"
//#include "stm32f4xx.h"
//#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_crc.h"
//#include "stm32f4xx_gpio.h"

extern uint32_t sw_checksum;
uint32_t sw_calculate_crc(uint32_t *input_data, uint32_t q);
uint32_t sw_calculate_soft_crc(uint32_t *pinput, uint32_t length);


#endif
