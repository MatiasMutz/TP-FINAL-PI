#include "dataADT.h"

#define BLOCK 20
#define MAX_LINE 1024

typedef struct elemQ1{
    size_t id;
    size_t cantP_sensor;
    char* name;
}elemQ1;

typedef struct elemQ2{
    unsigned int anio;
    size_t cantP_anio;
    struct elemQ2* tail;
}elemQ2;

typedef elemQ2* listQ2;

typedef struct elemQ3{
    char* dia;
    size_t cantP_diurno;
    size_t cantP_nocturno;
}elemQ3;

typedef struct dataCDT{
    elemQ1* VQ1;
    size_t dimVQ1;
    size_t posUltElem;
    listQ2 firstQ2;
    elemQ3 dias[7];
}dataCDT;

static dataADT newData(){
    return calloc(1, sizeof(dataCDT));
}

/*
static int cargarsensoresActivosRec(size_t id,char* name, listQ1 nodo,int* flag)
{
    int c;
    if (nodo==NULL || (c=strcmp(nodo->name,name))>0)
    {
        listQ1 new=malloc(sizeof(elemQ1));
        if (errno==ENOMEM)
        {
            *flag=ENOMEM;
        }
        new->id=id;
        new->name=name;
        new->tail=nodo;
        return new;
    }
    else if (c<0)
    {
        nodo->tail=cargarsensorActivosRec(id,name,nodo->tail,flag);
    }
    return nodo;
}

static int cargarsensor (size_t id,char* name,dataADT data)
{
    int flag=0;
    if (data->firstQ1==NULL)
    {
        malloc(sizeof())
    }
    return flag;
}
*/

static int cargarsensores(const size_t id,const char* name, dataADT data)
{
    if (data->posUltElem == data->dimVQ1)
    {
        data->VQ1=realloc(data->VQ1,(sizeof(elemQ1)*BLOCK)+data->dimVQ1);        
    }
    if (errno==ENOMEM)
    {
        return ENOMEM;
    }
    else
    {
    
        data->VQ1[data->posUltElem].id=id;
        data->VQ1[data->posUltElem].name=name;
        data->VQ1[data->posUltElem].cantP_sensor=0;
        data->posUltElem++;
        return OK;
    }
}

static int cargarPeatonesQ1(const size_t cantPeatones,const size_t id,dataADT data)
{
    
}


static listQ2 addYearRec(listQ2 l,const unsigned int year,const size_t cantPers,int* flag){
    if(l == NULL || year > l->anio){
        listQ2 aux = malloc(sizeof(elemQ2));
        if (errno==ENOMEM)
        {
            *flag=ENOMEM;
        }
        aux->anio = year;
        aux->cantP_anio = cantPers;
        aux->tail=l;
        return aux;
    }
    else if(year == l->anio){
        l->cantP_anio += cantPers;
        return l;
    }
    else{
        l->tail = addYearRec(l->tail, year, cantPers,flag);
    }
    return l;
}

static int addYear (dataADT data,const unsigned int year,const size_t cantPers)
{
    int flag=0;
    data->firstQ2=addYearRec(data->firstQ2,year,cantPers,&flag);
    return flag;
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
    return OK;
}

void query1(dataADT data){
    FILE* query1 = fopen("query1.csv", "w");
    fprintf(query1, "sensor;counts\n");
    for (int i = 0; i < data->dimVQ1; ++i) {
        fprintf(query1, data->VQ1[i].name + ";" + data->VQ1[i].cantP_sensor + '\n');
    }
    fclose(query1);
}

void query2(dataADT data);

void query3(dataADT data);

void freeAll(dataADT data);