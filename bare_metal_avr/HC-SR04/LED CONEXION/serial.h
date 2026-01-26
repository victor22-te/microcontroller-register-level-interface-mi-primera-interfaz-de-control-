/*
 * usart.h
 *
 * Created: 03/01/2024 10:34:38 a. m.
 *  Author: PC
 */ 


#ifndef USART_H_
#define USART_H_


#include <avr/interrupt.h> 

#define BAUD0 9600
#define MYUBRR0 F_CPU/16/BAUD0-1


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//USART0
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//Inicalizar serial
void USART0_init()
{
	DDRE |= 0b10; //PORT (TX on PE1)
	unsigned int ubrr = MYUBRR0;
	//Configurar Baudrate con el valor calculado de UBRR
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//Inicializa TX,RX
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	//8 bits de datos y 2 de stop
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	sei();
}
//Envia 1 dato por puerto serial
void usart0_putc(char x){
	//Esperar a que se puedan enviar datos
	while (!(UCSR0A&(1<<UDRE0)));     //Registro UCSR0A Bit UDRE0
	//Enviar datos
	UDR0 = x;
}
//Envia datos por puerto serial
uint8_t usart0_puts(char *s){
	uint8_t i =0;
	//Esperar a que acabe la cadena de caracteres y enviar cada uno
	while (s[i]!='\0'){
		usart0_putc(s[i]);
		i++;
	}
	return i;
}
//Recibe 1 dato por puerto serial
unsigned char usart0_getc( void )
{
	//Esperar a que los datos se reciban (polling)
	while ( !(UCSR0A & (1<<RXC0)) );
	//Retornar datos recibidos
	return UDR0;
}
//Recibe datos por puerto serial
uint8_t usart0_gets(char* data,char endc )
{
	//Inicializa char a un valor que no sea el fin de caracter
	char ch = 0;
	uint8_t i = 0;
	//Mientras no se encuentre el caracter de finalizacion, seguir leyendo
	while (ch != endc)
	{
		ch = usart0_getc();
		//Guardar en el arreglo
		data[i] = ch;
		i++;
	}
	//Fin de cadena, terminar la cadena en caracter nulo
	data[i-1] = '\0';
	//Devuelve la cantidad de datos leidos sin contar el caracter de finalizacion
	return i-1;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif /* USART_H_ */