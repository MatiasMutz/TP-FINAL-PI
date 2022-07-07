#include "queries.h"
//Carga los datos del query 1 en el archivo csv
int query1(dataADT data){
    if(data == NULL){
        return NOT_PROCESSED;
    }
    FILE* query1 = fopen("query1.csv", "wt");
    fprintf(query1, "sensor;counts\n");
    for (int i = 0; i < data->dimVQ1; i++) {
        fprintf(query1, "%s;%zu\n" , data->VQ1[i].name, data->VQ1[i].cantP_sensor);
    }
    fclose(query1);
    return OK;
}

int query2(dataADT data){
    if(data == NULL){
        return NOT_PROCESSED;
    }
    FILE *query2 = fopen("query2.csv", "wt");
    fprintf(query2, "year;counts\n");
    toBegin(data);
    unsigned short year;
    size_t cantPerYear;
    while(hasNext(data)){
        q2Processed(data, &year, &cantPerYear);
        //fprintf(query2,"%u;%zu\n", data->iterador->anio, data->iterador->cantP_anio);
        fprintf(query2,"%u;%zu\n", year, cantPerYear);
    }
    fclose(query2);
    return OK;
}

//Carga los datos del query 3 en el archivo csv
int query3(dataADT data){
    if(data == NULL){
        return NOT_PROCESSED;
    }
    FILE *query3 = fopen("query3.csv", "wt");
    fprintf(query3, "day;day_counts;night_counts;total_counts\n");
    for (int i = 0; i < 7; i++) {
        fprintf(query3,"%s;%zu;%zu;%zu\n", data->dias[i].dia, data->dias[i].cantP_diurno, data->dias[i].cantP_nocturno, (data->dias[i].cantP_diurno+data->dias[i].cantP_nocturno) );
    }
    fclose(query3);
    return OK;
}