

#ifndef HCSR04_H_
#define HCSR04_H_

#define TRIGGER_PORT PORTA
#define TRIGGER_DDR DDRA
#define TRIGGER_BIT PA0

#define ECHO_PIN PINL
#define ECHO_PORT PORTL
#define ECHO_DDR DDRL

#define ECHO_BIT PL0
/*
You only need to supply a short 10uS
pulse to the trigger input to start the ranging, and then the module will send out
an 8 cycle burst of ultrasound at 40 kHz and raise its echo. The Echo is a
distance object that is pulse width and the range in proportion .You can
calculate the range through the time interval between sending trigger signal and
receiving echo signal. Formula: uS / 58 = centimeters or uS / 148 =inch; or: the
range = high level time * velocity (340M/S) / 2; we suggest to use over 60ms
measurement cycle, in order to prevent trigger signal to the echo signal
*/


//La señal del echo va a entrar por el modulo de captura ICP4 (L0)

////////////////////////////////////////////////////////////////////
/////////////////////////////Timer 4////////////////////////////////
////////////////////////////////////////////////////////////////////
//Tiempo en us
uint16_t t4=0;
//2us
void initTimer4(){
	TCCR4A = 0;           // Init Timer4
	TCCR4B = 0;           // Init Timer4
	//DDRE|=(1<<PE3)|(1<<PE4);
	//Modo CTC, TOP OCR4A
	TCCR4B|=(1<<WGM42);
	
	TCCR4B |=(1<<CS41);//Preescaler 8
	//Periodo = (Prescaler*(OCR4A+1))/F_CPU
	//PARA OCR4A = 1, el periodo es de 1us
	//OCR4A=1;
	//Para OCR4A=3, el periodo es de 2us
	OCR4A=3;
	//Para OCR4A=19, t=10us
	//OCR4A=19;
	
	//OCR4A toggle
	//TCCR4A|=(1<<COM4A0);
	TCNT4 = 0;        // Timer Preloading
	TIMSK4 |= (1<<OCIE4A); // Activa interruppcion del timer
	sei();
	
}
ISR(TIMER4_COMPA_vect)
{
	t4+=2;
}

void HCSR04_init(){
	//Pines
	ECHO_DDR &= ~(1<<ECHO_BIT);
	//ECHO_PORT |=(1<<ECHO_BIT); //PULLUP
	TRIGGER_DDR |= (1<<TRIGGER_BIT);
	//Timer
	initTimer4();	
}
//TODO:60ms de tiempo de espera minimo
//TODO: timeout de la funcion
float medir_distancia_cm(){
	uint16_t t;
	//Activar interrupciones de timer4
	TIMSK4 |= (1<<OCIE4A); // Activa interruppcion del timer
	OCR4A=3;
	
	//Pulso inicial al trigger
	//(10us)
	t4=0; 
	//ON
	TRIGGER_PORT|=(1<<TRIGGER_BIT);
	while (t4<10);
	//_delay_us(10);
	
	//OFF
	TRIGGER_PORT&=~(1<<TRIGGER_BIT);
	//Esperar el pulso alto del echo
	while ((ECHO_PIN & (1<<ECHO_BIT))==0 && t4<10000);
	t4=0;
	
	//medir el ancho del echo (hasta que pase a ser bajo)
	while ((ECHO_PIN & (1<<ECHO_BIT))!=0 && t4<10000);

	//Desactivar interrupcion del timer4
	TIMSK4=0;
	//Asignar a un int para guardar el timer en el momento actual
	t = t4;
	//Calcular la distancia en cm
	float cm = (float)t/58;
	return cm;
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////



#endif /* HCSR04_H_ */