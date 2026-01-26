/*
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>


// Definiciones de I2C
#define MCP4725_ADDR 0b11000000  // Dirección I2C del MCP4725

// Funciones I2C
void I2C_init(void);
uint8_t I2C_start(void);
uint8_t I2C_slave(uint8_t SLA_W);
uint8_t I2C_write(uint8_t DATA);
void I2C_stop(void);

// Funciones para MCP4725
void MCP4725_write(uint16_t data);

// Funciones UART
void UART_init(void);
void UART_transmit(uint8_t data);
void UART_print(const char* string);
void UART_println(const char* string);

// Variables globales
volatile uint16_t voltage = 0;

int main(void)
{
	I2C_init();
	UART_init();
	//lcd_init(32);

	sei(); // Habilita las interrupciones globales
	
	while (1)
	{
		for (voltage = 0; voltage <= 4095; voltage++)
		{
			I2C_start();
			if (!I2C_slave(MCP4725_ADDR)) {
				// Error al comunicarse con el MCP4725
				continue;
			}
			
			// Enviar voltaje al MCP4725
			MCP4725_write(voltage);
			
			I2C_stop();
			_delay_ms(100);
			
			// Enviar datos a la terminal serial
			char buffer[50];
			sprintf(buffer, "Voltaje enviado: %u\r\n", voltage);
			UART_print(buffer);
		}
	}
}

// Inicialización de I2C
void I2C_init(void)
{
	TWCR = (1<<TWEN); // Habilita I2C
	TWSR &= ~((1<<TWPS1) | (1<<TWPS0)); // Prescaler de 1
	TWBR = 3; // Fórmula para 400kHz: (F_CPU/SCL-16)/(2)(4)^1
}

// Función para iniciar una comunicación I2C
uint8_t I2C_start(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); // Envía START
	while (!(TWCR & (1<<TWINT))); // Espera hasta que se envíe START correctamente
	
	if ((TWSR & 0xF8) != 0x08) // Comprueba el estado START
	return 0;
	else
	return 1;
}

// Función para seleccionar un esclavo I2C
uint8_t I2C_slave(uint8_t SLA_W)
{
	TWDR = SLA_W; // Carga la dirección del esclavo
	TWCR = (1<<TWINT)|(1<<TWEN); // Limpia TWINT y habilita I2C
	while (!(TWCR & (1<<TWINT))); // Espera a que se envíe la dirección correctamente
	
	if ((TWSR & 0xF8) != 0x18) // Comprueba el estado MT_SLA_ACK
	return 0;
	else
	return 1;
}

// Función para enviar datos por I2C
uint8_t I2C_write(uint8_t DATA)
{
	TWDR = DATA; // Carga los datos a enviar
	TWCR = (1<<TWINT)|(1<<TWEN); // Limpia TWINT y habilita I2C
	while (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) != 0x28) // Comprueba el estado MT_DATA_ACK
	return 0;
	else
	return 1;
}

// Función para detener la comunicación I2C
void I2C_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); // Envía STOP
}

// Función para escribir en el MCP4725
void MCP4725_write(uint16_t data)
{
	uint8_t data_high = (data >> 8) & 0x0F;
	uint8_t data_low = data & 0xFF;
	
	I2C_write(data_high);
	I2C_write(data_low);
}

// Inicialización de UART
void UART_init(void)
{
	UBRR0H = (unsigned char)(103 >> 8); // Configura baud rate a 9600bps
	UBRR0L = (unsigned char)103;
	UCSR0B = (1<<TXEN0); // Habilita transmisión
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // Configura 8 bits de datos, 1 bit de stop
}

// Función para transmitir datos por UART
void UART_transmit(uint8_t data)
{
	while (!(UCSR0A & (1<<UDRE0))); // Espera a que el buffer de transmisión esté vacío
	UDR0 = data; // Envía datos
}

// Función para imprimir una cadena por UART
void UART_print(const char* string)
{
	while (*string)
	{
		UART_transmit(*string);
		string++;
	}
}

// Función para imprimir una cadena seguida de nueva línea por UART
void UART_println(const char* string)
{
	UART_print(string);
	UART_print("\r\n");
}

// ISR para manejar interrupciones del UART
ISR(USART_RX_vect)
{
	// Puedes añadir código para manejar las interrupciones del UART si es necesario
}
*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

// Dirección I2C del MCP4725
#define MCP4725_ADDR 0b11000000

// Funciones I2C
void I2C_init(void);
uint8_t I2C_start(void);
uint8_t I2C_slave(uint8_t SLA_W);
uint8_t I2C_write(uint8_t DATA);
uint16_t I2C_read(uint8_t ack);

// Funciones UART
void UART_init(void);
void UART_transmit(uint8_t data);
void UART_print(const char* string);
void UART_println(const char* string);

// Funciones para MCP4725
void MCP4725_write(uint16_t data);
uint16_t MCP4725_read_voltage(void);

// Variables globales
volatile uint16_t voltage = 0;

int main(void)
{
	I2C_init();
	UART_init();

	sei(); // Habilita las interrupciones globales
	
	while (1)
	{
		// Leer voltaje de salida del MCP4725
		uint16_t output_voltage = MCP4725_read_voltage();
		
		// Enviar datos a la terminal serial
		char buffer[50];
		sprintf(buffer, "Voltaje de salida: %u mV\r\n", output_voltage);
		UART_print(buffer);
		
		_delay_ms(1000); // Espera un segundo antes de la siguiente lectura
	}
}

// Inicialización de I2C
void I2C_init(void)
{
	TWCR = (1<<TWEN); // Habilita I2C
	TWSR &= ~((1<<TWPS1) | (1<<TWPS0)); // Prescaler de 1
	TWBR = 3; // Fórmula para 400kHz: (F_CPU/SCL-16)/(2)(4)^1
}

// Función para iniciar una comunicación I2C
uint8_t I2C_start(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); // Envía START
	while (!(TWCR & (1<<TWINT))); // Espera hasta que se envíe START correctamente
	
	if ((TWSR & 0xF8) != 0x08) // Comprueba el estado START
	return 0;
	else
	return 1;
}

// Función para seleccionar un esclavo I2C
uint8_t I2C_slave(uint8_t SLA_W)
{
	TWDR = SLA_W; // Carga la dirección del esclavo
	TWCR = (1<<TWINT)|(1<<TWEN); // Limpia TWINT y habilita I2C
	while (!(TWCR & (1<<TWINT))); // Espera a que se envíe la dirección correctamente
	
	if ((TWSR & 0xF8) != 0x18) // Comprueba el estado MT_SLA_ACK
	return 0;
	else
	return 1;
}

// Función para enviar datos por I2C
uint8_t I2C_write(uint8_t DATA)
{
	TWDR = DATA; // Carga los datos a enviar
	TWCR = (1<<TWINT)|(1<<TWEN); // Limpia TWINT y habilita I2C
	while (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) != 0x28) // Comprueba el estado MT_DATA_ACK
	return 0;
	else
	return 1;
}

// Función para leer datos por I2C
uint16_t I2C_read(uint8_t ack)
{
	if (ack)
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); // Limpia TWINT, habilita I2C y ACK
	else
	TWCR = (1<<TWINT)|(1<<TWEN); // Limpia TWINT y habilita I2C sin ACK
	
	while (!(TWCR & (1<<TWINT))); // Espera a que se reciba el dato
	
	return TWDR; // Devuelve el dato recibido
}

// Función para detener la comunicación I2C
void I2C_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); // Envía STOP
}

// Función para escribir en el MCP4725
void MCP4725_write(uint16_t data)
{
	uint8_t data_high = (data >> 8) & 0x0F; // Bits de control + 4 bits superiores
	uint8_t data_low = data & 0xFF; // 8 bits inferiores
	
	I2C_start();
	I2C_slave(MCP4725_ADDR);
	I2C_write(data_high);
	I2C_write(data_low);
	I2C_stop();
}

// Función para leer el voltaje de salida del MCP4725
uint16_t MCP4725_read_voltage(void)
{
	// Enviar comando de lectura de voltaje al MCP4725
	I2C_start();
	I2C_slave(MCP4725_ADDR);
	I2C_stop();
	
	// Leer los datos de voltaje (12 bits) desde el MCP4725
	I2C_start();
	I2C_slave(MCP4725_ADDR | 0x01); // Cambia a modo lectura
	
	uint16_t data = (I2C_read(1) << 8); // Lee el byte más significativo
	data |= I2C_read(0); // Lee el byte menos significativo
	
	I2C_stop();
	
	return data;
}

// Inicialización de UART
void UART_init(void)
{
	UBRR0H = (unsigned char)(103 >> 8); // Configura baud rate a 9600bps
	UBRR0L = (unsigned char)103;
	UCSR0B = (1<<TXEN0); // Habilita transmisión
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // Configura 8 bits de datos, 1 bit de stop
}

// Función para transmitir datos por UART
void UART_transmit(uint8_t data)
{
	while (!(UCSR0A & (1<<UDRE0))); // Espera a que el buffer de transmisión esté vacío
	UDR0 = data; // Envía datos
}

// Función para imprimir una cadena por UART
void UART_print(const char* string)
{
	while (*string)
	{
		UART_transmit(*string);
		string++;
	}
}

// Función para imprimir una cadena seguida de nueva línea por UART
void UART_println(const char* string)
{
	UART_print(string);
	UART_print("\r\n");
}
