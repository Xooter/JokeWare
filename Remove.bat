@echo off

set nombre_tarea=Batman

schtasks /delete /tn "%nombre_tarea%" /f
echo La tarea %nombre_tarea% ha sido eliminada exitosamente.
