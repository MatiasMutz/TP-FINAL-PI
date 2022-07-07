#ifndef TPE_FINAL_DATAADT_H
#define TPE_FINAL_DATAADT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

enum ERRORS {OK=0, ARG_INV, NOT_EXIST, NOT_PROCESSED ,NO_MEMORY = ENOMEM};

typedef struct dataCDT * dataADT;

//procesa los datos de los archivos
int processData(const char* reading, const char* sensor, dataADT* data);
int query1(dataADT data);
int query2(dataADT data);
int query3(dataADT data);

//libera la memoria
void freeAll(dataADT data);

#endif //TPE_FINAL_DATAADT_H
