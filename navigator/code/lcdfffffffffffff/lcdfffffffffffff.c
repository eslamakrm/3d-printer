/*
 * lcdfffffffffffff.c
 *
 * Created: 2/28/2017 10:03:11 AM
 *  Author: Lenovo
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include "lcd.h"
unsigned char *files[6];
 int selector=0;
 int true_selector=0;
 int a=0;
number_of_files = sizeof(files)/sizeof(files[0]);

void INT0_Init(void)
{
	SREG  &= ~(1<<7);                   // Disable interrupts by clearing I-bit
	DDRD  &= (~(1<<PD2));               // Configure INT0/PD2 as input pin
	PORTD |= (1<<PD2);                  // Enable the internal pull up resistor at PD2 pin
	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
	MCUCR |= (1<<ISC00) | (1<<ISC01);   // Trigger INT0 with the raising edge
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}

void INT1_Init(void)
{
	SREG  &= ~(1<<7);      // Disable interrupts by clearing I-bit
	DDRD  &= (~(1<<PD3));  // Configure INT1/PD3 as input pin
	PORTD |= (1<<PD3);     // Enable the internal pull up resistor at PD3 pin
	GICR  |= (1<<INT1);    // Enable external interrupt pin INT1
	// Trigger INT1 with the falling edge
	MCUCR |= (1<<ISC11);
	MCUCR &= ~(1<<ISC10);
	SREG  |= (1<<7);       // Enable interrupts by setting I-bit
}

ISR(INT1_vect)
{
	if(true_selector<number_of_files-1){++true_selector;++selector;}//navigate down list
		if(selector>3){selector=3; a++;}
		
}

ISR(INT0_vect)
{
	 if(true_selector>0){--true_selector;--selector;}//navigate up list
		if(selector<0){selector=0;a--;}
}

int select_file()
{
	while(1)
	{
		if(BIT_IS_CLEAR(PINC,0))
		return true_selector;
		
		if(a<0)a=0;
		
		for(int i=0;i<4;i++){
			if(i==selector){
				LCD_goToRowColumn(selector,0);
				LCD_displayCharacter('>');
			               }
		            	   else{
				                LCD_goToRowColumn(i,0);
				                LCD_displayCharacter(' ');
			                    }
			   LCD_displayStringRowColumn(i,1,files[i+a]);
		                   }	
	}
	return true_selector;
}



int main(void)
{
	int selected_file;
	CLEAR_BIT(DDRC,0); //input  for select button
	PORTC |= (1<<0); //pull up resistor
	
	LCD_init();
	_delay_ms(10);
	
	files[0] = "file0";
files[1] = "file1";
files[2] = "file2";
files[3] = "file3";
files[4] = "file4";
files[5] = "file5";

		
		INT0_Init();
		_delay_ms(10);
		INT1_Init();
   selected_file = select_file();
   LCD_sendCommand(CLEAR_COMMAND);
   LCD_displayString(files[selected_file]);
}