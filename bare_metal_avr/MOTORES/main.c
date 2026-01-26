#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#define BAUD 9600
#define M_D PC0
#define M_I PC1
 uint16_t i = 0;

char stn[32] = {'\0'};
char stra[50] = {'\0'};

#define PWM_TOP 500
#define PWM_TOP_RATIO PWM_TOP / 100

uint16_t OCR1A_val = 0;
uint16_t OCR1B_val = 0;
uint16_t TCNT1_val = 0;

uint8_t angle, pwm1, pwm2;

volatile uint8_t motor_detenido = 0;

void init_timer0(void) {
	PORTB &= ~(1 << PORTB7);
	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
	TCCR0B |= (1 << CS02) | (1 << CS00);
	OCR0A = 17;
}

void USART_Init() {
	UBRR0H = (unsigned char)(103 >> 8);
	UBRR0L = (unsigned char)103;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_Transmit(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void giro_derecho(void) {
	PORTC |= (1 << M_D);
	PORTC &= ~(1 << M_I);
}

void giro_izquierdo(void) {
	PORTC |= (1 << M_I);
	PORTC &= ~(1 << M_D);
}

void espera(void) {
	PORTC &= ~((1 << M_D) | (1 << M_I));
}

void detener_motor(void) {
	PORTC &= ~((1 << M_D) | (1 << M_I)); // Apagar el motor
	motor_detenido = 1; // Establecer la bandera de motor detenido
}

void set_duty_cycle(uint8_t duty) {
	duty = (duty > 100) ? 100 : duty;
	uint16_t D = PWM_TOP_RATIO * duty;
	OCR1A_val = (uint16_t)D;
	OCR1A = OCR1A_val;
}

void init_Timer1() {
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	DDRB |= (1 << PB7);
	TCCR1A |= (1 << COM1A1);
	TCCR1A |= (1 << COM1B1);
	TCCR1B |= (1 << CS11) | (1 << CS10);
	ICR1 = PWM_TOP;
	TCNT1 = 0;
}

void puertos_salida(void) {
	DDRB |= (1 << DDB7);
	DDRC |= (1 << M_D) | (1 << M_I);
}

ISR(USART0_RX_vect) {
	char dato_recibido;
	dato_recibido = UDR0;

	if (dato_recibido == '\n') {
		if (stra[i - 1] == '\r') {
			stra[i - 1] = '\0';
			 uint16_t i = 0;

			if (strncmp(stra, "SERVO", 5) == 0) {
				angle = atoi(stra + 5);
				OCR0A = 17 + (angle / 180.0) * (43 - 17);
				} else if (strncmp(stra, "derecha", 7) == 0) {
				giro_derecho();
				} else if (strncmp(stra, "izquierda", 9) == 0) {
				giro_izquierdo();
				} else if (strncmp(stra, "VELOCIDAD", 9) == 0) {
				pwm1 = atoi(stra + 9);
				set_duty_cycle(pwm1);
				} else if (strncmp(stra, "paro", 4) == 0) {
				detener_motor();
			}
			} else {
			stra[i] = '\0';
			i = 0;

			if (strncmp(stra, "SERVO", 5) == 0) {
				angle = atoi(stra + 5);
				OCR0A = 17 + (angle / 180.0) * (43 - 17);

				} else if (strncmp(stra, "derecha", 7) == 0) {
				giro_derecho();
				} else if (strncmp(stra, "izquierda", 9) == 0) {
				giro_izquierdo();
				} else if (strncmp(stra, "VELOCIDAD", 9) == 0) {
				pwm1 = atoi(stra + 9);
				set_duty_cycle(pwm1);
				} else if (strncmp(stra, "paro", 4) == 0) {
				espera();
			}
		}
		} else {
		stra[i] = dato_recibido;
		i++;
	}
}

int main(void) {
	init_timer0();
	init_Timer1();
	USART_Init();
	puertos_salida();
	sei();
	while (1) {
		_delay_ms(1000);
	}

	return 0;
}
