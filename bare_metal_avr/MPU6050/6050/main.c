#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void uart_init(unsigned int ubrr) {
	// Configurar el registro de tasa de baudios
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;

	// Habilitar la transmisión y recepción
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Configurar el formato de trama: 8 bits de datos, 1 bit de stop
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void uart_transmit_string(const char* str) {
	while (*str) {
		uart_transmit(*str++);
	}
}

void i2c_init() {
	TWSR = 0x00;
	TWBR = 0x47;
	TWCR = (1 << TWEN);
}

void i2c_start(uint8_t address) {
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
	TWDR = address;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
}

void i2c_stop() {
	TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
}

void i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
}

uint8_t i2c_read_ack() {
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

uint8_t i2c_read_nack() {
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

void MPU6050_init() {
	i2c_start(0x68 << 1);
	i2c_write(0x6B);
	i2c_write(0x00); // Wake up the MPU6050
	i2c_stop();

	i2c_start(0x68 << 1);
	i2c_write(0x1A); // Configure DLPF
	i2c_write(0x01); // Set DLPF to 1 (188 Hz)
	i2c_stop();

	i2c_start(0x68 << 1);
	i2c_write(0x19); // Set sample rate
	i2c_write(0x07); // Sample rate = Gyro output rate / (1 + 7) = 1 kHz / 8 = 125 Hz
	i2c_stop();

	i2c_start(0x68 << 1);
	i2c_write(0x1B); // Configure gyroscope
	i2c_write(0x18); // Set gyroscope full scale range to ±2000 degrees/sec
	i2c_stop();

	i2c_start(0x68 << 1);
	i2c_write(0x1C); // Configure accelerometer
	i2c_write(0x10); // Set accelerometer full scale range to ±8g
	i2c_stop();
}

void MPU6050_read_accel(int16_t* ax, int16_t* ay, int16_t* az) {
	i2c_start(0x68 << 1);
	i2c_write(0x3B);
	i2c_stop();

	i2c_start((0x68 << 1) | 0x01);
	*ax = ((int16_t)i2c_read_ack() << 8) | i2c_read_ack();
	*ay = ((int16_t)i2c_read_ack() << 8) | i2c_read_ack();
	*az = ((int16_t)i2c_read_ack() << 8) | i2c_read_nack();
	i2c_stop();
}

void MPU6050_read_gyro(int16_t* gx, int16_t* gy, int16_t* gz) {
	i2c_start(0x68 << 1);
	i2c_write(0x43);
	i2c_stop();

	i2c_start((0x68 << 1) | 0x01);
	*gx = ((int16_t)i2c_read_ack() << 8) | i2c_read_ack();
	*gy = ((int16_t)i2c_read_ack() << 8) | i2c_read_ack();
	*gz = ((int16_t)i2c_read_ack() << 8) | i2c_read_nack();
	i2c_stop();
}

int main(void) {
	int16_t ax, ay, az;
	int16_t gx, gy, gz;
	char buffer[64];

	i2c_init();
	MPU6050_init();
	uart_init(MYUBRR);

	while (1) {
		MPU6050_read_accel(&ax, &ay, &az);
		MPU6050_read_gyro(&gx, &gy, &gz);

		snprintf(buffer, sizeof(buffer), "A:%d,%d,%d G:%d,%d,%d\n", ax, ay, az, gx, gy, gz);
		uart_transmit_string(buffer);

		_delay_ms(1000);
	}
}