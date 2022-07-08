#ifndef TPE_FINAL_LECTURA_H
#define TPE_FINAL_LECTURA_H
#include "dataADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024
#define COLUMNAS_SENSORS 3
#define COLUMNAS_READINGS 7

/**
 * @brief abre un archivo y si existe saltea la primera linea (encabezado) del mismo
 * @param name          path del archivo
 * @param cant_campos   indica la cantidad de campos que debe tener la linea
 * @param flag          indica si la cantidad de campos es incorrecta
 * @return              devuelve el archivo
 */
FILE* openFile(char* name,int cant_campos,int *flag);

/**
 * @brief cuenta la cantidad de campos en una linea (la cantidad de ; mas 1)
 * @param line  linea analizada para contar campos
 * @return      cantidad de campos encontrados
 * 
 */
int verificarColumnas(char* line);

/**
 * @brief dado un string con un formato devuelve en distintos parametros de salida los datos del sensor
 * @param id        es el identificador del sensor
 * @param name      es el nombre del sensor
 * @param activo    parametro de salida que indica si un sensor esta activo o removido
 * @param sensors   archivos sensors
 * @return          devuelve OK (0) si sigue habiendo lineas en el archivo, devuelve FINISHED_READING (!=0) cuando no quedan mas lineas en el archivo
 */
int leerSensors(size_t* id, char** name, char** activo, FILE* sensors);

/**
 * @brief dado un string con un formato devuelve en distintos parametros de salida los datos de una medicion
 * @param year      año en el que se realizo la medicion
 * @param time      hora en la que se realizo la medicion
 * @param id        identificador del sensor
 * @param day       dia en el que se realizo la medicion (ej:Monday,Tuesday,...) 
 * @param people    cantidad de personas que contabilizo el sensor
 * @param readings  archivo readings
 * @return          devuelve OK (0) si sigue habiendo lineas en el archivo, devuelve FINISHED_READING (!=0) cuando no quedan mas lineas en el archivo
 */
int leerReadings(unsigned short* year, unsigned short* time, size_t* id, char** day, size_t* people, FILE* readings);



#endif
