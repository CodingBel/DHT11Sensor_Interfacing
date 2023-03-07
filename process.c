#include "prototyper.h"

void DelayFor18TCY (void)		
{
	Delay10TCYx (2);		        // a delay of 20 TCY
}

void DelayPORXLCD (void) 		
{
	Delay1KTCYx (15);     			// a delay of 15ms at 4 MHZ
}

void DelayXLCD (void) 			
{
	Delay1KTCYx (5);				// a delay of 5ms at 4 MHZ  
}


void serialTx (unsigned char rx_data)
{
	TXREG = rx_data;
	Delay10TCY();	
}

void DHT11_Start (void)
{
	Data_Dxn = 0; 				// DataPin is set as an Output pin 
	
	Data_Out = 0; 				// Pull the DataBus line Low 
	Delay1KTCYx (20); 			// A Delay of At least 18ms @4MHZ 
		
	Data_Out = 1; 
	Delay10TCYx (3);			// A Delay of atleast 30us @4MHZ
	 	
	Data_Dxn = 1; 				// DataPin set as an Input pin
}

short int  DHT11_Response (void)
{	
	Delay10TCYx (4);			// A Delay of 40us @ 4MHZ
	
	if (Data_In == 0)			// If Data line is low 
	{
		Delay10TCYx (8);		// A Delay of 80us @ 4MHZ
		
		if (Data_In == 1)		// If Data Line is High 
		{
			Delay10TCYx (5);	// A Delay of 50us @ 4MHZ
			return 1; 
		}
	}	
}

unsigned char DHT11_Read (void)
{
	unsigned char i; 
	unsigned char data = 0x00; 
	unsigned char Time = 0; 
	
	for (i = 0; i < 8; i++)
	{
		while (!Data_In)			// Wait Till Line goes High
		{
			Time++; 
			if (Time > 100)
			{	
				break; 
			}
			
			Delay1TCY (); 			// A Delay of 1us @ 4MHZ
		}
		
		Delay10TCYx (3); 			// Wait for atleast 30us to check if bit is either 1 or 0
		
		if (!Data_In)				// Bit is 0				
		{
			data = (data << 1) | 0; 
		}
		
		else 
		{
			data = (data << 1) | 1; // Bit is 1
			
			Time = 0; 
			while (Data_In)			// Wait till Databus goes Low 
			{
				Time++; 
				if (Time > 100)
				{
					break; 
				}
				
				Delay1TCY (); 
			}		
		}
	}
	
	return data; 
}

unsigned char Check_Parity (unsigned char parity, unsigned char RHI, unsigned char RHD, unsigned char TI, unsigned char TD)
{	
	if (parity == (RHI + RHD + TI + TD))
	{
		return 1; 
	}
	else 
	{
		return 0; 
	}
}


void Humidity_Serial (unsigned char Int_Part, unsigned char Float_Part)
{
	unsigned char Tens; 
	unsigned char Ones; 
	unsigned char Dec; 
	
	Tens = Int_Part / 10; 
	Ones = Int_Part % 10;
	Dec = Float_Part;  
	
	Tens += 48;				// Convet To ASCII Counterparts
	Ones += 48;  			//
	Dec  += 48; 			// 
	
	serialTx ('H');
	Delay1KTCYx (2);
	serialTx ('\r');
	
	serialTx (Tens);
	Delay1KTCYx (2);
	serialTx (Ones);
	Delay1KTCYx (2);
	serialTx ('.');
	Delay1KTCYx (2);
	serialTx (Dec);
	serialTx ('\r');
	Delay1KTCYx (2);
	serialTx ('\r');
	Delay1KTCYx (2);
}

void Temp_Serial (unsigned char Int_Part, unsigned char Float_Part)
{
	unsigned char Tens; 
	unsigned char Ones; 
	unsigned char Dec; 
	
	Tens = Int_Part / 10; 
	Ones = Int_Part % 10;
	Dec = Float_Part;  
	
	Tens += 48;				// Convet To ASCII Counterparts
	Ones += 48;  			//
	Dec  += 48; 			// 
	
	serialTx ('T');
	Delay1KTCYx (2);
	serialTx ('\r');
	
	serialTx (Tens);
	Delay1KTCYx (2);
	serialTx (Ones);
	Delay1KTCYx (2);
	serialTx ('.');
	Delay1KTCYx (2);
	serialTx (Dec);
	serialTx ('\r');
	Delay1KTCYx (2);
	serialTx ('\r');
	Delay1KTCYx (2);
}

void ParityError (void)
{
	unsigned char i = 0; 
	unsigned char Parity [] = "Parity Error";
	
	while (Parity [i] != '\0')
	{
		serialTx (Parity [i]);
		i++; 
		Delay1KTCYx (2);
	}
	
	serialTx ('\r');
	Delay1KTCYx (2);
	serialTx ('\r');
	Delay1KTCYx (2);
	
	Delay10KTCYx (50);
}

void SensorUnresponsive (void)
{
	unsigned char i = 0; 
	unsigned char Response [] = "Sensor Unresponsive"; 
	
	while (Response [i] != '\0')
	{
		serialTx (Response [i]);
		i++; 
		Delay1KTCYx (2);
	}
	
	serialTx ('\r');
	Delay1KTCYx (2);
	serialTx ('\r');
	Delay1KTCYx (2);
	
	Delay10KTCYx (50);
}

void DisplayTemp (unsigned char Int_Part , unsigned char Float_Part)
{
	unsigned char Tens; 
	unsigned char Ones; 
	unsigned char Dec; 
	
	Tens = Int_Part / 10; 
	Ones = Int_Part % 10;
	Dec = Float_Part;
	
	while (BusyXLCD());
	WriteCmdXLCD (0xC7);
	
	if (Tens != 0)
	{
		while (BusyXLCD());	
		WriteDataXLCD (Tens + 48);
	}
	
	else 
	{
		while (BusyXLCD());
		putrsXLCD (" "); 		// Write nothing 	
	}
	
	while (BusyXLCD());	
	WriteDataXLCD (Ones + 48);
	
	while (BusyXLCD());
	WriteCmdXLCD (0xCA);
	
	while (BusyXLCD());	
	WriteDataXLCD (Dec + 48);	
}

void DisplayHumidity (unsigned char Int_Part , unsigned char Float_Part)
{
	unsigned char Tens; 
	unsigned char Ones; 
	unsigned char Dec; 
	
	Tens = Int_Part / 10; 
	Ones = Int_Part % 10;
	Dec = Float_Part;
	
	while (BusyXLCD());
	WriteCmdXLCD (0x9B);
	

	while (BusyXLCD());	
	WriteDataXLCD (Tens + 48);
		
	while (BusyXLCD());	
	WriteDataXLCD (Ones + 48);
	
	while (BusyXLCD());
	WriteCmdXLCD (0x9E);
	
	while (BusyXLCD());	
	WriteDataXLCD (Dec + 48);
}

void Parity_Error (void)
{
	while (BusyXLCD());
	WriteCmdXLCD (CLRSCR); 		// Clear Screen 
	
	while (BusyXLCD());
	putrsXLCD ("Parity Error!!");
	
	while (BusyXLCD());
	WriteCmdXLCD (CLRSCR); 		// Clear Screen 
}

void Sensor_Unresponsive (void)
{	
	while (BusyXLCD());
	WriteCmdXLCD (CLRSCR); 		// Clear Screen 
	
	while (BusyXLCD());
	putrsXLCD ("Sensor Unresponsive!");
	
	while (BusyXLCD());
	WriteCmdXLCD (CLRSCR); 		// Clear Screen 
}


