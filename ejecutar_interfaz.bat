@echo off
echo ========================================
echo   Interfaz de Sensores - Practica Final
echo ========================================
echo.
echo Activando entorno virtual...
call venv\Scripts\activate.bat
echo.
echo Ejecutando interfaz...
python ProyectoSensoresPractFinal.py
echo.
echo Presiona cualquier tecla para salir...
pause >nul
