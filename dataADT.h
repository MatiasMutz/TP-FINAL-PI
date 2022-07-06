#ifndef TPE_FINAL_DATAADT_H
#define TPE_FINAL_DATAADT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct dataCDT * dataADT;

//procesa los datos de los archivos
int processData(const char* reading, const char* sensors, dataADT* data);
void query1(dataADT data);
void query2(dataADT data);
void query3(dataADT data);

//libera la memoria
void freeAll(dataADT data);

#endif //TPE_FINAL_DATAADT_H
