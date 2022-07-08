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

#define ARGS 3 //debe ser el argumento del ejecutable mas los dos nombres de los archivos

int main(int argc, char *argv[]){
    enum ERRORS result = OK;

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
    if(sensors == NULL){
        printf("El archivo que no fue encontrado o no existe\n");
        freeAll(data);
        return NOT_EXIST;
    }
    FILE *readings = fopen(argv[2], "rt");
    if(readings == NULL){
        fclose(sensors);
        printf("El archivo que no fue encontrado o no existe\n");
        freeAll(data);
        return NOT_EXIST;
    }
    fgets(line, MAX_LINE, sensors); //para saltearme el encabezado
    while(fgets(line, MAX_LINE, sensors)){
        result = leerSensors(&id, &name, &activo, line);
        if(result != OK){
            fclose(sensors);
            fclose(readings);
            freeAll(data);
            return result;
        }
        result = cargarSensor (id, name, activo, data);
        if(result != OK){
            fclose(sensors);
            fclose(readings);
            freeAll(data);
            return result;
        }
    }
    fclose(sensors);

    fgets(line, MAX_LINE, readings); //para saltearme el encabezado
    while(fgets(line, MAX_LINE, readings)){
        result = leerReadings(&year, &time, &id, &day, &people, line);
        if(result != OK){
            fclose(sensors);
            fclose(readings);
            freeAll(data);
            return result;
        }
        result = processLine(data, id, people, day, year, time);
        if(result != OK){
            fclose(sensors);
            fclose(readings);
            freeAll(data);
            return result;
        }
    }
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
