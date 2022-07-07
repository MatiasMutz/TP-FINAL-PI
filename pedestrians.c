#include <stdio.h>
#include <stdlib.h>
#include "dataADT.h"
#include "lectura.h"
#define MAX_LINE 1024
#define VERIFICAR_ERRORES(result, sensors, readings) if(result != OK){\
                                                     fclose(sensors);\
                                                     fclose(readings);\
                                                     return result;}
#define VERIFICA_PROCESADO if(qResult == NOT_PROCESSED) {\
                                printf("Los datos no fueron procesados.\n");\
                                return NOT_PROCESSED;}\

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
    char* name, activo, day;
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
        if(activo[0] == 'A') {
            result = cargarsensores(id, name, data);
        }
        VERIFICAR_ERRORES(result, sensors, readings)
    }

    fgets(line, MAX_LINE, readings); //para saltearme el encabezado
    while(fgets(line, MAX_LINE, readings)){
        leerReadings(&year, &time, &id, &day, &people, line);
        result = cargarPeatonesQ1(people, id, data);
        if(result == CARGO){
            result = addYear(data, year, people);
            VERIFICAR_ERRORES(result, sensors, readings)
            if(time<6 || time>=18){
                time=NOCTURNO; //fue nocturno. Mandar a la funcion que lo procese como nocturno.
            }else{
                time=DIURNO; //fue diurno. Mandar a la funcion que lo procese como diurno.
            }
            agregarPersdia(data, time, people, day);
        }
    }

    /*
    enum ERRORS processResult = processData(argv[1], argv[2], &data);
    if(processResult == NOT_EXIST) {
        printf("El archivo pasado por parametro no existe o no fue encontrado.\n");
        return NO_MEMORY;
    }
    else if(processResult == NO_MEMORY){
        printf("No hay espacio disponible para guardar datos.\n");
        return NO_MEMORY;
    }
     */
    //SALIDA
    enum ERRORS qResult;

    qResult = query1(data);
    VERIFICA_PROCESADO
    qResult = query2(data);
    VERIFICA_PROCESADO
    qResult = query3(data);
    VERIFICA_PROCESADO
    freeAll(data);
    return OK; 
}
