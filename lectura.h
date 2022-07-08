#ifndef TPE_FINAL_LECTURA_H
#define TPE_FINAL_LECTURA_H
#include "dataADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief dado un string con un formato devuelve en distintos parametros de salida los datos del sensor
 * @param id        es el identificador del sensor
 * @param name      es el nombre del sensor
 * @param activo    parametor de salida que indica si un sensor esta activo o removido
 * @param line      string que contiene todos los datos de un sensor con un formato especifico 
 * 
 */
void leerSensors(size_t* id, char** name, char** activo, char line []);

/**
 * @brief dado un string con un formato devuelve en distintos parametros de salida los datos de una medicion
 * @param year      año en el que se realizo la medicion
 * @param time      hora en la que se realizo la medicion
 * @param id        identificador del sensor
 * @param day       dia en el que se realizo la medicion (ej:Monday,Tuesday,...) 
 * @param people    cantidad de personas que contabilizo el sensor
 * @param line      string que contiene todos los datos de una medicion con un formato especifico 
 * 
 */
void leerReadings(unsigned short* year, unsigned short* time, size_t* id, char** day, size_t* people, char line []);

#endif