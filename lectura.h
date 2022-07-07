#ifndef TPE_FINAL_LECTURA_H
#define TPE_FINAL_LECTURA_H
#include "dataADT.h"

void leerSensors(size_t* id, char** name, char** activo, char line []);
void leerReadings(unsigned short* year, unsigned short* time, size_t* id, char** day, size_t* people, char line []);

#endif