# Bare-Metal AVR - Programación a Nivel de Registros

Esta carpeta contiene el código fuente de todos los sensores y actuadores programados a **nivel de registros** (bare-metal) para el microcontrolador **ATmega328P** (Arduino UNO), sin usar librerías de alto nivel.

## 📁 Estructura de Carpetas

### Sensores de Temperatura
- **`lm35/`** - Sensor de temperatura analógico LM35
  - Lectura mediante ADC (registros ADMUX, ADCSRA)
  - Conversión a grados Celsius
  
- **`max6575/`** - Sensor de temperatura digital MAX6575
  - Comunicación I2C/SPI
  - Configuración de registros TWI
  
- **`mxl90614/`** - Sensor de temperatura infrarrojo MLX90614
  - Comunicación I2C
  - Lectura de temperatura sin contacto

### Sensores de Distancia y Luz
- **`HC-SR04/`** - Sensor ultrasónico de distancia
  - Control de pines mediante GPIO (DDRx, PORTx, PINx)
  - Medición de tiempo con Timer1
  
- **`BH_1750/`** - Sensor de intensidad de luz
  - Comunicación I2C (registros TWI)
  - Medición en lux

### Sensores de Color y Movimiento
- **`TCS/`** - Sensor de color TCS3200
  - Lectura de frecuencia RGB
  - Control de pines de selección
  
- **`MPU6050/`** - Acelerómetro y giroscopio
  - Comunicación I2C
  - Lectura de 6 ejes (3 aceleración + 3 giroscopio)

### Actuadores
- **`MOTORES/`** - Control de motor DC
  - PWM mediante Timer0/Timer2 (registros TCCRx, OCRx)
  - Control de dirección con puente H
  
- **`MAX7219/`** - Matriz de LEDs 8x8
  - Comunicación SPI
  - Display de mensajes y patrones

### Otros
- **`GPS/`** - Módulo GPS
  - Comunicación UART (registros UCSRx, UBRRx, UDRx)
  - Parseo de datos NMEA
  
- **`MCP/`** - Convertidor Digital-Analógico MCP
  - Comunicación SPI/I2C
  - Generación de señales analógicas

## 🔧 Características Técnicas

### Sin Librerías de Alto Nivel
❌ **NO se usa:**
- `Serial.begin()`, `Serial.print()`
- `analogRead()`, `analogWrite()`
- `digitalWrite()`, `digitalRead()`
- `Wire.begin()`, `Wire.write()`
- Librerías de terceros

✅ **SE USA:**
- Configuración directa de registros AVR
- Manipulación de bits con máscaras
- Control manual de periféricos
- Interrupciones configuradas por registros

### Periféricos Configurados

#### UART (Comunicación Serial)
```c
// Configuración de baudrate a 9600
UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
UBRR0L = (unsigned char)UBRR_VALUE;

// Habilitar transmisión y recepción
UCSR0B = (1 << RXEN0) | (1 << TXEN0);

// Formato: 8 bits de datos, 1 bit de stop
UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
```

#### ADC (Conversión Analógica-Digital)
```c
// Referencia AVCC, canal ADC0
ADMUX = (1 << REFS0);

// Habilitar ADC, prescaler 128
ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

// Iniciar conversión
ADCSRA |= (1 << ADSC);
```

#### PWM (Control de Motores)
```c
// Timer0 en modo Fast PWM
TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
TCCR0B = (1 << CS01); // Prescaler 8

// Duty cycle (0-255)
OCR0A = duty_cycle;
```

#### I2C/TWI (Comunicación con Sensores)
```c
// Configurar velocidad I2C (100kHz)
TWBR = ((F_CPU / 100000L) - 16) / 2;

// Habilitar TWI
TWCR = (1 << TWEN);

// Enviar START
TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
```

## 🛠️ Compilación

### Atmel Studio
1. Abre el archivo `.atsln` de cada proyecto
2. Selecciona el dispositivo: **ATmega328P**
3. Configura el clock: **16 MHz**
4. Compila: `Build` → `Build Solution` (F7)
5. Programa: `Tools` → `Device Programming`

### AVR-GCC (Línea de comandos)
```bash
# Compilar
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o main.elf main.c

# Generar HEX
avr-objcopy -O ihex -R .eeprom main.elf main.hex

# Programar con avrdude
avrdude -c arduino -p m328p -P COM4 -b 115200 -U flash:w:main.hex:i
```

## 📊 Configuración de Pines

### Pines Comunes
| Función | Pin Arduino | Puerto AVR | Registro |
|---------|-------------|------------|----------|
| UART TX | D1 | PD1 | PORTD |
| UART RX | D0 | PD0 | PORTD |
| I2C SDA | A4 | PC4 | PORTC |
| I2C SCL | A5 | PC5 | PORTC |
| PWM OC0A | D6 | PD6 | PORTD |
| PWM OC0B | D5 | PD5 | PORTD |
| PWM OC1A | D9 | PB1 | PORTB |
| PWM OC1B | D10 | PB2 | PORTB |
| ADC0-5 | A0-A5 | PC0-5 | PORTC |

## 📖 Recursos de Aprendizaje

### Datasheets
- [ATmega328P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
- Ver carpeta `Data Sheets/` para datasheets de sensores

### Tutoriales Recomendados
- AVR Libc Documentation
- Microchip AVR Instruction Set Manual
- Embedded C Programming

## 🔍 Notas Importantes

1. **Frecuencia de Clock**: Todos los proyectos asumen **F_CPU = 16000000UL** (16 MHz)
2. **Voltaje**: 5V (Arduino UNO estándar)
3. **Comunicación Serial**: 9600 baudios por defecto
4. **Pull-ups I2C**: Requieren resistencias externas de 4.7kΩ

## 🐛 Debugging

### Verificar Comunicación UART
```c
// Enviar carácter de prueba
while (!(UCSR0A & (1 << UDRE0)));
UDR0 = 'A';
```

### Verificar ADC
```c
// Leer valor ADC
ADCSRA |= (1 << ADSC);
while (ADCSRA & (1 << ADSC));
uint16_t adc_value = ADC;
```

### Verificar I2C
```c
// Verificar ACK después de enviar dirección
if ((TWSR & 0xF8) != 0x18) {
    // Error: No ACK recibido
}
```

## 📝 Licencia

Este código es parte del proyecto educativo de Microcontroladores e Interfaz.
Licencia MIT - Ver archivo LICENSE en la raíz del repositorio.
