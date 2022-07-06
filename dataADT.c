 #include "dataADT.h"

typedef struct elemQ1{
    size_t id;
    size_t cantP_sensor;
    char* name;
    struct elemQ1* tail;
}elemQ1;

typedef elemQ1* listQ1;
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
    listQ1 firstQ1;
    listQ2 firstQ2;
    elemQ3 dias[7];
}dataCDT;

static dataADT newData(){
    return calloc(1,sizeof(dataCDT));
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
    data->firstQ1=cargarsensorActivosRec(id,name,data->firstQ1,&flag);
    return flag;
}

static int cargarPeatonesQ1(size_t cantPeatones,size_t id,dataADT data)
{
    
}


int processData(const char* reading, const char* sensor, dataADT* data){
    errno = 0;
    dataADT new = newData();
    if(errno == ENOMEM){
        return ENOMEM;
    }
    FILE *sensors = fopen(sensor, "rt");
    if(sensors == NULL)
        return NOT_EXIST;

    FILE *readings = fopen(reading, "rt");
    if(readings == NULL)
        return NOT_EXIST;
}

void query1(dataADT data);

void query2(dataADT data);

void query3(dataADT data);

void freeAll(dataADT data);