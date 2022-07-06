#include <stdio.h>
#include <stdlib.h>
#include "dataADT.h"

#define ARGS 3 //debe ser el argumento del ejecutable mas los dos nombres de los archivos

enum ERRORES {OK=0, ARG_INV, NO_MEMORY};

int main(int argc, char *argv[]){
    if( argc != ARGS ) {
        printf("La cantidad de argumentos ingresada no es valida");
        return ARG_INV;
    }
    //comenzamos con el codigo
    return 0;
}