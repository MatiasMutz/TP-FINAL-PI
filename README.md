# TPE-Final

### FECHA DE ENTREGA: 11/07/2022

### Integrantes:
* Boullosa Gutierrez Juan Cruz (legajo: 63414)
* Deyheralde Ben (legajo: 63559)
* Mutz Matias (legajo: 63590)

### **Resumen**:

Este programa está diseñado para el procesamiento de datos de sensores de peatones según los datos reales de la ciudad de Melbourne.
La salida serán 3 archivos .csv con los datos procesados según 3 consultas distintas.


### **Pasos a seguir**:

1) Descargar en una misma carpeta los siguientes archivos:
* main.c
* dataADT.c
* queries.c
* lectura.c
* makefile

2) Desde la terminal, colocarse en la carpeta donde están los archivos descargados e insertar el siguiente comando:
`make`

Este comando compila el programa y crea el archivo ejecutable llamado pedestrians.

3) Ahora ya se puede ejecutar el programa de la siguiente manera:
`./pedestrians sensors.csv readings.csv`

Siendo sensors.csv el archivo de los sensores(**1*) y readings.csv(**2*) el archivo con las mediciones totales.

ATENCIÓN: es MUY importante enviar los archivos en este orden para el correcto funcionamiento del programa.

4) Para eliminar los archivos código objetos generados (.o) se inserta el siguiente comando: `make clean`

REFERENCIAS: 

* (**1*): el archivo de sensores debe ser un archivo con 3 columnas separadas con ';' tendiendo el siguiente orden: el
id del sensor, la segunda el nombre y la tercera el estado ('A' para activo y 'R' para removido).
* (**2*): el archivo de mediciones debe ser un archivo con 7 columnas separadas con ';' tendiendo el siguiente orden: el
año, el mes, el día (número), el día (nombre), el id del sensor, el horario y la cantidad de personas de la medición.