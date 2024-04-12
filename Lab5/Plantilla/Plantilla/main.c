/*
 * Lab5.c
 *
 * Created: 11/04/2024 09:07:45
 * Author : Josue Ortiz
 */ 
/*
	------------------------------------------------------
	DEFINICIONES, INCLUDES Y VARIABLES
	------------------------------------------------------
*/
#define F_CPU 16000000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include "Setups/SetupsMicro.h"
uint8_t ValorADC = 0;
/*
	------------------------------------------------------
	DEFINICION DE FUNCIONES
	------------------------------------------------------
*/
	void setup(void);
/*
	------------------------------------------------------
	MAIN-LOOP
	------------------------------------------------------
*/

int main(void)
{
    setup();
    while (1) 
    {
		OCR1A =  ValorADC/6;
    }
}

/*
	------------------------------------------------------
	PROGRAMACION DE FUNCIONES
	------------------------------------------------------
*/
	void setup(void){
	cli();
	SetupADC(PC6);
	SetupTimer0(1024,255);
	SetupPWM1(Fast8,Positivo,A,1024);	
	sei();
	}
/*
	------------------------------------------------------
	INTERRUPCIONES
	------------------------------------------------------
*/
ISR(TIMER0_OVF_vect){
	ADCSRA |= (1<<ADSC);
	TCNT0 = 255;
	TIFR0 |= (1 << TOV0);
}

ISR(ADC_vect){
	ValorADC = ADCH;
	ADCSRA |= (1<<ADIF);
}