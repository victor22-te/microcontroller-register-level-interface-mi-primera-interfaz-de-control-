# Guía Rápida: Subir a GitHub

## 📤 Pasos para Subir el Proyecto a GitHub

### 1️⃣ Crear Repositorio en GitHub

1. Ve a [GitHub](https://github.com) e inicia sesión
2. Haz clic en el botón **"+"** (arriba a la derecha) → **"New repository"**
3. Configura tu repositorio:
   - **Repository name**: `register-level-arduino-sensor-dashboard`
   - **Description**: "Dashboard de sensores Arduino con programación a nivel de registros (bare-metal AVR)"
   - **Public** o **Private** (tu elección)
   - ❌ **NO** marques "Initialize this repository with a README"
   - ❌ **NO** agregues .gitignore ni license (ya los tenemos)
4. Haz clic en **"Create repository"**

### 2️⃣ Inicializar Git Localmente

**Opción A: Usando el script automático (Recomendado)**
```bash
# Doble clic en:
inicializar_git.bat
```

**Opción B: Manualmente**
```bash
cd "c:\Users\Spider-Man\Desktop\Desktop\Clases\Microcontroladores, Microprocesadores e Interfaz\Sensores Practica Final\interfaz\ProyectoSensoresPractFinal"

git init
git add .
git commit -m "Initial commit: Arduino Register-Level Sensor Dashboard"
```

### 3️⃣ Conectar con GitHub y Subir

Reemplaza `TU_USUARIO` con tu nombre de usuario de GitHub:

```bash
git remote add origin https://github.com/TU_USUARIO/register-level-arduino-sensor-dashboard.git
git branch -M main
git push -u origin main
```

### 4️⃣ Verificar

Ve a tu repositorio en GitHub y verifica que todos los archivos se hayan subido correctamente.

---

## 🔄 Comandos Git Útiles

### Hacer cambios y subirlos
```bash
git add .
git commit -m "Descripción de los cambios"
git push
```

### Ver estado del repositorio
```bash
git status
```

### Ver historial de commits
```bash
git log --oneline
```

### Crear una nueva rama
```bash
git checkout -b nombre-de-la-rama
```

### Cambiar entre ramas
```bash
git checkout main
git checkout nombre-de-la-rama
```

---

## 📋 Archivos que se Subirán

✅ `ProyectoSensoresPractFinal.py` - Código principal  
✅ `requirements.txt` - Dependencias  
✅ `ejecutar_interfaz.bat` - Script de ejecución  
✅ `README.md` - Documentación principal  
✅ `README_ENTORNO_VIRTUAL.md` - Guía del entorno virtual  
✅ `LICENSE` - Licencia MIT  
✅ `.gitignore` - Archivos ignorados  
✅ `icono.ico` - Icono de la aplicación  
✅ `termome.png` - Imagen de la interfaz  

❌ `venv/` - Entorno virtual (ignorado por .gitignore)  
❌ `.vs/` - Archivos de Visual Studio (ignorado)  
❌ `*.pyproj`, `*.sln` - Archivos de proyecto (ignorado)

---

## 🎨 Personalizar el README

Antes de subir, considera personalizar:

1. **Agregar capturas de pantalla**: Toma screenshots de la interfaz y agrégalos a una carpeta `screenshots/`
2. **Actualizar el autor**: Cambia `[Tu Nombre]` en LICENSE
3. **Agregar diagrama de conexiones**: Crea un diagrama de cómo conectar los sensores
4. **Incluir código Arduino**: Si tienes el código del microcontrolador, agrégalo en una carpeta `arduino/`

---

## 🚨 Solución de Problemas

### Error: "git: command not found"
Instala Git desde: https://git-scm.com/download/win

### Error: "Permission denied (publickey)"
Configura SSH o usa HTTPS:
```bash
git remote set-url origin https://github.com/TU_USUARIO/TU_REPO.git
```

### Error: "Updates were rejected"
```bash
git pull origin main --rebase
git push
```

---

## 📞 Ayuda

Si necesitas ayuda, consulta:
- [Documentación oficial de Git](https://git-scm.com/doc)
- [Guía de GitHub](https://docs.github.com/es)
