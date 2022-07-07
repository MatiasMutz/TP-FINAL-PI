#include "dataADT.h"

#define BLOCK 20
#define MAX_LINE 1024

typedef struct elemQ1{
    size_t id;
    size_t cantP_sensor;
    char* name;
}elemQ1;

typedef struct elemQ2{
    unsigned short anio;
    size_t cantP_anio;
    struct elemQ2* tail;
    struct elemQ2* iterador;
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


static listQ2 addYearRec(listQ2 l,const unsigned short year,const size_t cantPers,int* flag){
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

static int addYear (dataADT data,const unsigned short year,const size_t cantPers)
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
    //ABRO AMBOS ARCHIVOS
    FILE *sensors = fopen(sensor, "rt");
    if(sensors == NULL)
        return NOT_EXIST;
    FILE *readings = fopen(reading, "rt");
    if(readings == NULL)
        return NOT_EXIST;
    char line[MAX_LINE];
    //LEIDA DE DATOS DE SENSORS
    char* value;
    size_t id;
    char* name;
    char* activo;
    fgets(line, MAX_LINE, sensors); //para saltearme el encabezado
    while(fgets(line, MAX_LINE, sensors)){
        value = strtok(line, ";");
        id = strtoul(value, NULL, 10);
        name = strtok(NULL, ";");
        activo = strtok(NULL, ";");
        //llamar a funcion que se encarga de usarlos
    }
    //LEIDA DE DATOS DE READINGS
    unsigned short year;
    char* day;
    unsigned short time;
    size_t people;
    fgets(line, MAX_LINE, readings); //para saltearme el encabezado
    while(fgets(line, MAX_LINE, readings)){
        //Year	Month	Mdate	Day	Sensor_ID	Time	Hourly_Counts
        value = strtok(line, ";"); //tomo el valor del anio
        year = (unsigned short)(atoi(value)); //lo llevo a que sea un unsig short
        value = strtok(NULL, ";"); // no me importan los meses
        value = strtok(NULL, ";"); // no me importan el dia
        day = strtok(NULL, ";"); //leo el dia
        value = strtok(NULL, ";"); //leo el id
        id = strtoul(value, NULL, 10); //lo paso a unisg long
        value = strtok(NULL, ";");
        time = (unsigned short)(atoi(value)); //lo llevo a que sea un unsig short
        value = strtok(NULL, ";"); //leo la cant personas
        people = strtoul(value, NULL, 10); //lo paso a unisg long
        if(time<6 || time>=18){
            //fue nocturno. Mandar a la funcion que lo procese como nocturno.
        }else{
            //fue diurno. Mandar a la funcion que lo procese como diurno.
        }
    }
    //CIERRO AMBOS ARCHIVOS
    fclose(sensors);
    fclose(readings);
    free(new); //este free no se deberia hacer aca, esta aca para que no tire el sanitaze. Una vez hecho el freeAll lo sacamos.
    return OK;
}

int query1(dataADT data){
    if(data == NULL){
        return NOT_PROCESSED;
    }
    FILE* query1 = fopen("query1.csv", "w");
    fprintf(query1, "sensor;counts\n");
    for (int i = 0; i < data->dimVQ1; ++i) {
        fprintf(query1, data->VQ1[i].name + ";" + data->VQ1[i].cantP_sensor + '\n');
    }
    fclose(query1);
    return OK;
}

int query2(dataADT data){
    if(data == NULL){
        return NOT_PROCESSED;
    }
    FILE *query2 = fopen("query2.csv", "w");
    fprintf(query2, "year;counts\n");
    data->firstQ2->iterador = data->firstQ2;
    while(hasNext(data->firstQ2->iterador)){
        fprintf(query2, data->firstQ2->iterador->anio + ";" + data->firstQ2->iterador->cantP_anio + "\n");
    }
    fclose(query2);
    return OK;
}

static int hasNext(const listQ2 l){
     return l->iterador != NULL;
}

static listQ2 next(listQ2 l){
     assert(hasNext(l)); // chequeo que no tendriamos que hacer en el backend, corregir
     listQ2 aux = l->iterador;
     l->iterador = l->iterador->tail;
     return aux;
}

int query3(dataADT data);

void freeAll(dataADT data);