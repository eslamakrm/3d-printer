//***********************************************************
// **** MAIN routine FOR Interfacing microSD/SDHC CARD ****
//***********************************************************
//Controller: ATmega32 (Clock: 8 Mhz-internal)
//Compiler	: AVR-GCC (winAVR with AVRStudio)
//Version 	: 2.3
//Author	: CC Dharmani, Chennai (India)
//			  www.dharmanitech.com
//Date		: 08 May 2010
//***********************************************************

//Link to the Post: http://www.dharmanitech.com/2009/01/sd-card-interfacing-with-atmega8-fat32.html

#define F_CPU 8000000L
	//freq 8 MHz
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "SPI_routines.h"
#include "SD_routines.h"

//#include "UART_routines.h"
#include "FAT32.h"
#include "lcd.h"

 int selector=0;
 int true_selector=0;
 int a=0;
 number_of_files = sizeof(files)/sizeof(files[0]);

int select_file();
void INT0_Init();
void INT1_Init();


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




void port_init(void)
{

PORTB = 0xEF;
SET_BIT(DDRB,4); //MISO line i/p, rest o/p
SET_BIT(DDRB,5);
SET_BIT(DDRB,7);
CLEAR_BIT(DDRB,6);
   
CLEAR_BIT(DDRC,0); //input  for select button
PORTC |= (1<<0); //pull up resistor

}


//call this routine to initialize all peripherals
void init_devices(void)
{
 cli();  //all interrupts disabled
  spi_init();
  _delay_ms(10);
 port_init();
_delay_ms(10);
 LCD_init();

INT0_Init();
_delay_ms(10);
INT1_Init();

// uart0_init();

_delay_ms(10);
 MCUCR = 0x00;
 GICR  = 0x00;
 TIMSK = 0x00; //timer interrupt sources
 //all peripherals are now initialized
}

//*************************** MAIN *******************************//
int main(void)
{
unsigned char error;
unsigned char FAT32_active;
unsigned int i,selector=0;
int selected_file;
//unsigned char fileName[13];

//_delay_ms(100);  //delay for VCC stabilization
 
init_devices();






//SET_BIT(PORTD,4); //switching ON the LED (for testing purpose only)
//_delay_ms(2000);
//TX_NEWLINE;
//TX_NEWLINE;
//transmitString_F (PSTR("*********************************************"));
//TX_NEWLINE;
//transmitString_F (PSTR("    Dharmani's microSD Card Testing..  "));
//TX_NEWLINE;
//transmitString_F (PSTR("*********************************************"));
//TX_NEWLINE;
// 
  cardType = 0;
  LCD_sendCommand(CLEAR_COMMAND);
 	 LCD_displayString("init");
 
  for (i=0; i<10; i++)
  {
 	LCD_displayCharacter('.');
    error = SD_init();
    if(!error) break;
  }
  
  if(error)
  {
 	 LCD_sendCommand(CLEAR_COMMAND);
    if(error == 1) LCD_displayString("sd not detected.."); // transmitString_F(PSTR("SD card not detected.."));
    if(error == 2) LCD_displayString("Init failed..");//transmitString_F(PSTR("Card Initialization failed.."));
  
    while(1);  //wait here forever if error in SD init 
  }
  LCD_sendCommand(CLEAR_COMMAND);
  LCD_displayString("no error ");
 // _delay_ms(1000);
// CLEAR_BIT(PORTD,4);
 
switch (cardType)
 {
   case 1://transmitString_F(PSTR("Standard Capacity Card (Ver 1.x) Detected!"));
          LCD_displayString("Ver 1.x");
   		 break;
   case 2://transmitString_F(PSTR("High Capacity Card Detected!"));
          LCD_displayString("High Capacity Card Detected!");
   		 break;
   case 3://transmitString_F(PSTR("Standard Capacity Card (Ver 2.x) Detected!"));
       LCD_displayString("Ver 2.x");
   		 break;
   default://transmitString_F(PSTR("Unknown SD Card Detected!"));
       LCD_displayString("Unknown card!");
   		 break; 
 }
 
 
 SPI_HIGH_SPEED;	//SCK - 4 MHz
 _delay_ms(1);   //some delay
 
 
 FAT32_active = 1;
 error = getBootSectorData (); //read boot sector and keep necessary data in global variables
 if(error) 	
 {
//   //TX_NEWLINE;
//  // transmitString_F (PSTR("FAT32 not found!"));  //FAT32 incompatible drive
  LCD_sendCommand(CLEAR_COMMAND);
  LCD_displayString("FAT32 not found!");
   FAT32_active = 0;
 }
 LCD_displayStringRowColumn(1,0,"finish");

//while(1)
//{
// 	TX_NEWLINE;
// 	transmitString_F(PSTR("Press any key..."));
// 	TX_NEWLINE;
// 	option = receiveByte();
// 	TX_NEWLINE;
// 	transmitString_F(PSTR("> 0 : Erase Blocks"));
// 	TX_NEWLINE;
// 	transmitString_F(PSTR("> 1 : Write single Block"));
// 	TX_NEWLINE;
// 	transmitString_F(PSTR("> 2 : Read single Block"));
// 
// 	#ifndef FAT_TESTING_ONLY
// 	TX_NEWLINE;
// 	transmitString_F(PSTR("> 3 : Write multiple Blocks"));
// 	TX_NEWLINE;
// 	transmitString_F(PSTR("> 4 : Read multiple Blocks"));
// 	#endif
// 
// 
// 	TX_NEWLINE;
// 	transmitString_F(PSTR("> 5 : Get file list"));
// 	TX_NEWLINE;
// 	transmitString_F(PSTR("> 6 : Read File"));
// 	TX_NEWLINE;
// 	transmitString_F(PSTR("> 7 : Write File"));
// 	TX_NEWLINE;
// 	transmitString_F(PSTR("> 8 : Delete File"));
// 	TX_NEWLINE;
// 	transmitString_F(PSTR("> 9 : Read SD Memory Capacity (Total/Free)"));
// 
// 	TX_NEWLINE;
// 	TX_NEWLINE;
// 	transmitString_F(PSTR("> Select Option (0-9): "));
// 
// 
// 	/*WARNING: If option 0, 1 or 3 is selected, the card may not be detected by PC/Laptop again,
// 	as it disturbs the FAT format, and you may have to format it again with FAT32.
// 	This options are given for learnig the raw data transfer to & from the SD Card*/
// 
// 	option = receiveByte();
// 	transmitByte(option);
// option = 5; //get files list
// 	if(option >=0x35 && option <=0x39)  //options 5 to 9 disabled if FAT32 not found
// 	{
// 		if(!FAT32_active)
// 		{
// 		//	TX_NEWLINE;
// 			//TX_NEWLINE;
// 			//transmitString_F(PSTR("FAT32 options disabled!"));
// 			continue;
// 		}
// 	}
// 
// 
// 	if((option >= 0x30) && (option <=0x34)) //get starting block address for options 0 to 4
// 	{
// // 	TX_NEWLINE;
// // // 	TX_NEWLINE;
// // 		transmitString_F(PSTR("Enter the Block number (0000-9999):"));
//  		data = receiveByte(); transmitByte(data);
// 		startBlock = (data & 0x0f) * 1000;
// 		data = receiveByte(); transmitByte(data);
// 		startBlock += (data & 0x0f) * 100;
// 		data = receiveByte(); transmitByte(data);
// 		startBlock += (data & 0x0f) * 10;
// 		data = receiveByte(); transmitByte(data);
// 		startBlock += (data & 0x0f);
// //		TX_NEWLINE;
// 	}
// 
// 	totalBlocks = 1;
// 
// 	#ifndef FAT_TESTING_ONLY
// 
// 	if((option == 0x30) || (option == 0x33) || (option == 0x34)) //get total number of blocks for options 0, 3 or 4
// 	{
// // 		TX_NEWLINE;
// // 		TX_NEWLINE;
// // 		transmitString_F(PSTR("How many blocks? (000-999):"));
// 		data = receiveByte(); transmitByte(data);
// 		totalBlocks = (data & 0x0f) * 100;
// 		data = receiveByte(); transmitByte(data);
// 		totalBlocks += (data & 0x0f) * 10;
// 		data = receiveByte(); transmitByte(data);
// 		totalBlocks += (data & 0x0f);
// //		TX_NEWLINE;
// 	}
// 	#endif

// 	switch (option)
// 	{
// 		case '0': //error = SD_erase (block, totalBlocks);
// 		error = SD_erase (startBlock, totalBlocks);
// 	//	TX_NEWLINE;
// 		if(error)
// 		LCD_displayString("Erase failed..");
// 		//transmitString_F(PSTR("Erase failed.."));
// 		else
// 		LCD_displayString("Erased");
// 		//transmitString_F(PSTR("Erased!"));
// 		break;
// 
// 		case '1': //TX_NEWLINE;
// 		//transmitString_F(PSTR(" Enter text (End with ~):"));
// 		LCD_displayString(" Enter text (End with ~):");
// 		i=0;
// 		do
// 		{
// 			data = receiveByte();
// 			transmitByte(data);
// 			buffer[i++] = data;
// 			if(data == 0x0d)
// 			{
// 				transmitByte(0x0a);
// 				buffer[i++] = 0x0a;
// 			}
// 			if(i == 512) break;
// 		}while (data != '~');
// 
// 		error = SD_writeSingleBlock (startBlock);
// // 		TX_NEWLINE;
// // 		TX_NEWLINE;
// 		if(error)
// 		LCD_displayString("Write failed!");
// 		//transmitString_F(PSTR("Write failed.."));
// 		else
// 		LCD_displayString("Write successful!");
// 		//transmitString_F(PSTR("Write successful!"));
// 		break;
// 
// 		case '2': error = SD_readSingleBlock (startBlock);
// 		//TX_NEWLINE;
// 		if(error)
// 		LCD_displayString("read failed!");
// 		//transmitString_F(PSTR("Read failed.."));
// 		else
// 		{
// 			for(i=0;i<512;i++)
// 			{
// 				if(buffer[i] == '~') break;
// 				transmitByte(buffer[i]);
// 			}
// // 			TX_NEWLINE;
// // 			TX_NEWLINE;
// // 			transmitString_F(PSTR("Read successful!"));
// LCD_displayString("read successful!");
// 		}
// 
// 		break;
// 		//next two options will work only if following macro is cleared from SD_routines.h
// 		#ifndef FAT_TESTING_ONLY
// 
// 		case '3':
// 		error = SD_writeMultipleBlock (startBlock, totalBlocks);
// 		//TX_NEWLINE;
// 		if(error)
// 		//transmitString_F(PSTR("Write failed.."));
// 		LCD_displayString("Write failed!");
// 		else
// 		LCD_displayString("Write successful!");
// 		//transmitString_F(PSTR("Write successful!"));
// 		break;
// 
// 		case '4': error = SD_readMultipleBlock (startBlock, totalBlocks);
// 		//TX_NEWLINE;
// 		if(error)
// 		//transmitString_F(PSTR("Read failed.."));
// 		LCD_displayString("read failed!");
// 		else
// 		LCD_displayString("read successful!");
// 		//transmitString_F(PSTR("Read successful!"));
// 		break;
// 		#endif

		//case '5':// TX_NEWLINE;
		
 		 LCD_sendCommand(CLEAR_COMMAND);
		findFiles(GET_LIST,0);
// 		_delay_ms(5000);
 		LCD_sendCommand(CLEAR_COMMAND);
       // LCD_goToRowColumn(selector,0);
		selected_file = select_file();
		LCD_sendCommand(CLEAR_COMMAND);
		LCD_displayString(files[selected_file]);
		_delay_ms(3000);
		readFile(READ,files[selected_file]);

	//	break;
// 
// 		case '6':
// 		case '7':
// 		case '8': //TX_NEWLINE;
// 		//TX_NEWLINE;
// 		LCD_displayString("Enter file name: ");
// 		//transmitString_F(PSTR("Enter file name: "));
// 		for(i=0; i<13; i++)
// 		fileName[i] = 0x00;   //clearing any previously stored file name
// 		i=0;
// 		while(1)
// 		{
// 			data = receiveByte();
// 			if(data == 0x0d) break;  //'ENTER' key pressed
// 			if(data == 0x08)	//'Back Space' key pressed
// 			{
// 				if(i != 0)
// 				{
// 				//	transmitByte(data);
// 				//	transmitByte(' ');
// 				//	transmitByte(data);
// 					i--;
// 				}
// 				continue;
// 			}
// 			if(data <0x20 || data > 0x7e) continue;  //check for valid English text character
// 			transmitByte(data);
// 			fileName[i++] = data;
// 			if(i==13){//transmitString_F(PSTR(" file name too long..")); 
// 				
// 				break;
// 			}
// 		}
// 		if(i>12) break;
//    fileName[0] = 'b';
//    fileName[1] = 'b';
//    fileName[2] = '.';
//    fileName[3] = 't';
//    fileName[4] = 'x';
//    fileName[5] = 't';
// 	//	TX_NEWLINE;
// 		if(option == '6')
	//	readFile( READ, fileName);
// 		if(option == '7')
// 		writeFile(fileName);
// 		if(option == '8')
// 		deleteFile(fileName);
// 		break;
// 
// 		case '9': memoryStatistics();
// 		break;
// 
// 		//default:// TX_NEWLINE;
// 	//	TX_NEWLINE;
// 	//	transmitString_F(PSTR(" Invalid option!"));
// 	//LCD_displayString("Invalid option!");
// 	//	TX_NEWLINE;
// 	}
// 
// 	//TX_NEWLINE;
//}
return 0;
}
//********** END *********** www.dharmanitech.com *************

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