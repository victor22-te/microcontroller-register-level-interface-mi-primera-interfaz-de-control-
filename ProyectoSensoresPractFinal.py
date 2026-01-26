# -*- coding: utf-8 -*-
# #from gzip import BadGzipFile

import os
from pathlib import Path
from tkinter import *
from tkinter import ttk
from turtle import position
import serial
import threading
import time

# Obtener el directorio donde está el script
SCRIPT_DIR = Path(__file__).parent.absolute()


#Configuracion de la conexion serial
#arduino = serial.Serial('COM4',9600) #datos enviados al com4 a una velocidad de 9600BD
#time.sleep(2)
"""
def leer_datos_MAX66(puerto="COM3", baudios=2400):

def leer_datos_MPU(puerto="COM3", baudios=2400):

def leer_datos_TCS(puerto="COM3", baudios=2400):

def leer_datos_MLX(puerto="COM3", baudios=2400):

def leer_datos_MCP(puerto="COM3", baudios=2400):

def leer_datos_Motor(puerto="COM3", baudios=2400):
    """
# Funci�n para leer datos del sensor HC-SR04
def leer_datos_HC(puerto="COM4", baudios=9600):
    #abrimos una nueva ventana
    ventana_HC = Toplevel(raiz)
    ventana_HC.title("Distancia en cm:")
    ventana_HC.geometry("500x200")
    ventana_HC.resizable(False,False)
    
    arduino = serial.Serial('COM4',9600)
    time.sleep(2)#tiempo en S
    
    #Enviamos un dato
    dato = '2'
    arduino.write(dato.encode('ascii'))

    dist = arduino.readline().decode('ascii')#lee la cadena y la decodifica
    
    datos_corr = Label(ventana_HC, text=f"La distancia de su objeto es de {dist} ", font=("Comic Sans MS",14))
    datos_corr.place(x=50, y=50)  # Ajusta las coordenadas (x, y) seg�n sea necesario
    btn_regresar = Button(ventana_HC, text="Regresar al Menu Principal", command=ventana_HC.destroy)
    btn_regresar.place(x=150, y=100)  # Ajusta las coordenadas (x, y) seg�n sea necesario
    
def leer_datos_BH(puerto="COM4", baudios=9600):
    #abrimos una ventana secundaria
    ventana_BC = Toplevel(raiz)
    ventana_BC.title("Intensidad de luz")
    ventana_BC.geometry("500x200")
    ventana_BC.resizable(False,False)
    
    arduino = serial.Serial('COM4',9600)
    time.sleep(2)#tiempo en S
    
    #Enviamos un dato
    dato = '3'
    arduino.write(dato.encode('ascii'))
    
    lux = arduino.readline().decode('ascii')#lee la cadena y la decodifica
    datos_lux = []
    
    
    
    
    #<3
    datosBC_label = Label(ventana_BC, text=f"La intensidad de luz es de {lux} lm/m^2", fg="black", font=("Comic Sans MS", 12))
    datosBC_label.place(x=135, y=50)  # Ajusta las coordenadas (x, y) seg�n sea necesario
    btn_regresar = Button(ventana_BC, text="Regresar al Menu Principal", command=ventana_BC.destroy)
    btn_regresar.place(x=145, y=105)  # Ajusta las coordenadas (x, y) seg�n sea necesario

def Matrizdeleds():
    ventana_sensores = Toplevel(raiz)
    ventana_sensores.title("Matriz de leds")
    ventana_sensores.geometry("300x300")
    ventana_sensores.resizable(False,False)
    
    # Crear y colocar el campo de texto y su etiqueta en la nueva ventana
    mensaje_label = Label(ventana_sensores, text="Escribe un mensaje:", fg="black", font=("Comic Sans MS", 14))
    mensaje_label.place(x=50, y=50)  # Ajusta las coordenadas (x, y) segun sea necesario

    mensaje_entry = Entry(ventana_sensores, width=30)  # Ajusta el ancho del campo de texto
    mensaje_entry.place(x=50, y=100)  # Ajusta las coordenadas (x, y) segun sea necesario
    btn_regresar = Button(ventana_sensores, text="Regresar al Menu Principal", command=ventana_sensores.destroy)
    btn_regresar.place(x=65, y=200)  # Ajusta las coordenadas (x, y) seg�n sea necesario
 
def abrir_controlador_motor():
    ventana_motor = Toplevel(raiz)
    ventana_motor.title("Controlador de Motor")
    ventana_motor.geometry("400x500")
    ventana_motor.resizable(False,False)
    
    # Crear y colocar los cuatro botones en la nueva ventana
    btn1 = Button(ventana_motor, text="Encendido")
    btn1.pack(pady=10)
    
    btn2 = Button(ventana_motor, text="Horario")
    btn2.pack(pady=10)
    
    btn3 = Button(ventana_motor, text="Antihorario")
    btn3.pack(pady=10)
    
    btn4 = Button(ventana_motor, text="Velocidad")
    btn4.pack(pady=10)
    
    btn5 = Button(ventana_motor, text="Paro")
    btn5.pack(pady=10)
    
    # Indicador visual del tablero del carro (puedes personalizar con imagenes)
    label_estado = Label(ventana_motor, text="Estado del Motor:", font=("Arial", 12))
    label_estado.pack(pady=10)
    
    estado_motor = Label(ventana_motor, text="Apagado", fg="red", font=("Arial", 12, "bold"))
    estado_motor.pack()
    
    label_velocidad = Label(ventana_motor, text="Duty Cycle:", font=("Arial", 12))
    label_velocidad.pack(pady=10)
    
    velocidad_actual = Label(ventana_motor, text="0 %", font=("Arial", 12))
    velocidad_actual.pack()
    
    # Slider para ajustar la velocidad del carro
    velocidad_slider = ttk.Scale(ventana_motor, from_=0, to=100, orient=HORIZONTAL, length=300)
    velocidad_slider.pack(pady=20)
    
    def actualizar_velocidad():
        velocidad = velocidad_slider.get()
        velocidad_actual.config(text=f"{velocidad} %")
        if velocidad > 0:
            estado_motor.config(text="Encendido", fg="green")
        else:
            estado_motor.config(text="Apagado", fg="red")
    
    btn_actualizar = Button(ventana_motor, text="Actualizar Velocidad", command=actualizar_velocidad)
    btn_actualizar.pack(pady=10)

    btn_regresar = Button(ventana_motor, text="Regresar al Menu Principal", command=ventana_motor.destroy)
    btn_regresar.place(x=200, y=500)  # Ajusta las coordenadas (x, y) seg�n sea necesario
        
def tempLM35():
    #abrimos ventana secundaria
    ventana_LM = Toplevel(raiz)
    ventana_LM.title("Temperatura Ambiente")
    ventana_LM.geometry("500x200")
    ventana_LM.resizable(False,False)
    
    arduino = serial.Serial('COM4',9600)
    time.sleep(2)#tiempo en S
    
    #Enviamos un dato
    dato = '1'
    arduino.write(dato.encode('ascii'))
    
    #Variable para almacenar el valor de la temperatura
    temp = arduino.readline().decode('ascii')#lee la cadena y la decodifica
    
    #Texto muestra
    temp_lm = Label(ventana_LM, text= f"La temperatura ambiente actual es de {temp} ", fg="black", font=("Comic Sans MS", 14)) #crear la funcion para llamar al dato
    temp_lm.place(x=50, y=50)  # Ajusta las coordenadas (x, y) seg�n sea necesario
    
    btn_regresar = Button(ventana_LM, text="Regresar al Menu Principal", command=ventana_LM.destroy)
    btn_regresar.place(x=100, y=300)  # Ajusta las coordenadas (x, y) seg�n sea necesario
    
def ControlServo():
    ventana_ajustes = Toplevel(raiz)
    ventana_ajustes.title("Servo Motor")
    ventana_ajustes.geometry("300x200")
    ventana_ajustes.resizable(False,False)
    
    # Crear y colocar la etiqueta y el campo de texto para ingresar el angulo
    angulo_label = Label(ventana_ajustes, text="Ingrese un angulo:", fg="black", font=("Comic Sans MS", 14))
    angulo_label.place(x=80, y=25)  # Ajusta las coordenadas (x, y) seg�n sea necesario
    
    angulo_entry = Entry(ventana_ajustes, width=20)  # Ajusta el ancho del campo de texto
    angulo_entry.place(x=80, y=75)  # Ajusta las coordenadas (x, y) seg�n sea necesario 
    btn_regresar = Button(ventana_ajustes, text="Regresar al Menu Principal", command=ventana_ajustes.destroy)
    btn_regresar.place(x=70, y=125)  # Ajusta las coordenadas (x, y) seg�n sea necesario

def leer_datos_CONV():
    #Iniciamos una nueva ventana
    ventana_CONV = Toplevel(raiz)
    ventana_CONV.title("Convertidor Digital-Analogico")
    ventana_CONV.geometry("250x300")
    ventana_CONV.resizable(False,False)
    
    #Luegar donde leeremos y enviaremos datos
    arduino = serial.Serial('COM4',9600)
    time.sleep(2)#tiempo en S
    
    #Enviamos un dato
    dato = '7'
    arduino.write(dato.encode('ascii'))
    
    #Variable para almacenar el valor de la temperatura
    volt = arduino.readline().decode('ascii')#lee la cadena y la decodifica agrega V al dato enviado
    
    #Texto muestra
    cam_vol = Label(ventana_CONV, text= f"El voltaje actual es de {volt} ", fg="black", font=("Comic Sans MS", 14)) #crear la funcion para llamar al dato
    cam_vol.place(x=50, y=50)  # Ajusta las coordenadas (x, y) seg�n sea necesario
    
    btn_regresar = Button(ventana_CONV, text="Regresar al Menu Principal", command=ventana_CONV.destroy)
    btn_regresar.place(x=50, y=100)  # Ajusta las coordenadas (x, y) seg�n sea necesario

def leer_datos_GPS():
    ventana_GPS = Toplevel(raiz)
    ventana_GPS.title("GPS")
    ventana_GPS.geometry("250x300")
    btn_regresar = Button(ventana_GPS, text="Regresar al Menu Principal", command=ventana_GPS.destroy)
    btn_regresar.place(x=50, y=100)  # Ajusta las coordenadas (x, y) seg�n sea necesario

def leer_datos_MAX():
    #Abrimos una nueva ventana
    ventana_MAX= Toplevel(raiz)
    ventana_MAX.title("Temperatura por MAX")
    ventana_MAX.geometry("250x300")
    ventana_MAX.resizable(False,False)
    
    arduino = serial.Serial('COM4',9600)
    time.sleep(2)#tiempo en S
    
    #Enviamos un dato
    dato = '1'
    arduino.write(dato.encode('ascii'))
    
    #dato a enviar para texto
    envio = '$'
    
    angulo_entry = Entry(ventana_MAX, width=20)  # Ajusta el ancho del campo de texto
    angulo_entry.place(x=80, y=75)  # Ajusta las coordenadas (x, y) seg�n sea necesario
    
    def enviar_dato():
        ang = envio + angulo_entry.get()
        

    btn_regresar = Button(ventana_MAX, text="Regresar al Menu Principal", command=ventana_MAX.destroy)
    btn_regresar.place(x=50, y=100)  # Ajusta las coordenadas (x, y) seg�n sea necesario

def leer_datos_TCS():##justar dato de envio
    #Abrir una nueva mentana
    ventana_TCS = Toplevel(raiz)
    ventana_TCS.title("Indicadores del RGBr")
    ventana_TCS.geometry("550x500")
    ventana_TCS.resizable(False,False)
    
    #objeto que envia y recibe datos
    arduino = serial.Serial('COM4',9600)
    time.sleep(2)#tiempo en S
    
    #Enviamos un dato
    dato = '5'
    arduino.write(dato.encode('ascii'))
    
    #variable donde guardamos datooooo
    R = arduino.readline().decode('ascii')#lee la cadena y la decodifica
    G = arduino.readline().decode('ascii')#lee la cadena y la decodifica
    B = arduino.readline().decode('ascii')#lee la cadena y la decodifica
     
   # Crear y colocar etiquetas para los indicadores de barras
    label_R = Label(ventana_TCS, text="Rojo", font=("Comic Sans MS", 14))
    label_R.pack(pady=5)
    
    frame_R = Frame(ventana_TCS, bg="black", width=210, height=30)
    frame_R.pack(pady=5)
    frame_R.pack_propagate(False)  # Evita que el frame cambie de tama�o autom�ticamente
    barra_R = Canvas(frame_R, width=200, height=30, bg="black")
    barra_R.pack()
    barra_R.create_rectangle(2, 2, 2, 30, fill="red", outline="red")
    porcentaje_R = Label(ventana_TCS, text="0%", font=("Comic Sans MS", 12))
    porcentaje_R.pack(pady=5)
    
    label_G = Label(ventana_TCS, text="Verde", font=("Comic Sans MS", 14))
    label_G.pack(pady=5)
    
    frame_G = Frame(ventana_TCS, bg="black", width=210, height=30)
    frame_G.pack(pady=5)
    frame_G.pack_propagate(False)
    barra_G = Canvas(frame_G, width=200, height=30, bg="black")
    barra_G.pack()
    barra_G.create_rectangle(2, 2, 2, 30, fill="green", outline="green")
    porcentaje_G = Label(ventana_TCS, text="0%", font=("Comic Sans MS", 12))
    porcentaje_G.pack(pady=5)
    
    label_B = Label(ventana_TCS, text="Azul", font=("Comic Sans MS", 14))
    label_B.pack(pady=5)
    
    frame_B = Frame(ventana_TCS, bg="black", width=210, height=30)
    frame_B.pack(pady=5)
    frame_B.pack_propagate(False)
    barra_B = Canvas(frame_B, width=200, height=30, bg="black")
    barra_B.pack()
    barra_B.create_rectangle(2, 2, 2, 30, fill="blue", outline="blue")
    porcentaje_B = Label(ventana_TCS, text="0%", font=("Comic Sans MS", 12))
    porcentaje_B.pack(pady=5)
    
    def actualizar_barras():
        # Aqu� leer�as los datos del sensor TCS3200
        # Los valores de ejemplo a continuaci�n deben ser reemplazados por los valores reales del sensor
        rojo = R  # valor ejemplo
        verde = G  # valor ejemplo
        azul = B  # valor ejemplo
        
        # Calcular el ancho de las barras bas�ndose en los valores le�dos
        ancho_maximo = 200  # Ancho m�ximo de la barra
        barra_R.coords(1, 2, 2 + ancho_maximo * rojo / 100, 30)
        barra_G.coords(1, 2, 2 + ancho_maximo * verde / 100, 30)
        barra_B.coords(1, 2, 2 + ancho_maximo * azul / 100, 30)
        
        # Actualizar los porcentajes
        porcentaje_R.config(text=f"{rojo}%")
        porcentaje_G.config(text=f"{verde}%")
        porcentaje_B.config(text=f"{azul}%")
        
        # Programar la actualizaci�n peri�dica
        ventana_TCS.after(1000, actualizar_barras)  # Actualiza cada 1 segundo (1000 ms)
    
    # Iniciar la primera actualizaci�n
    actualizar_barras()
    
    btn_regresar = Button(ventana_TCS, text="Regresar al Menu Principal", command=ventana_TCS.destroy)
    btn_regresar.place(x=100, y=400)  # Ajusta las coordenadas (x, y) seg�n sea necesario

def leer_datos_MLX():
    ventana_MLX = Toplevel(raiz)
    ventana_MLX.title("Temperatura por MLX")
    ventana_MLX.geometry("250x300")
    btn_regresar = Button(ventana_MLX, text="Regresar al Menu Principal", command=ventana_MLX.destroy)
    btn_regresar.place(x=50, y=100)  # Ajusta las coordenadas (x, y) seg�n sea necesario

def leer_datos_MPU():
    #Agregamos una nueva ventana
    ventana_MPU = Toplevel(raiz)
    ventana_MPU.title("Acelerometro y Giroscopio")
    ventana_MPU.geometry("500x500")
    #ventana_MPU.resizable(False, False)
    
    #objeto que envia y recibe datos
    arduino = serial.Serial('COM4',9600)
    time.sleep(2)#tiempo en S
    
    #Enviamos un dato
    dato = '4'
    arduino.write(dato.encode('ascii'))
    
    #Variable para almacenar el valor de la temperatura
    XLR8 = arduino.readline().decode('ascii')#lee la cadena y la decodifica
    pos = arduino.readline().decode('ascii')#lee la cadena y la decodifica
    
    #Texto muestra
    muesXLR = Label(ventana_MPU, text= f"La aceleracion es de {XLR8} \nLa posicion del giro es de {pos}  ", fg="black", font=("Comic Sans MS", 14)) #crear la funcion para llamar al dato
    muesXLR.place(x=50, y=50)  # Ajusta las coordenadas (x, y) seg�n sea necesario

    btn_regresar = Button(ventana_MPU, text="Regresar al Menu Principal", command=ventana_MPU.destroy)
    btn_regresar.place(x=100, y=450)  # Ajusta las coordenadas (x, y) seg�n sea necesario


# Raiz
raiz = Tk()  # Crea la raiz

raiz.title("Practica Mortal de Millones de Sensores dxd")  # Titulo de la ventana
raiz.resizable(True, True)  # Permite cambiar el tamano de la ventana
raiz.iconbitmap(str(SCRIPT_DIR / "icono.ico"))  # Icono de la ventana con ruta absoluta
raiz.geometry("800x500")  # Ancho y alto de la ventana
raiz.config(bg="black")  # Color de fondo de la ventana

# Frame
elframe = Frame(raiz, bg = "blue")  # Crea el frame dentro de la raiz
elframe.pack(fill="both", expand=True)  # Empaqueta el frame con la raiz
elframe.config(bg="blue")  # Color de fondo del frame
elframe.config(width=650, height=350)
elframe.config(bd=10)  # Ancho del borde del frame
elframe.config(relief="raised")  # Tipo de relieve del frame
elframe.config(cursor="hand2")  # Tipo de cursor dentro del frame

# Cargar la imagen
imagen = PhotoImage(file=str(SCRIPT_DIR / "termome.png"))  # Imagen con ruta absoluta

# Labels (textos)
milabel = Label(elframe, text="Sensores de muestra natural", fg="black", font=("Comic Sans MS", 20), bg="blue")
milabel.pack(pady=20)  # Anadido padding para separar el texto del borde del frame


#LADO IZQUIERDOOOOOO
#Boton para control de motor 
btn_abrir_motor= Button(elframe, text="CONTROLADOR DE MOTOR", command=abrir_controlador_motor, width=35, height=1,bg= "red", fg="white" )
btn_abrir_motor.place(x=25, y=150)

#Boton para la matriza
btn_abrir_matriz= Button(elframe, text="MATRIZ DE LEDS", command=Matrizdeleds, width=35, height=1,bg= "red", fg="white")
btn_abrir_matriz.place(x=25, y=200)  # Ajusta las coordenadas (x, y) segun sea necesario

#Boton para el servo
btn_abrir_servo= Button(elframe, text="MOVIMIENTO DE SERVOMOTOR", command=ControlServo, width=35, height=1,bg= "red", fg="white" )
btn_abrir_servo.place(x=25, y=250)  # Ajusta las coordenadas (x, y) segun sea necesario

#Boton para el LM35
btn_abrir_LM = Button(elframe, text="TEMPERATURA AMBIENTE LM35", command=tempLM35, width=35, height=1,bg= "red", fg="white" )
btn_abrir_LM.place(x=25, y=300)  # Ajusta las coordenadas (x, y) segun sea necesario

#Boton para el HC-SR04
btn_abrir_HC = Button(elframe, text="DISTANCIA HC-SR04", command=leer_datos_HC, width=35, height=1,bg= "red", fg="white" )
btn_abrir_HC.place(x=25, y=350)  # Ajusta las coordenadas (x, y) segun sea necesario

#Boton para el CONVERTIDOR DIG-ANA
btn_abrir_CONV= Button(elframe, text="CONVERTIDOR DIG-ANA", command=leer_datos_CONV, width=35, height=1,bg= "red", fg="white" )
btn_abrir_CONV.place(x=25, y=400)  # Ajusta las coordenadas (x, y) segun sea necesario




#LADO DERECHOOOO
#Boton para el BH
btn_abrir_BH = Button(elframe, text="INTENCIDAD DE LUZ BH1750", command=leer_datos_BH, width=35, height=1,bg= "red", fg="white" )
btn_abrir_BH.place(x=500, y=150)  # Ajusta las coordenadas (x, y) segun sea necesario

#Boton para el GPS
btn_abrir_GPS = Button(elframe, text="LOCALIZADOR GPS", command=leer_datos_GPS, width=35, height=1,bg= "red", fg="white" )
btn_abrir_GPS.place(x=500, y=200)  # Ajusta las coordenadas (x, y) segun sea necesario

#Boton para el MAX
btn_abrir_MAX = Button(elframe, text="TEMPERATURA SENSOR MAX", command=leer_datos_MAX, width=35, height=1,bg= "red", fg="white" )
btn_abrir_MAX.place(x=500, y=250)  # Ajusta las coordenadas (x, y) segun sea necesario

#Boton para el TCS
btn_abrir_TCS = Button(elframe, text="BARRAS DE NIVEL RGB", command=leer_datos_TCS, width=35, height=1,bg= "red", fg="white" )
btn_abrir_TCS.place(x=500, y=300)  # Ajusta las coordenadas (x, y) segun sea necesario

#Boton para el MLX
btn_abrir_MLX = Button(elframe, text="TEMPERATURA SENSOR MLX", command=leer_datos_MLX, width=35, height=1,bg= "red", fg="white" )
btn_abrir_MLX.place(x=500, y=350)  # Ajusta las coordenadas (x, y) segun sea necesario

#Boton para el MPU
btn_abrir_MPU = Button(elframe, text="ACELEROMETRO Y GIROSCOPIO", command=leer_datos_MPU, width=35, height=1,bg= "red", fg="white" )
btn_abrir_MPU.place(x=500, y=400)  # Ajusta las coordenadas (x, y) segun sea necesario



# Ejecutar el bucle principal de la aplicacion
raiz.mainloop()
