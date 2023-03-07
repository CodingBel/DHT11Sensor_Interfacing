#include "prototyper.h"

void Port_Init (void)
{
	TRISCbits.TRISC2 = 0; 			// Pin Used to Check Pwr on Ckt
	LATCbits.LATC2   = 1; 			//
	
	TRISD = 0x00;					// Port used for LCD
	LATD  = 0x00; 
}

void LCD_Init (void)
{
	OpenXLCD (FOUR_BIT & LINES_5X7);
	while (BusyXLCD()); 
	WriteCmdXLCD (DISPLAY_ON);  		 // TURN DISPLAY ON cursor off
	while (BusyXLCD()); 									
	WriteCmdXLCD (0x80);				 // begin at the first line 
	while (BusyXLCD()); 
	WriteCmdXLCD (CLRSCR);	 			 // clear the lcd screen
}

void LCD_Stuff (void)
{
	while (BusyXLCD());
	putrsXLCD ("DHT11 Interfacing");
	
	while (BusyXLCD());
	WriteCmdXLCD (0xC0);				
	while (BusyXLCD());
	putrsXLCD ("Temp = "); 
	
	while (BusyXLCD());
	WriteCmdXLCD (0xC9);
	while (BusyXLCD());
	putrsXLCD (".");
	
	while (BusyXLCD());
	WriteCmdXLCD (0xCC);
	while (BusyXLCD());
	putrsXLCD ("deg"); 
	
	
	while (BusyXLCD());
	WriteCmdXLCD (0x94);				
	while (BusyXLCD());
	putrsXLCD ("Humi = "); 
	
	while (BusyXLCD());
	WriteCmdXLCD (0x9D);
	while (BusyXLCD());
	putrsXLCD (".");
	
	while (BusyXLCD());
	WriteCmdXLCD (0xA0);
	while (BusyXLCD());
	putrsXLCD ("%");
}

void SerialInit (void)
{
   RCSTAbits.SPEN 	 = 0;	// disable serial port before configuring other parameters 
   TXSTAbits.SYNC    = 0;	// enable usart in asynchronous mode 
   
   			/******************* BaudRater Generation *************************/
   BAUDCONbits.BRG16 = 0;	// use the 8 bit spbrg protocol 
   TXSTAbits.BRGH    = 1;  	// use High speed 
   SPBRG = 0x19;			// to get a baud rate of 9600 bps @4MHZ
    
   RCSTAbits.SPEN 	 = 1;	// enable serial port 
   TXSTAbits.TXEN    = 1;	// enable usart transmit	
}




