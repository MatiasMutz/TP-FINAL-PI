#include "lectura.h"

#define NOT_EXPECTED_LINE   if(line == NULL || *line == 0)\
                                return ERROR_LINE;

FILE* openFile(char* name){
    char line[MAX_LINE];
    FILE *file = fopen(name, "rt");
    if(file != NULL)
        fgets(line, MAX_LINE, file); //para saltearme el encabezado
    return file;
}


int leerSensors(size_t* id, char** name, char** activo, FILE* sensors){
    char line[MAX_LINE];
    if(fgets(line, MAX_LINE, sensors)){
        char* value;
        value = strtok(line, ";");
        *id = strtoul(value, NULL, 10);
        *name = strtok(NULL, ";");
        *activo = strtok(NULL, ";");
        return OK;
    }
    return FINISHED_READING;
}

int leerReadings(unsigned short* year, unsigned short* time, size_t* id, char** day, size_t* people, FILE* readings){
    char line[MAX_LINE];
    if(fgets(line, MAX_LINE, readings)){
        char* value;
        value = strtok(line, ";"); //tomo el valor del anio
        *year = (unsigned short)(atoi(value)); //lo llevo a que sea un unsig short
        value = strtok(NULL, ";"); // no me importan los meses
        value = strtok(NULL, ";"); // no me importa la fecha
        *day = strtok(NULL, ";"); //leo el dia
        value = strtok(NULL, ";"); //leo el id
        *id = strtoul(value, NULL, 10); //lo paso a unisg long
        value = strtok(NULL, ";");
        *time = (unsigned short)(atoi(value)); //lo llevo a que sea un unsig short
        value = strtok(NULL, ";"); //leo la cant personas
        *people = strtoul(value, NULL, 10); //lo paso a unisg long
        return OK;
    }
    return FINISHED_READING;
}

