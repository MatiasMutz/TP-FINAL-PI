#include "dataADT.h"

typedef struct elemQ1{
    size_t id;
    size_t cantP_sensor;
    char* name;
    struct elemQ1* tail;
}elemQ1;

typedef struct elemQ2{
    unsigned int anio;
    size_t cantP_anio;
    struct elemQ2* tail;
}elemQ2;

typedef struct elemQ3{
    char* dia;
    size_t cantP_diurno;
    size_t cantP_nocturno;
}elemQ3;

typedef struct dataCDT{
    elemQ1* firstQ1;
    elemQ2* firstQ2;
    elemQ3 dias[7];
}dataCDT;

static dataADT newData(){
    return calloc(1, sizeof(dataCDT));
}

static void addYear(listQ2 l, unsigned int year, size_t cantPers){
    if(l == NULL || year > l->anio){
        listQ2 aux = malloc(sizeof(listQ2));
        aux->anio = year;
        aux->cantP_anio = cantPers;
        return aux;
    }
    else if(year == l->anio){
        l->cantP_anio += cantPers;
        return l;
    }
    else{
        l->tail = addYear(l->tail, year, cantPers);
    }
    return l;
}

int processData(const char* sensor, const char* reading, dataADT* data){
    errno = 0;
    dataADT new = newData();
    if(errno == ENOMEM){
        return ENOMEM;
    }
    FILE *sensors = fopen(sensor, "rt");
    char line[MAX_LINE];
    char* value;
    char* name;
    char* activo;
    size_t id;
    fgets(line, MAX_LINE, sensors);
    if(sensors == NULL)
        return NOT_EXIST;
    while(fgets(line, MAX_LINE, sensors)){
        value = strtok(line, ";");
        id = strtoul(value, NULL, 10);
        printf("%zu\t", id);
        name = strtok(NULL, ";");
        printf("%s\t\t", name);
        activo = strtok(NULL, ";");
        printf("%s", activo);
        //llamar a funcion que se encarga de usarlos
    }
    FILE *readings = fopen(reading, "rt");
    if(readings == NULL)
        return NOT_EXIST;
    fclose(sensors);
    fclose(readings);
    free(new);
    return 1;
}

void query1(dataADT data);

void query2(dataADT data);

void query3(dataADT data);

void freeAll(dataADT data);