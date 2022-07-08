#include <stdio.h>
#include <stdlib.h>
#include "dataADT.h"
#include "lectura.h"
#include "queries.h"

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

    while(fgets(line, MAX_LINE, sensors)){
        result = leerSensors(&id, &name, &activo, line);
        VERIFICAR_ERRORES(result, sensors, readings)
        result = cargarSensor(id, name, activo, data);
        VERIFICAR_ERRORES(result, sensors, readings)
    }

    while(fgets(line, MAX_LINE, readings)){
        result = leerReadings(&year, &time, &id, &day, &people, line);
        VERIFICAR_ERRORES(result, sensors, readings)
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
