#ifndef TPE_FINAL_DATAADT_H
#define TPE_FINAL_DATAADT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

enum ERRORS {OK=0, ARG_INV, NOT_EXIST, NOT_PROCESSED ,NO_MEMORY = ENOMEM};

typedef struct dataCDT * dataADT;

dataADT newData();

int cargarSensor(const size_t id,char* name,char activo, dataADT data);

//procesa los datos de los archivos resolviendo las queries
//int processData(const char* reading, const char* sensor, dataADT* data);

int toBegin(dataADT data);

int hasNext(dataADT data);

size_t getDimQ1 (dataADT data);

void q1Processed (dataADT data,char** name, size_t* cantP_sensorsm, int indice);

void q2Processed (dataADT data, unsigned short* year, size_t* cantPerYear);

//libera la memoria
void freeAll(dataADT data);

#endif //TPE_FINAL_DATAADT_H
