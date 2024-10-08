#include "queries.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERIFICA_PROCESADO_QUERIES(x,name) if(x == NOT_PROCESSED) {\
                                fclose(name);\
                                return NOT_PROCESSED;}

int query1(dataADT data){
    enum ERRORS result = OK;
    FILE* query1 = fopen("query1.csv", "wt");
    fprintf(query1, "sensor;counts\n");
    size_t dim;
    char* name;
    size_t cantP_sensor;
    result = getCantSensores(data, &dim);
    VERIFICA_PROCESADO_QUERIES(result,query1);
    for (int i = 0; i < dim; i++){
        result = getDataQ1(data, &name, &cantP_sensor, i);
        VERIFICA_PROCESADO_QUERIES(result,query1);
        fprintf(query1, "%s;%zu\n" , name, cantP_sensor);
    }
    fclose(query1);
    return OK;
}

int query2(dataADT data){
    enum ERRORS result = OK;
    FILE *query2 = fopen("query2.csv", "wt");
    fprintf(query2, "year;counts\n");
    toBeginQ2(data);
    unsigned short year;
    size_t cantPerYear;
    while((result = getDataQ2(data, &year, &cantPerYear))==OK){
        fprintf(query2,"%u;%zu\n", year, cantPerYear);
    }
    VERIFICA_PROCESADO_QUERIES(result,query2);
    fclose(query2);
    return OK;
}

int query3(dataADT data){
    enum ERRORS result = OK;
    FILE *query3 = fopen("query3.csv", "wt");
    fprintf(query3, "day;day_counts;night_counts;total_counts\n");
    char* dia;
    size_t cantP_diurno, cantP_nocturno, suma;
    for (int i = 0; i < CANT_DIAS; i++) {
        result = getDataQ3(data, &dia, &cantP_diurno, &cantP_nocturno, &suma, i);
        VERIFICA_PROCESADO_QUERIES(result,query3);
        fprintf(query3,"%s;%zu;%zu;%zu\n", dia, cantP_diurno, cantP_nocturno, suma);
    }
    fclose(query3);
    return OK;
}
