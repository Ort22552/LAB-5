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
uint8_t ValorADC1 = 0;
uint8_t ValorADC2 = 0;
uint8_t ValorADC3 = 0;  
uint8_t PWMFALSO = 0;
uint8_t Turno = 0;
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

		PWMFALSO++; 
		OCR1A =  ValorADC1/6;
		OCR2A= ValorADC2/6;
		if (PWMFALSO == 0)
		{
			PORTD |= (1<<PORTD4);
		}
		else if (PWMFALSO == (ValorADC3))
		{
			PORTD &= ~(1<<PORTD4);
		}
    }
}

/*
	------------------------------------------------------
	PROGRAMACION DE FUNCIONES
	------------------------------------------------------
*/
	void setup(void){
	cli();
	SetupADC(PC1);
	SetupADC(PC6);
	SetupADC(PC0);
	SetupTimer0(1024,255);
	SetupPWM1(Fast8,Positivo,A,1024);	
	SetupPWM2(Fast8,Positivo,A,1024);
	SetupSalida(D, PD4);
	sei();
	}
/*
	------------------------------------------------------
	INTERRUPCIONES
	------------------------------------------------------
*/
ISR(TIMER0_OVF_vect){
	switch (Turno)
	{
	case 0:
		Turno=1;
		ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
		break;
	case 1:
		Turno=2;
		ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
		ADMUX |= (1<<MUX2)|(1<<MUX1);
		break;
	case 2:
		Turno=0;
		ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));
		ADMUX|=(1<<MUX0);
		break;
	}
	ADCSRA |= (1<<ADSC);
	TCNT0 = 255;
	TIFR0 |= (1 << TOV0);
}

ISR(ADC_vect){
	switch (Turno)
	{
	case 0:
		ValorADC1 = ADCH;
		break;
	case 1:
		ValorADC2 = ADCH;
		break;
	case 2:
		ValorADC3 = ADCH;
		break;
	}
	ADCSRA |= (1<<ADIF);
}