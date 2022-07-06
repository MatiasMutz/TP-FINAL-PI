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
 
int processData(const char* reading, const char* sensors){
    errno = 0;
    dataADT new = newData();
    if(errno == ENOMEM){
        return ENOMEM;
    }
}

void freeAll(dataADT data);