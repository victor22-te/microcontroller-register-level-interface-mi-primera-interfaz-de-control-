/*
 * BH_1750.c
 *
 * Created: 29/06/2024 12:33:11 a. m.
 * Author : victo
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "LCD/lcd.h"

#define BH1750_ADDR  0x23 // DIRECCIÓN DEL SENSOR BH1750
#define   START      0x08 // Maestro envía START
//INSTRUCCIONES DE ARQUITECTURA
#define RESTART      0x10 // Maestro envía ReSTART o cambio de dirección de información
#define MT_SLA_ACK   0x18 // Maestro indica que va escribir y esclavo contesta
#define MR_SLA_ACK   0x40 // Maestro indica que va leer y esclavo contesta
#define MT_DATA_ACK  0x28 // Maestro envía datos y esclavo contesta
#define MR_DATA_ACK  0x50 // Maestro contesta a esclavo que recibío los datos
#define MR_DATA_NACK 0x58 // Maestro recibe datos pero genera un NACK
#define   POWER_ON   0X01 // Encendemos el sensor 
#define H_RESOLUTION 0x10 // Modo para alta resolucion

char print[32], strrx[50];
uint8_t i=0;

// USART initialization function
void init_USART(void) {
	// Set baud rate to 9600
	UBRR0H = (uint8_t)(103 >> 8);
	UBRR0L = (uint8_t)(103);

	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Function to send a character over USART
void USART_sendChar(char data) {
	// Wait until the transmitter is ready
	while (!(UCSR0A & (1 << UDRE0)));

	// Send the data
	UDR0 = data;
}

// Function to send a string over USART
void USART_puts(const char* print) {
	while (*print) {
		USART_sendChar(*print++);
	}
	}


void I2C_init(){
	TWSR=0x00; // Preescaler de 1
	TWBR=0x03; // 400Khz
	TWCR=(1<<TWEN); // Habilitando el I2C
}
uint8_t I2C_start(){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);// Limpiar TWINT, TWSTAR =1 y habiliatar TWI
	while (!(TWCR & (1<<TWINT)));          // Esperamos a que el START se haya enviado correctamente //(TWCR & (1<<TWINT))==0
	if (((TWSR & 0xF8) != START) || ((TWSR & 0xF8) != RESTART))
	return 0;
	else
	return 1;
}
uint8_t I2C_slave(uint8_t SLA_WR){
	TWDR = SLA_WR;                  // Cargamos la dirección del Esclavo
	TWCR = (1<<TWINT)|(1<<TWEN);   // Limpiamos bandera de INT y habilitamos I2C
	while (!(TWCR & (1<<TWINT))) ; // Esperamos a que la dirección se haya enviado correctamente
	
	if (((TWSR & 0xF8) != MT_SLA_ACK) || ((TWSR & 0xF8) != MR_SLA_ACK))  // Verificando que el esclavo recibió la dirección
	return 0;
	else
	return 1;
}
uint8_t I2C_write(uint8_t DATA){
	TWDR = DATA;                 // Cargando registro con los datos a enviar
	TWCR =(1<<TWINT)|(1<<TWEN);  // Limpiando Bandera de INT y Habilitando I2C
	while (!(TWCR & (1<<TWINT))); // Esperando a que los datos hayan sido enviados
	
	if ((TWSR & 0xF8) != MT_DATA_ACK)
	return 0;
	else
	return 1;
}
uint8_t I2C_read_ack(){
	uint8_t DATA;
	
	TWCR =(1<<TWINT)|(1<<TWEN)|(1<<TWEA); // Contesto con Acknowledge
	while (!(TWCR & (1<<TWINT)));
	DATA = TWDR;
	
	if ((TWSR & 0xF8) != MR_DATA_ACK)  // Todo llego bien
	return 0;
	else
	return DATA;
}
uint8_t I2C_read_nack(){
	uint8_t DATA;
	
	TWCR =(1<<TWINT)|(1<<TWEN); // Contesto con Acknowledge
	while (!(TWCR & (1<<TWINT)));
	DATA = TWDR;
	
	if ((TWSR & 0xF8) != MR_DATA_NACK)  // Todo llego bien
	return 0;
	else
	return DATA;
}
void I2C_stop(void){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); // Limpiar Bandera INT, Habilitar I2C y Habilitar Bit Stop
}

void init_BH1750(){
	I2C_start();
	I2C_write(BH1750_ADDR<<1);
	I2C_write(POWER_ON);			// ENCENDER EL SENSOR
	I2C_stop();
}
uint16_t medirLux(){
	uint16_t lux=0;
	
	I2C_start();
	I2C_write(BH1750_ADDR<<1);
	I2C_write(H_RESOLUTION);			// MODO DE MEDICIÓN CONTINUA DE ALTA RESOLUCIÓN
	I2C_stop();
	
	_delay_ms(120);				// TIEMPO DE ESPERA PARA LA MEDICIÓN
	
	I2C_start();
	I2C_write((BH1750_ADDR<<1)|POWER_ON);	// LEER
	lux = (I2C_read_ack()<<8);
	lux |= I2C_read_nack();
	I2C_stop();
	
	lux=lux/1.2;	// AJUSTE PARA OBTENER LA INTENSIDAD EN LUX
	return lux;
}
/*
void mostrarLux(void){
	uint16_t lux=medirLux();
	
/ *
	lcd_gotoxy(0,0);
	sprintf(print,"Lux: %u",lux);
	lcd_puts(print);
	_delay_ms(500);* /
}*/

int main(void) {
	init_USART();
	//lcd_init(LCD_DISP_ON);
	I2C_init();
	init_BH1750();
	uint16_t lux=medirLux();
    while (1) {
		
		sprintf(print,"Lux: %u",lux);
		USART_puts(print);
		_delay_ms(100);
		//mostrarLux();			// PUNTO 1
    }
}

