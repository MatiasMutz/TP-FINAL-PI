#include "queries.h"
//Carga los datos del query 1 en el archivo csv

#define VERIFICA_PROCESADO(x) if(x == NOT_PROCESSED) {\
                                printf("Los datos no fueron procesados.\n");\
                                return NOT_PROCESSED;}

int query1(dataADT data){
    enum ERRORS result = OK;
    FILE* query1 = fopen("query1.csv", "wt");
    fprintf(query1, "sensor;counts\n");
    size_t dim;
    char* name;
    size_t cantP_sensor;
    dim = getCantSensores(data);
    for (int i = 0; i < dim; i++){
        result = q1Processed(data, &name, &cantP_sensor, i);
        VERIFICA_PROCESADO(result);
        fprintf(query1, "%s;%zu\n" , name, cantP_sensor);
    }
    fclose(query1);
    return OK;
}

int query2(dataADT data){
    enum ERRORS result = OK;
    FILE *query2 = fopen("query2.csv", "wt");
    fprintf(query2, "year;counts\n");
    toBegin(data);
    unsigned short year;
    size_t cantPerYear;
    while((result = q2Processed(data, &year, &cantPerYear))==OK){
        fprintf(query2,"%u;%zu\n", year, cantPerYear);
    }
    VERIFICA_PROCESADO(result);
    fclose(query2);
    return OK;
}

//Carga los datos del query 3 en el archivo csv
int query3(dataADT data){
    enum ERRORS result = OK;
    FILE *query3 = fopen("query3.csv", "wt");
    fprintf(query3, "day;day_counts;night_counts;total_counts\n");
    char* dia;
    size_t cantP_diurno, cantP_nocturno, suma;
    for (int i = 0; i < 7; i++) {
        result = q3Processed(data, &dia, &cantP_diurno, &cantP_nocturno, &suma, i);
        VERIFICA_PROCESADO(result);
        fprintf(query3,"%s;%zu;%zu;%zu\n", dia, cantP_diurno, cantP_nocturno, suma);
    }
    fclose(query3);
    return OK;
}
