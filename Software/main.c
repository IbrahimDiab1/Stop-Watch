/*
 * project 2: Stop Watch
 *
 *  Created on: 15/9/2021
 *      Author: Ibrahim_Diab
 */

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

unsigned char second;
unsigned char minute;
unsigned char hour;

void INT0_IN() {
	SREG &= ~(1 << 7);
	DDRD &= (~(1 << PD2));
	PORTD |= (1 << PD2);
	GICR |= (1 << INT0);
	MCUCR |= (1 << ISC01);
	MCUCR &= ~(1 << ISC00);
	SREG |= (1 << 7);
}
void INT1_IN() {
	SREG &= ~(1 << 7);
	DDRD &= (~(1 << PD3));
	GICR |= (1 << INT1);
	MCUCR |= (1 << ISC11) | (1 << ISC10);
	SREG |= (1 << 7);
}
void INT2_IN() {
	SREG &= ~(1 << 7);
	DDRD &= (~(1 << PB2));
	PORTB |= (1 << PB2);
	GICR |= (1 << INT2);
	MCUCR &= ~(1 << ISC2);
	SREG |= (1 << 7);
}
void Timer1_IN_Cmp_Mode() {
	SREG &= ~(1 << 7);
	TCNT1 = 0;
	OCR1A = 15625;
	TIMSK |= (1 << OCIE1A);
	TCCR1A = (1 << FOC1A);  //////select 64 prescaler as return an integer value
	TCCR1B = (1 << CS11) | (1 << CS10) | (1 << WGM12);
	SREG |= (1 << 7);
}
ISR(INT0_vect) {

	second = 0;
	minute = 0;
	hour = 0;
}
ISR(INT1_vect) {

	TCCR1B &= ~(1 << CS12);
	TCCR1B &= ~(1 << CS11);
	TCCR1B &= ~(1 << CS10);
}
ISR(INT2_vect) {

	TCCR1B |= (1 << CS11) | (1 << CS10);
}

ISR(TIMER1_COMPA_vect) {
	second++;
	if (second == 60) {
		second = 0;
		minute++;
	}

	if (minute == 60) {
		minute = 0;
		hour++;
	}

	if (hour == 99) {
		hour = 0;
		second = 0;
		minute = 0;
	}
}

int main(void) {

	DDRC |= 0X0F;
	PORTC &= 0XF0;
	DDRA |= 0X3F;
	PORTA &= 0XC0;

	INT0_IN();
	INT1_IN();
	INT2_IN();
	Timer1_IN_Cmp_Mode();

	while (1) {

		PORTA = (PORTA & 0XC0) | ((1 << 0) & 0X3F);
		PORTC = (PORTC & 0XF0) | (second % 10);
		_delay_ms(5);
		PORTA = (PORTA & 0XC0) | ((1 << 1) & 0X3F);
		PORTC = (PORTC & 0XF0) | (second / 10);
		_delay_ms(5);
		PORTA = (PORTA & 0XC0) | ((1 << 2) & 0X3F);
		PORTC = (PORTC & 0XF0) | (minute % 10);
		_delay_ms(5);
		PORTA = (PORTA & 0XC0) | ((1 << 3) & 0X3F);
		PORTC = (PORTC & 0XF0) | (minute / 10);
		_delay_ms(5);
		PORTA = (PORTA & 0XC0) | ((1 << 4) & 0X3F);
		PORTC = (PORTC & 0XF0) | (hour % 10);
		_delay_ms(5);
		PORTA = (PORTA & 0XC0) | ((1 << 5) & 0X3F);
		PORTC = (PORTC & 0XF0) | (hour / 10);
		_delay_ms(5);

	}

	return 0;
}

