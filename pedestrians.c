#include <stdio.h>
#include <stdlib.h>
#include "dataADT.h"
#define VERIFICA_PROCESADO if(qResult == NOT_PROCESSED) {\
                                printf("Los datos no fueron procesados.\n");\
                                return NOT_PROCESSED;}\

#define ARGS 3 //debe ser el argumento del ejecutable mas los dos nombres de los archivos

int main(int argc, char *argv[]){
    if( argc != ARGS ) {
        printf("La cantidad de argumentos ingresada no es valida.\n");
        return ARG_INV;
    }
    dataADT data = NULL;
    //LEER


    enum ERRORS processResult = processData(argv[1], argv[2], &data);
    if(processResult == NOT_EXIST) {
        printf("El archivo pasado por parametro no existe o no fue encontrado.\n");
        return NO_MEMORY;
    }
    else if(processResult == NO_MEMORY){
        printf("No hay espacio disponible para guardar datos.\n");
        return NO_MEMORY;
    }

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
