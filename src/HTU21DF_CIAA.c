/*
 * HTU21DF_CIAA.c
 *
 *  Created on: Aug 2, 2017
 *      Author: Carlos Miguens
 *
 *  Driver for HTU21D-F humidity and temperature sensor for EDU-CIAA NXP
 *
 */


#include "sapi.h"     // <= Biblioteca sAPI
#include "board.h"
#include "HTU21DF.h"
#include "stdio.h"

void initHardware(void);

uartMap_t MyUART = UART_USB;
int UART_SPEED = 115200;

void initHardware(void)
{
   // Read clock settings and update SystemCoreClock variable
   SystemCoreClockUpdate();
   // Set up and initialize all required blocks and
   // functions related to the board hardware
   Board_Init();
   // Set the LED to the state of "Off"
   Board_LED_Set(0, 0);

   uartConfig( MyUART, UART_SPEED );
}

int n_tu(int number, int count)
{
    int result=1;
    while(count-- > 0)
    result *= number;

    return result;
}

/***Convert float to string***/
void float_to_string(float f, char r[])
{
    long long int length, length2, i, number, position, sign;
    float number2;

    sign = -1;   // -1 == positive number
    if (f < 0)
    {
        sign = '-';
        f *= -1;
    }


    number2 = f;
    number = f;
    length = 0;  // size of decimal part
    length2 = 0; //  size of tenth

    /* calculate length2 tenth part*/
    while( (number2 - (float)number) != 0.0 && !((number2 - (float)number) < 0.0) )
    {
         number2 = f * (n_tu(10.0, length2 + 1));
         number = number2;

         length2++;
    }

    /* calculate length decimal part*/
    for(length = (f > 1) ? 0 : 1; f > 1; length++)
        f /= 10;

    position = length;
    length = length + 1 + length2;
    number = number2;
    if(sign == '-')
    {
        length++;
        position++;
    }

    for(i = length; i >= 0 ; i--)
    {
        if(i == (length))
            r[i] = '\0';
        else if(i == (position))
            r[i] = '.';
        else if(sign == '-' && i == 0)
            r[i] = '-';
        else
        {
            r[i] = (number % 10) + '0';
            number /=10;
        }
    }
}

int main(void)
{
	int i;
	float temperature, humidity;
	char text[50];
	initHardware();
	HTU21DF_initI2C();
	HTU21DF_begin();

	while(1)
	{
		temperature = HTU21DF_readTemperature();
		humidity = HTU21DF_readHumidity();


		float_to_string(temperature, text);
		uartWriteString(MyUART, "Temperature: \0");
		uartWriteString(MyUART, text);
		uartWriteString(MyUART, "\n\r\0");
		float_to_string(humidity, text);
		uartWriteString(MyUART, "Humidity: \0");
		uartWriteString(MyUART, text);
		uartWriteString(MyUART, "\n\r\n\r\n\r\0");
	}
}


