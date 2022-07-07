#include <stdio.h>
#include <stdlib.h>
#include "dataADT.h"
#include "lectura.h"
#include "queries.h"
#define MAX_LINE 1024

#define VERIFICA_PROCESADO(x) if(x == NOT_PROCESSED) {\
                                printf("Los datos no fueron procesados.\n");\
                                return NOT_PROCESSED;}
#define VERIFICAR_ERRORES(result, sensors, readings) if(result != OK){\
                                                     fclose(sensors);\
                                                     fclose(readings);\
                                                     return result;}

#define ARGS 3 //debe ser el argumento del ejecutable mas los dos nombres de los archivos

int main(int argc, char *argv[]){
    if( argc != ARGS ) {
        printf("La cantidad de argumentos ingresada no es valida.\n");
        return ARG_INV;
    }

    errno = 0;
    dataADT data = newData();
    if(errno == ENOMEM){
        return ENOMEM;
    }

    //LEER
    size_t id, people;
    char* name;
    char* activo;
    char* day;
    char line[MAX_LINE];
    unsigned short year, time;

    //ABRO AMBOS ARCHIVOS
    FILE *sensors = fopen(argv[1], "rt");
    if(sensors == NULL)
        return NOT_EXIST;
    FILE *readings = fopen(argv[2], "rt");
    if(readings == NULL){
        fclose(sensors);
        return NOT_EXIST;
    }

    int result = OK;

    fgets(line, MAX_LINE, sensors); //para saltearme el encabezado
    while(fgets(line, MAX_LINE, sensors)){
        leerSensors(&id, &name, &activo, line);
        cargarSensor (id, name, activo[0], data);
        VERIFICAR_ERRORES(result, sensors, readings)
    }
    fclose(sensors);

    fgets(line, MAX_LINE, readings); //para saltearme el encabezado
    while(fgets(line, MAX_LINE, readings)){
        leerReadings(&year, &time, &id, &day, &people, line);
        result = processLine(data, id, people, name, activo, day, year, time);
        VERIFICAR_ERRORES(result, sensors, readings)
    }
    fclose(readings);

    ordenarSensors(data);

    //SALIDA
    enum ERRORS qResult = OK;

    qResult = query1(data);
    VERIFICA_PROCESADO(qResult)
    qResult = query2(data);
    VERIFICA_PROCESADO(qResult)
    qResult = query3(data);
    VERIFICA_PROCESADO(qResult)
    freeAll(data);
    return OK; 
}
