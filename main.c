/*
* This Code is written to Developed to Extract Temperature and Humidity 
* Data from the DHT11 Sensor using the one-wire Communication protocol. 
* And Displays the Extracted Data on a 20x4 LCD Display. 
* The Project uses an external XTAL of 4MHZ. 
* 
* The Timing Parameters should not be tampered with!!
*
* The project works perfectly both on Simmulation and Hardware. 
* Though it sometimes act erratically on simmulation. 
*
* Author: ABEL ASHENAFI
* Date: August 28, 2019 
*/

#include "prototyper.h"

#pragma config OSC = HS
#pragma config LVP = OFF
#pragma config WDT = OFF

void main (void)
{	
	unsigned char RH_Integral;
	unsigned char RH_Decimal; 
	
	unsigned char Temp_Integral; 
	unsigned char Temp_Decimal;
	
	unsigned char Checksum; 
	
	unsigned char DataValid; 
		
	Port_Init (); 
//	SerialInit (); 
	
	LCD_Init ();
	LCD_Stuff (); 
	
	while (1)
	{		 
		DHT11_Start (); 		
		if (DHT11_Response ())
		{
			RH_Integral = DHT11_Read (); 
			RH_Decimal  = DHT11_Read ();

					
			Temp_Integral = DHT11_Read ();
			Temp_Decimal  = DHT11_Read ();
			
			Checksum = DHT11_Read ();
			
			DataValid = Check_Parity (Checksum, RH_Integral, RH_Decimal, Temp_Integral, Temp_Decimal); 
			
			if (DataValid)
			{
				DisplayTemp (Temp_Integral, Temp_Decimal);
				DisplayHumidity (RH_Integral, RH_Decimal);	
				Delay10KTCYx (120);				// A minimum Sample Period of 1.2 seconds @4MHZ
			}
			
			else 
			{
				Parity_Error (); 		
			}		
		} 
		
		else 
		{
			Sensor_Unresponsive (); 
		}		
	} 
}

