@echo off
echo ========================================
echo   Inicializando Repositorio Git
echo ========================================
echo.

REM Verificar si ya existe un repositorio
if exist .git (
    echo Ya existe un repositorio Git en este directorio.
    echo.
    choice /C SN /M "Deseas continuar de todas formas"
    if errorlevel 2 goto :end
)

echo Inicializando repositorio...
git init

echo.
echo Agregando archivos al staging area...
git add .

echo.
echo Creando primer commit...
git commit -m "Initial commit: Arduino Register-Level Sensor Dashboard"

echo.
echo ========================================
echo   Repositorio inicializado exitosamente
echo ========================================
echo.
echo Proximos pasos:
echo 1. Crea un repositorio en GitHub
echo 2. Ejecuta los siguientes comandos:
echo.
echo    git remote add origin https://github.com/TU_USUARIO/TU_REPO.git
echo    git branch -M main
echo    git push -u origin main
echo.
echo ========================================

:end
pause
