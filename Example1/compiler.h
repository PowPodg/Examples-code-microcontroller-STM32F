
#ifndef _COMPILER_H_
#define _COMPILER_H_

/*_____ I N C L U D E S ____________________________________________________*/


/*_____ D E C L A R A T I O N S ____________________________________________*/

typedef float               Float16;

typedef u8         U8 ;
typedef u16        U16;
typedef u32        U32;
typedef s8         S8 ;
typedef s16        S16;
typedef s32        S32;

typedef unsigned char       Bool;


#define PASS 0
#define FAIL 1



typedef unsigned char       Uchar;


typedef unsigned char       Uint8;
typedef unsigned int        Uint16;
typedef unsigned long int   Uint32;

typedef char                Int8;
typedef int                 Int16;
typedef long int            Int32;

typedef unsigned char       Byte;
typedef unsigned int        Word;
typedef unsigned long int   DWord;


//#define Enable_interrupt() __enable_interrupt()
//#define Disable_interrupt() __disable_interrupt()



/* General purpose defines */
/*#define _ConstType_	__farflash
#define _MemType_
#define _GenericType_ __generic
#define code __farflash
#define xdata
#define idata
#define data*/




/*_____ M A C R O S ________________________________________________________*/


#  define MSB(u16)        (((U8* )&u16)[1])
#  define LSB(u16)        (((U8* )&u16)[0])
#  define MSW(u32)        (((U16*)&u32)[1])
#  define LSW(u32)        (((U16*)&u32)[0])
#  define MSB0(u32)       (((U8* )&u32)[3])
#  define MSB1(u32)       (((U8* )&u32)[2])
#  define MSB2(u32)       (((U8* )&u32)[1])
#  define MSB3(u32)       (((U8* )&u32)[0])
#  define LSB0(u32)       MSB3(u32)
#  define LSB1(u32)       MSB2(u32)
#  define LSB2(u32)       MSB1(u32)
#  define LSB3(u32)       MSB0(u32)

#  define MSB_S(s16)       (((U8* )&s16)[1])
#  define LSB_S(s16)       (((U8* )&s16)[0])


// Endian converters special for accelerometer SCA3100 (format data - MSB - 0,1 bits not used, because shift to right on 2)
#define Le16_ShR_2(b)                   \
   ( (s16)(  ((u16)(     (b) &   0xFF) << 8)      \
   |   (        ((u16)(b) & 0xFF00) >> 8)      \
       )  >> 2  \
   )

// Endian converters
#define Le16(b)                        \
   (  ((U16)(     (b) &   0xFF) << 8)  \
   |  (     ((U16)(b) & 0xFF00) >> 8)  \
   )
#define Le32(b)                             \
   (  ((U32)(     (b) &       0xFF) << 24)  \
   |  ((U32)((U16)(b) &     0xFF00) <<  8)  \
   |  (     ((U32)(b) &   0xFF0000) >>  8)  \
   |  (     ((U32)(b) & 0xFF000000) >> 24)  \
   )

// host to network conversion: used for Intel HEX format, TCP/IP, ...
// Convert a 16-bit value from host-byte order to network-byte order
// Standard Unix, POSIX 1003.1g (draft)
//#define Is_joy_not_down()     ((PINE & 0x20) ?  TRUE : FALSE)


// Constants
//#define ENABLE   1
#define ENABLED  1
#define DISABLED 0
//#define DISABLE  0
#define FALSE   (0==1)
#define TRUE    (1==1)

#define KO      0
#define OK      1
#define OFF     0
#define ON      1
#ifndef NULL
#define NULL    0
#endif
#ifndef ASM_INCLUDE // define ASM_INCLUDE in your a51 source code before include of .h file
#define CLR     0
//#define SET     1
#endif

/* Bit and bytes manipulations */
#define LOW(U16)                ((Uchar)U16)
#define HIGH(U16)               ((Uchar)(U16>>8))
#define TST_BIT_X(addrx,mask)   (*addrx & mask)
#define SET_BIT_X(addrx,mask)   (*addrx = (*addrx | mask))
#define CLR_BIT_X(addrx,mask)   (*addrx = (*addrx & ~mask))
#define OUT_X(addrx,value)      (*addrx = value)
#define IN_X(addrx)             (*addrx)

#  define Max(a, b)            ( (a)>(b) ? (a) : (b) )       // Take the max between a and b
#  define Min(a, b)            ( (a)<(b) ? (a) : (b) )       // Take the min between a and b

// Align on the upper value <val> on a <n> boundary
// i.e. Upper(0, 4)= 4
//      Upper(1, 4)= 4
//      Upper(2, 4)= 4
//      Upper(3, 4)= 4
//      Upper(4, 4)= 8
//      ../..
#  define Upper(val, n)        ( ((val)+(n)) & ~((n)-1) )

// Align up <val> on a <n> boundary
// i.e. Align_up(0, 4)= 0
//      Align_up(1, 4)= 4
//      Align_up(2, 4)= 4
//      Align_up(3, 4)= 4
//      Align_up(4, 4)= 4
//      ../..
#  define Align_up(val, n)     ( ((val)+(n)-1) & ~((n)-1) )

// Align down <val> on a <n> boundary
// i.e. Align_down(0, 4)= 0
//      Align_down(1, 4)= 0
//      Align_down(2, 4)= 0
//      Align_down(3, 4)= 0
//      Align_down(4, 4)= 4
//      ../..
#  define Align_down(val, n)   (  (val)        & ~((n)-1) )

/*M**************************************************************************
* NAME: Long_call
*----------------------------------------------------------------------------
* PARAMS:
* addr: address of the routine to call
*----------------------------------------------------------------------------
* PURPOSE:
* Call the routine at address addr: generate an Assembly LCALL addr opcode.
*----------------------------------------------------------------------------
* EXAMPLE:
* Long_call(0); // Software reset (if no IT used before)
*----------------------------------------------------------------------------
* NOTE:
* May be used as a long jump opcode in some special cases
*****************************************************************************/
#define Long_call(addr)         ((*(void (_ConstType_*)(void))(addr))())

/* {For Langdoc} */

/***********************************************************
 SET_SFR_BIT macro
  parameters
    sfr_reg : defined value in include file for sfr register
    bit_pos : defined value B_XX in include file for particular
              bit of sfr register
    bit_val : CLR / SET
************************************************************/
#define SET_SFR_BIT(sfr_reg, bit_pos, bit_val) { sfr_reg &= ~(1<<(bit_pos)); sfr_reg |= ((bit_val)<<(bit_pos));}

/***********************************************************
 bit_is_clear macro
  parameters
    PORT     : defined value in include file for sfr register
    POSITION : defined value in include file for particular
              bit of sfr register
  example : if (bit_is_clear(PORTB,PORTB3)) ...
************************************************************/
#define bit_is_clear(PORT,POSITION) ((PORT & (1<<POSITION)) == 0 )

/***********************************************************
 bit_is_set macro
  parameters
    PORT     : defined value in include file for sfr register
    POSITION : defined value in include file for particular
              bit of sfr register
  example : if (bit_is_set(PORTB,PORTB3)) ...
************************************************************/
#define bit_is_set(PORT,POSITION) ((PORT & (1<<POSITION)) != 0 )





#define TID_GUARD(proc) ((__TID__ & 0x7FF0) != ((90 << 8) | ((proc) << 4)))



/******************************************************************************/
/* GCC COMPILER                                                               */
/******************************************************************************/
//   #include <avr/sfr_defs.h>
//   #include <avr/interrupt.h>
//   #include <avr/pgmspace.h>
 //  #define Enable_interrupt() sei()
 //  #define Disable_interrupt() cli()


#endif /* _COMPILER_H_ */

