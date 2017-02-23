/*
Serial to parallel on teensy
*/
#include "Arduino.h"
#ifdef BOARD_TEENSY
#include <usb_serial.h>
#endif

unsigned long time ;
char buffer [255];

#ifdef BOARD_TEENSY
#define OFFSET_PIN  0
#else
#define OFFSET_PIN  2
#endif


void usage ()
{

	Serial.println ("# (c) BARTHELEMY STEPHANE 02/2017");
	Serial.println ("#   Convertor serial parallel ");
	Serial.println ("# formats : ");
	Serial.println ("# Decimal     : aaa       : 170  ");
	Serial.println ("# Hexadecimal : xaa       : xAA  ");
	Serial.println ("# binary      : baaaaaaaa : b10101010  ");

}

void manage_buffer ()
{
	unsigned char value = 0 ;
	boolean ok = false ;
//	Serial.println ("buffer : ");
//	Serial.println (buffer);
	if ((buffer[0] >= '0') && (buffer[0] <= '9'))
	{
//		Serial.println ("Decimal : ");
		ok = true;
		for ( unsigned int i = 0 ; i < strlen (buffer ) ; i ++ )
		{
//			Serial.print ("i         : ");Serial.println (i);
//			Serial.print ("ok        : ");Serial.println (ok);
//			Serial.print ("buffer[i] : " );Serial.println (buffer[i],DEC);
//			Serial.print ("value     : ");Serial.println (value);
			if ((buffer[i] >= '0') && (buffer[i] <= '9')&& (ok == true))
			{
				ok = true;
				value = ( value *10 ) + (buffer[i] - '0') ;
			}
			else
			{
				ok = false ;
			}
		}

	} else if ((buffer[0] == 'X') || (buffer[0] == 'x'))
	{
		ok = true;
		for ( unsigned int i = 1 ; i < strlen (buffer ) ; i ++ )
		{

//			Serial.print ("i         : ");Serial.println (i);
//			Serial.print ("ok        : ");Serial.println (ok);
//			Serial.print ("buffer[i] : " );Serial.println (buffer[i],DEC);
//			Serial.print ("value     : ");Serial.println (value);
			if ((buffer[i] >= '0') && (buffer[i] <= '9')&& (ok == true))
			{
				value = ( value *16 ) + (buffer[i] - '0') ;
			}
			else if ((buffer[i] >= 'a') && (buffer[i] <= 'f')&& (ok == true))
			{
				value = ( value *16 ) + (buffer[i] - 'a' + 10) ;
			}
			else if ((buffer[i] >= 'A') && (buffer[i] <= 'F')&& (ok == true))
			{
				value = ( value *16 ) + (buffer[i] - 'A' + 10) ;
			}
			else
			{
				ok = false ;
			}

		}
	} else if ((buffer[0] == 'B') || (buffer[0] == 'b'))
	{
		ok = true;
		for ( unsigned int i = 1 ; i < strlen (buffer ) ; i ++ )
		{
			if ((buffer[i] >= '0') && (buffer[i] <= '1')&& (ok == true))
			{
				value = ( value *2 ) + (buffer[i] - '0') ;
			}
			else
			{
				ok = false ;
			}

		}
	}
	if (ok == true )
	{
		Serial.print   ("Set Value : ");
		Serial.print   ( value , DEC);
		Serial.print   (" ,  x");
		Serial.print   ( value , HEX);
		Serial.print   (" ,  b");
		Serial.println ( value , BIN);

		for (int i = 0 ; i < 8 ; i ++ )
		{
			if ((value & (1 << i )) != 0 )
			{
		                //Serial.print   ("Set high : "); Serial.println (i) ;
				digitalWrite ( i  + OFFSET_PIN, HIGH);

			} else {
		                //Serial.print   ("Set low : "); Serial.println (i) ;
				digitalWrite ( i + OFFSET_PIN , LOW);
			}
		}
		digitalWrite(LED_BUILTIN,HIGH);
		delay (500);
		digitalWrite(LED_BUILTIN,LOW);

	} else
	{
		Serial.println ("Unknown value !");
		usage();
	}
	strcpy(buffer , "");
}

void setup ()
{
	for (int i = 0 ; i < 8 ; i ++ )
	{
		pinMode(i + OFFSET_PIN, OUTPUT);
		digitalWrite ( i + OFFSET_PIN , LOW);
  	}
	pinMode(LED_BUILTIN, OUTPUT);
	time = millis ();

	Serial.begin(115200);
	strcpy (buffer , "");
	usage ();
}

void loop ()
{
	if (Serial.available()) {
		char sztmp[2] ;
		sztmp[0] = Serial.read();  // will not be -1
		Serial.print(sztmp[0]);
		sztmp[1]= 0;
		if (( sztmp[0] == 0X0A) || ( sztmp[0] == 0x0D))
		{
			manage_buffer ();
		} else {
			if (strlen (buffer) < 254 ) strcat (buffer , sztmp);
		}
	}

}
