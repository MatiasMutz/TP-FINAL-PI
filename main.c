#include <stdio.h>
#include <stdlib.h>
#include "dataADT.h"
#include "lectura.h"
#include "queries.h"

#define MAX_LINE 1024

#define VERIFICA_PROCESADO(x) if(x == NOT_PROCESSED) {\
                                printf("Los datos no fueron procesados.\n");\
                                freeAll(data);\
                                return NOT_PROCESSED;}

#define VERIFICAR_ERRORES(result, sensors, readings) if(result != OK){\
                                                     fclose(sensors);\
                                                     fclose(readings);\
                                                     freeAll(data);\
                                                     return result;}

#define ARGS 3 //debe ser el argumento del ejecutable mas los dos nombres de los archivos

int main(int argc, char *argv[]){
    enum ERRORS result = OK;

    char* nombre_dias[CANT_DIAS]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};

    if( argc != ARGS ) {
        printf("La cantidad de argumentos ingresada no es valida.\n");
        return ARG_INV;
    }

    errno = 0;
    dataADT data = newData(nombre_dias);
    if(errno == ENOMEM){
        return ENOMEM;
    }

    //ABRO AMBOS ARCHIVOS
    FILE *sensors = openFile(argv[1]);
    if(sensors == NULL){
        printf("El archivo que no fue encontrado o no existe\n");
        freeAll(data);
        return NOT_EXIST;
    }

    FILE *readings = openFile(argv[2]);
    if(readings == NULL){
        fclose(sensors);
        printf("El archivo que no fue encontrado o no existe\n");
        freeAll(data);
        return NOT_EXIST;
    }

    //LEER
    size_t id, people;
    char* name;
    char* activo;
    char* day;
    unsigned short year, time;

    while(leerSensors(&id, &name, &activo, sensors) == OK){
        result = cargarSensor(id, name, activo, data);
        VERIFICAR_ERRORES(result, sensors, readings)
    }

    while(leerReadings(&year, &time, &id, &day, &people, readings) == OK){
        result = processLine(data, id, people, day, year, time);
        VERIFICAR_ERRORES(result, sensors, readings)
    }

    fclose(sensors);
    fclose(readings);

    result = ordenarSensors(data);
    VERIFICA_PROCESADO(result)

    result = query1(data);
    VERIFICA_PROCESADO(result)
    result = query2(data);
    VERIFICA_PROCESADO(result)
    result = query3(data);
    VERIFICA_PROCESADO(result)
    freeAll(data);
    return OK; 
}
