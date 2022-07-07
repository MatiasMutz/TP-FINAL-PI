#ifndef TPE_FINAL_QUERIES_H
#define TPE_FINAL_QUERIES_H
#include "dataADT.h"

void leerSensors(size_t* id, char** name, char** activo, FILE* sensors, char line []);
void leerReadings(unsigned short* year, unsigned short* time, size_t* id, char** day, size_t* people, FILE* readings, char line []);

#endif