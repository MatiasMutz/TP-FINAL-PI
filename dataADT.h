#ifndef TPE_FINAL_DATAADT_H
#define TPE_FINAL_DATAADT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

enum ERRORS {OK=0, ARG_INV, NOT_EXIST, NOT_PROCESSED ,NO_MEMORY = ENOMEM,NO_HAY_MAS_ELEMENTO};

typedef struct dataCDT * dataADT;

dataADT newData();

int cargarSensor(const size_t id,char* name,char* activo, dataADT data);

int ordenarSensors(dataADT data);

int processLine(dataADT data,size_t id,size_t people,char* name,char* day,unsigned short year,unsigned short time);

int toBegin(dataADT data);

int hasNext(dataADT data);

size_t getCantSensores (dataADT data);

int q1Processed (dataADT data,char** name, size_t* cantP_sensors, int indice);

int q2Processed (dataADT data, unsigned short* year, size_t* cantPerYear);

int q3Processed (dataADT data, char** dia, size_t* cantP_diurno, size_t* cantP_nocturno, size_t* suma, int indice);

//libera la memoria
void freeAll(dataADT data);

#endif //TPE_FINAL_DATAADT_H
