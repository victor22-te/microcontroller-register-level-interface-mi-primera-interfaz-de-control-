/*
 * lECTURA_DATOS_MLX_90614.c
 *
 * Created: 28/06/2024 02:32:34 p. m.
 * Author : victor
 
 PROTOCOLO I2C MODO ESCLAVO
 
 1.- Inicializar i2c -> Habillita I2C Establecer velocidad de transferencia
 2.- Funcion I2C START
 3.- Funcion para Read
 4.- Funcion para Write
 5.- Funcion para Re-Start
 6.- Funcion de STOP
 */ 

#define F_CPU 16000000ul
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD/lcd.h"

#define STATUS 0xF8
// MAPA DE COMANDOS
#define START 0x08
#define MT_SLA_ACK 0x18
#define MT_DATA_ACK 0x28
#define MR_SLA_ACK 0x40
#define MR_DATA_ACK 0x50
#define MR_DATA_NACK 0x58

#define MLX90614_ADDR 0x5A
#define MLX90614_TA 0x06
#define MLX90614_TOBJ1 0x07

#define ERROR 0

/*
La lectura PWM
Bit0         -> tipo de PWM
Bit1         -> Enable PWM
Bit2         -> Configuracion PWM
Bit3         -> Selecciona el modo
Bits[8:4]    ->  PWM definicion
Bits[15:9]   -> Reloj
*/

char print[32];
void I2C_init(void){
	TWCR = (1<<TWEN);//HABILITA I2C
	TWSR &= ~((1<<TWPS1) | (1<<TWPS0));//TWSR DETERMINA PREESCALADOR -> PRESCALADOR DE 1
	TWBR = 3; //PARA LA FRECUENCIA DE 400KHz -> (F_CPU/SCL-16)/(2)(4)^1	
}

uint8_t I2C_start(){
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); //LIMPIAMOS TWINT, TWSTAR = 1 Y HABILITAMOS I2C
	while (! (TWCR & (1<<TWINT)) );//ESPERAMOS HATA QUE EL STAR SE ENVIE CORRECTAMENTE
	if (( TWSR & STATUS) != START)
		return ERROR;
	else
		return 1;
}

uint8_t I2C_slave(uint8_t SLA_W){
	TWDR = SLA_W;                //CARGAMOS LA DIRECCION DEL ESCLAVO
	TWCR = (1<<TWINT)|(1<<TWEN); //LIMPIAMOS LA BANDERA TWINT Y HABILITAMOS I2C
	while (!(TWCR & (1<<TWINT))); //ESPERA A QUE LA DIRECCION SE ENVIE CORRECTAMENTE
	
	if ((TWSR & STATUS) != MT_SLA_ACK)
		return ERROR;
	else
		return 1;	
}

uint8_t I2C_read_ack(){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); //LIMPIAMOS LA BANDERA TWINT Y HABILITAMOS I2C
	while (!(TWCR & (1<<TWINT)));
		
	return TWDR; //DATO RECIBIDO
}

uint8_t I2C_read_nack(){
	TWCR = (1<<TWINT) | (1<<TWEN); // Habilita la recepción de datos sin enviar un ACK
	while (!(TWCR & (1<<TWINT)));
	return TWDR; // Devuelve el dato recibido
}

uint8_t I2C_write(uint8_t DATA){
	TWDR = DATA;                //CARGANDO REGISTRO DE LOS DATOS  ENVIAR
	TWCR = (1<<TWINT)|(1<<TWEN); //LIMPIAMOS LA BANDERA TWINT Y HABILITAMOS I2C
	while (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & STATUS) != MT_DATA_ACK)
	return ERROR;
	else
	return 1;
}

void I2C_stop(void){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);//LIMPIAMOS LA BANDETA INT -> 1, HABILITA I2C, HABILITA BIT DE STOP
	while (TWCR & (1<<TWSTO));
}

uint16_t read_MLX90614(uint8_t reg){
	uint16_t data;
	I2C_start();
	I2C_slave(MLX90614_ADDR << 1); // Dirección de escritura
	I2C_write(reg); // Registro a leer
	I2C_start(); // Repeated start
	I2C_slave((MLX90614_ADDR << 1) | 1); // Dirección de lectura
	data = I2C_read_ack(); // Leer byte bajo
	data |= ((uint16_t)I2C_read_nack() << 8); // Leer byte alto
	I2C_stop();
	return data;
}

float convert_temp(uint16_t raw_temp){
	return (raw_temp * 0.02) - 273.15; // Conversión a grados Celsius
}

int main(void){
	I2C_init();
	lcd_init(LCD_DISP_ON); // Inicializa el LCD
	/*lcd_clrscr();*/
	lcd_gotoxy(0,0);

	while (1){
		
		uint16_t raw_temp = read_MLX90614(MLX90614_TA);
		float temp = convert_temp(raw_temp);

		// Mostrar temperatura en el LCD
		
		sprintf(print,"Temp: %.2f C", temp);
		lcd_puts(print);

		_delay_ms(500);
		
	}
}