# Instrucciones para usar el Entorno Virtual

## ¿Qué es un entorno virtual?
Un entorno virtual es un espacio aislado donde puedes instalar las dependencias de Python específicas para este proyecto sin afectar otras instalaciones de Python en tu sistema.

## Cómo activar el entorno virtual

### En PowerShell (Windows):
```powershell
.\venv\Scripts\Activate.ps1
```

### En CMD (Windows):
```cmd
.\venv\Scripts\activate.bat
```

Cuando el entorno virtual esté activado, verás `(venv)` al inicio de tu línea de comandos.

## Cómo ejecutar la interfaz

1. Activa el entorno virtual (ver arriba)
2. Ejecuta el script:
```powershell
python ProyectoSensoresPractFinal.py
```

## Cómo desactivar el entorno virtual

Simplemente escribe:
```powershell
deactivate
```

## Dependencias instaladas

- **pyserial 3.5**: Para la comunicación serial con Arduino
- **tkinter**: Viene incluido con Python, se usa para la interfaz gráfica

## Notas importantes

- **Puerto COM**: Asegúrate de que el Arduino esté conectado al puerto COM4 (o modifica el código si usas otro puerto)
- **Velocidad**: La comunicación está configurada a 9600 baudios
- **Archivos necesarios**: 
  - `icono.ico` (icono de la ventana)
  - `termome.png` (imagen de fondo)

## Solución de problemas

### Si PowerShell no permite ejecutar scripts:
Ejecuta este comando en PowerShell como Administrador:
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### Si no detecta el puerto COM:
- Verifica en el Administrador de Dispositivos qué puerto COM está usando tu Arduino
- Modifica el código en las líneas donde aparece `'COM4'` por el puerto correcto
