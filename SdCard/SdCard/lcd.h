/*
 * LCD.h
 *
 * Created: 4/2/2014 7:46:00 PM

 */

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"
#include "common_macros.h"
#include <avr/io.h>
#include <util/delay.h>
//#include "micro_config.h"

/* LCD Data bits mode configuration */
#define DATA_BITS_MODE 4

/* Use higher 4 bits in the data port */
#if (DATA_BITS_MODE == 4)
#define UPPER_PORT_PINS
#endif

/* LCD HW Pins */
#define RS PB1
#define RW PB2
#define E  PB3
#define LCD_CTRL_PORT PORTB
#define LCD_CTRL_PORT_DIR DDRB
#define LCD_DATA_PORT PORTA
#define LCD_DATA_PORT_DIR DDRA

/* LCD Commands */
#define CLEAR_COMMAND 0x01
#define FOUR_BITS_DATA_MODE 0x02
#define TWO_LINE_LCD_Four_BIT_MODE 0x28
#define TWO_LINE_LCD_Eight_BIT_MODE 0x38
#define CURSOR_OFF 0x0C
#define CURSOR_ON 0x0E
#define CURSOR_ON_BLINKING 0x0F
#define SET_CURSOR_LOCATION 0x80
#define RIGHT 0
#define LEFT 1
#define SHIFT_DISPLAY_LEFT 0X18
#define SHIFT_DISPLAY_RIGHT 0x1C
#define SHIFT_CURSOR_LEFT 0X10
#define SHIFT_CURSOR_RIGHT 0X14

void LCD_SHIFT_DISPLAT(uint8 direction,uint8 displacement);
void LCD_SHIFT_CURSOR(uint8 direction,uint8 displacement);
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_displayString(const char *Str);
void LCD_init(void);
void LCD_clearScreen(void);
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);
void LCD_goToRowColumn(uint8 row,uint8 col);
void LCD_intgerToString(int data);

/*
#define SMILY 0x00
#define DEGREE 0x01
#define M_FINGURE 0x02
#define Sad_face 0x03

void LCD_WRITE_TO_CGRAM(uint8 address,uint8 data[]);
void LCD_LOAD_SPECAIAL_CHARACTERS();
void LCD_LOAD_BATTERY_SYMBOLS();
void LCD_Display_smily();
void LCD_Display_degree();
void LCD_Display_midfingure();
void LCD_Display_SadFace();
void LCD_CHARGING_ANIMATION();

#ifndef battery
uint8 S_CHAR [4][8] = { // up to 8 characters
		{0,0,0x0A,0,0x11,0x0E,0,0},
		{0x02,0x05,0x02,0,0,0,0,0},
		{0x04,0x04,0x04,0x14,0x1F,0x1F,0x0E,0x04},
		{0x11,0x0A,0x11,0x04,0x00,0x0E,0x11}
};
#endif
  // remove this line if you not using battery symbols
#ifdef battery
uint8 battary[7][8] = { // up to 8 characters
		{0x0E,0x1B,0x11,0x11,0x11,0x11,0x11,0x1F},
		{0x0E,0x1B,0x11,0x11,0x11,0x11,0x1F,0x1F},
		{0x0E,0x1B,0x11,0x11,0x11,0x1F,0x1F,0x1F},
		{0x0E,0x1B,0x11,0x11,0x1F,0x1F,0x1F,0x1F},
		{0x0E,0x1B,0x11,0x1F,0x1F,0x1F,0x1F,0x1F},
		{0x0E,0x1B,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F},
		{0x0E,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F},
};
#endif
*/
#endif /* LCD_H_ */
