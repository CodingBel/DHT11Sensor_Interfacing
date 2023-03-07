#include <p18f4520.h>
#include <delays.h>
#include <xlcd.h>

#define CLRSCR 		0x01
#define DISPLAY_ON 	0x0C
#define DISPLAY_OFF 0x08


#define Data_Dxn TRISCbits.TRISC1
#define Data_Out LATCbits.LATC1
#define Data_In  PORTCbits.RC1


void DHT11_Start (void);
short int DHT11_Response (void);
unsigned char DHT11_Read (void);
unsigned char Check_Parity (unsigned char, unsigned char, unsigned char, unsigned char, unsigned char); 


void Port_Init (void);
void SerialInit (void); 
void serialTx (unsigned char); 

void Temp_Serial (unsigned char, unsigned char);
void Humidity_Serial (unsigned char, unsigned char);

void ParityError (void);
void SensorUnresponsive (void);

/*************************************** LCD Functions ************************************/

void LCD_Init (void);
void LCD_Stuff (void); 

void DisplayTemp (unsigned char, unsigned char);
void DisplayHumidity (unsigned char, unsigned char);

void Parity_Error (void);
void Sensor_Unresponsive (void);  





