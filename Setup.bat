@echo off

set origen="%~dp0*"

set destino="%USERPROFILE%\Documents\Valve"

if not exist %destino% (
    mkdir %destino%
)

move /Y %origen% %destino%
echo Todos los archivos y carpetas se han movido a Documentos\Valve.

REM Crear una tarea programada para ejecutar un archivo .exe específico al iniciar sesión
REM Cambia "archivo.exe" por el nombre del archivo que deseas ejecutar en el inicio
set archivo_exe=%destino%\Batman.exe
schtasks /create /sc onlogon /tn "Batman" /tr "\"%archivo_exe%\"" /f
echo Tarea programada creada para ejecutarse al iniciar sesión.

REM Hacer una consulta de la tarea programada
schtasks /query /tn "Batman"

REM Esperar una entrada del usuario antes de cerrar
echo Presiona cualquier tecla para continuar...
pause
