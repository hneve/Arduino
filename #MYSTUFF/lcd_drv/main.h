#ifndef _main_h_
#define _main_h_
#include <avr/io.h>
#include "mydefs.h"

#define	F_CPU           16e6             // 16MHz
#include <util/delay.h>

/***************************************************************************/
/*                       define the wanted LCD type:                       */
/***************************************************************************/
//#define LCD_1X8
//#define LCD_1X16
//#define LCD_1X20
//#define LCD_1X40
//#define LCD_2X8       // some 1x16 are wired as 2x8
//#define LCD_2X12
#define LCD_2X16
//#define LCD_2X20
//#define LCD_2X24
//#define LCD_2X40
//#define LCD_4X16
//#define LCD_4X20

/***************************************************************************/
/*                       define the LCD connections:                       */
/***************************************************************************/
#define	LCD_D4		SBIT( PORTB, 3 )
#define	LCD_DDR_D4	SBIT( DDRB, 3 )

#define	LCD_D5		SBIT( PORTB, 5 )
#define	LCD_DDR_D5	SBIT( DDRB, 5 )

#define	LCD_D6		SBIT( PORTC, 5 )
#define	LCD_DDR_D6	SBIT( DDRC, 5 )

#define	LCD_D7		SBIT( PORTC, 4 )
#define	LCD_DDR_D7	SBIT( DDRC, 4 )

#define	LCD_RS		SBIT( PORTC, 6 )
#define	LCD_DDR_RS	SBIT( DDRC, 6 )

#define	LCD_E0		SBIT( PORTC, 3 )
#define	LCD_DDR_E0	SBIT( DDRC, 3 )


#endif
