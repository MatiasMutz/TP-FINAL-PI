#ifndef TPE_FINAL_QUERIES_H
#define TPE_FINAL_QUERIES_H
#include "dataADT.h"

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

#endif
