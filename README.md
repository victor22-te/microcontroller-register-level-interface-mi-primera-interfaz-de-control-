# Register-Level Arduino Sensor Dashboard
 **Dashboard de sensores Arduino con programación a nivel de registros (bare-metal AVR)**

Interfaz gráfica Python/Tkinter para monitoreo y control de múltiples sensores y actuadores conectados a Arduino, programado completamente a nivel de registros sin usar librerías de alto nivel.

![Python](https://img.shields.io/badge/Python-3.x-blue.svg)
![Arduino](https://img.shields.io/badge/Arduino-AVR-00979D.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

## Características

### Sensores Soportados

| Sensor/Actuador | Función | Comunicación |
|----------------|---------|--------------|
| **LM35** | Temperatura ambiente | ADC |
| **MAX** | Temperatura (termopar) | I2C/SPI |
| **MLX** | Temperatura infrarroja | I2C |
| **HC-SR04** | Distancia ultrasónica | GPIO |
| **BH1750** | Intensidad de luz | I2C |
| **TCS3200** | Sensor de color RGB | GPIO/PWM |
| **MPU6050** | Acelerómetro y giroscopio | I2C |
| **GPS** | Localización | UART |
| **Motor DC** | Control de velocidad | PWM |
| **Servomotor** | Control de ángulo | PWM |
| **Matriz LED** | Display de mensajes | GPIO |
| **DAC** | Convertidor Digital-Analógico | SPI/I2C |

## Programación a Nivel de Registros (Bare-Metal)

Este proyecto **NO utiliza las librerías estándar de Arduino IDE**. Todo el código del microcontrolador está programado directamente manipulando los registros del AVR ATmega328P:

### Periféricos Configurados Manualmente:

- ✅ **UART/USART**: Configuración de baudrate mediante `UBRR0`, control de transmisión/recepción con `UCSR0A/B/C`
- ✅ **ADC**: Conversión analógica-digital configurando `ADMUX`, `ADCSRA`, `ADCH/ADCL`
- ✅ **Timers/PWM**: Control de motores mediante `TCCR0A/B`, `TCCR1A/B`, `OCR0A/B`, `OCR1A/B`
- ✅ **GPIO**: Manipulación directa de `DDRx`, `PORTx`, `PINx`
- ✅ **I2C/TWI**: Comunicación con sensores usando `TWBR`, `TWSR`, `TWDR`, `TWCR`

### Sin Abstracción de Alto Nivel
❌ Sin `Serial.begin()`, `analogRead()`, `digitalWrite()`, etc.  
✅ Control total del hardware  
✅ Optimización de recursos  
✅ Comprensión profunda del microcontrolador

## Instalación

### Requisitos

- Python 3.x
- Arduino UNO (ATmega328P)
- Cable USB para comunicación serial

### Configuración del Entorno Virtual

1. **Clonar el repositorio:**
```bash
git clone https://github.com/TU_USUARIO/register-level-arduino-sensor-dashboard.git
cd register-level-arduino-sensor-dashboard
```

2. **Crear entorno virtual:**
```bash
python -m venv venv
```

3. **Activar entorno virtual:**

**Windows (PowerShell):**
```powershell
.\venv\Scripts\Activate.ps1
```

**Windows (CMD):**
```cmd
.\venv\Scripts\activate.bat
```

**Linux/Mac:**
```bash
source venv/bin/activate
```

4. **Instalar dependencias:**
```bash
pip install -r requirements.txt
```

## Dependencias

- **pyserial**: Comunicación serial con Arduino
- **tkinter**: Interfaz gráfica (incluido con Python)

## Uso

### Opción 1: Script de ejecución rápida (Windows)
```bash
ejecutar_interfaz.bat
```

### Opción 2: Ejecución manual
```bash
# Activar entorno virtual
.\venv\Scripts\Activate.ps1

# Ejecutar interfaz
python ProyectoSensoresPractFinal.py
```

### Configuración del Puerto Serial

Por defecto, la interfaz usa el puerto **COM4** a **9600 baudios**. Si tu Arduino está en otro puerto:

1. Verifica el puerto en el Administrador de Dispositivos (Windows) o `ls /dev/tty*` (Linux/Mac)
2. Modifica las líneas en `ProyectoSensoresPractFinal.py`:
```python
arduino = serial.Serial('COM4', 9600)  # Cambiar COM4 por tu puerto
```

## Capturas de Pantalla

### Menú Principal
La interfaz principal muestra 12 botones para acceder a cada módulo de sensor/actuador.

### Módulos Disponibles
- Control de Motor DC con PWM
- Matriz de LEDs
- Control de Servomotor
- Temperatura LM35
- Distancia HC-SR04
- Convertidor Digital-Analógico
- Intensidad de Luz BH1750
- GPS
- Temperatura MAX
- Barras RGB (TCS3200)
- Temperatura MLX
- Acelerómetro y Giroscopio (MPU6050)

## Conexiones Hardware

### Pines Arduino (ATmega328P)

| Sensor | Pin Arduino | Registro AVR |
|--------|-------------|--------------|
| UART TX/RX | D0/D1 | USART (UDR0) |
| ADC (LM35) | A0-A5 | ADC (ADMUX) |
| PWM Motor | D3, D5, D6 | Timer0/1 (OCRx) |
| I2C (SDA/SCL) | A4/A5 | TWI (TWDR) |

## Estructura del Proyecto

```
ProyectoSensoresPractFinal/
├── ProyectoSensoresPractFinal.py  # Interfaz principal Python
├── requirements.txt                # Dependencias Python
├── ejecutar_interfaz.bat          # Script de ejecución rápida
├── inicializar_git.bat            # Script para inicializar Git
├── README.md                       # Este archivo
├── README_ENTORNO_VIRTUAL.md      # Guía del entorno virtual
├── GUIA_GITHUB.md                 # Guía para subir a GitHub
├── LICENSE                         # Licencia MIT
├── .gitignore                      # Archivos ignorados por Git
├── icono.ico                       # Icono de la aplicación
├── termome.png                     # Imagen de la interfaz
├── venv/                           # Entorno virtual (no en Git)
└── bare_metal_avr/                 # Código AVR a nivel de registros
    ├── README.md                   # Documentación del código bare-metal
    ├── lm35/                       # Sensor de temperatura LM35
    ├── max6575/                    # Sensor de temperatura MAX6575
    ├── mxl90614/                   # Sensor de temperatura infrarrojo
    ├── HC-SR04/                    # Sensor ultrasónico de distancia
    ├── BH_1750/                    # Sensor de intensidad de luz
    ├── TCS/                        # Sensor de color TCS3200
    ├── MPU6050/                    # Acelerómetro y giroscopio
    ├── GPS/                        # Módulo GPS
    ├── MOTORES/                    # Control de motor DC
    ├── MAX7219/                    # Matriz de LEDs 8x8
    ├── MCP/                        # Convertidor DAC
    └── Data Sheets/                # Hojas de datos de sensores
```

## 🛠️ Desarrollo

### Agregar un Nuevo Sensor

1. Crear función de lectura en `ProyectoSensoresPractFinal.py`:
```python
def leer_datos_NUEVO_SENSOR():
    ventana = Toplevel(raiz)
    # Configuración de la ventana
    # Comunicación serial
    # Mostrar datos
```

2. Agregar botón en la interfaz principal:
```python
btn_nuevo = Button(elframe, text="NUEVO SENSOR", 
                   command=leer_datos_NUEVO_SENSOR)
btn_nuevo.place(x=25, y=450)
```

## Solución de Problemas

### Error: "bitmap 'icono.ico' not defined"
**Solucionado**: El código usa rutas absolutas automáticamente.

### Error: "Serial port not found"
- Verifica que el Arduino esté conectado
- Comprueba el puerto COM en el Administrador de Dispositivos
- Asegúrate de que ningún otro programa esté usando el puerto

### PowerShell no permite ejecutar scripts
Ejecuta como Administrador:
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

## Contexto Académico

Este proyecto fue desarrollado como práctica final para el curso de **Microcontroladores, Microprocesadores e Interfaz**, con énfasis en:

- Programación de bajo nivel (bare-metal)
- Configuración directa de registros AVR
- Comunicación serial UART
- Conversión ADC
- Generación de PWM con timers
- Protocolo I2C/TWI
- Interfaz gráfica de usuario

## Licencia

Este proyecto está bajo la Licencia MIT. Ver el archivo `LICENSE` para más detalles.

## Autor

Proyecto desarrollado para la clase de Microcontroladores, Microprocesadores e Interfaz.

## Contribuciones

Las contribuciones son bienvenidas. Por favor:

1. Fork el proyecto
2. Crea una rama para tu feature (`git checkout -b feature/NuevaCaracteristica`)
3. Commit tus cambios (`git commit -m 'Agregar nueva característica'`)
4. Push a la rama (`git push origin feature/NuevaCaracteristica`)
5. Abre un Pull Request

## Soporte

Si tienes problemas o preguntas, abre un **Issue** en GitHub.

---

⭐ Si este proyecto te fue útil, considera darle una estrella en GitHub! 🕷️
