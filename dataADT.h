#ifndef TPE_FINAL_DATAADT_H
#define TPE_FINAL_DATAADT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

enum ERRORS {OK=0, ARG_INV, NOT_EXIST, NOT_PROCESSED ,NO_MEMORY = ENOMEM};

typedef struct dataCDT * dataADT;

//procesa los datos de los archivos resolviendo las queries
int processData(const char* reading, const char* sensor, dataADT* data);

/*
 * deja en un csv los resultados de la query1.
 * devuelve OK (0) si no hubo errores y se genero el csv correctamente.
 * devuelve NOT_PROCESSED si se la llama sin procesar los datos.
 */
int query1(dataADT data);

/*
 * deja en un csv los resultados de la query2.
 * devuelve OK (0) si no hubo errores y se genero el csv correctamente.
 * devuelve NOT_PROCESSED si se la llama sin procesar los datos.
 */
int query2(dataADT data);

/*
 * deja en un csv los resultados de la query3.
 * devuelve OK (0) si no hubo errores y se genero el csv correctamente.
 * devuelve NOT_PROCESSED si se la llama sin procesar los datos.
 */
int query3(dataADT data);

//libera la memoria
void freeAll(dataADT data);

#endif //TPE_FINAL_DATAADT_H
