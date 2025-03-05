
#include "CRC32.h"

uint32_t sw_calculate_soft_crc(uint32_t *pinput, uint32_t length)
{
	uint32_t initial_crc = 0xFFFFFFFF;               
	uint32_t* input_data = pinput;
	uint32_t polynom     = 0x4C11DB7 ;                  
	uint32_t crc         = 0         ;               
	uint32_t bindex      = 0;

    while( index<length )
    {
    if(index==0){   crc = initial_crc ^ input_data[index];}
    else        {   crc = crc ^ input_data[index];        }

        while(   bindex<(sizeof(input_data[index])*8)   )
        {	// if MSB of CRC = 1
            if((crc>>31)==1){crc=(crc<<1)^polynom;}
            else            {crc=(crc<<1);        }
            bindex++;
        }

    bindex=0;
    index++;
    }

    return crc;
}
