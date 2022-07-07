#include "dataADT.h"
#define BLOCK 20
#define MAX_LINE 1024
#define DIURNO 0
#define NOCTURNO 1
#define NO_CARGO 0
#define CARGO 1

typedef struct elemQ1{
    size_t id;
    size_t cantP_sensor;
    char* name;
}elemQ1;

typedef struct elemQ2{
    unsigned short anio;
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
    size_t posNewElem;
    listQ2 firstQ2;
    listQ2 iterador;
    elemQ3 dias[7];
}dataCDT;

//crea la estructura que almacena todos los datos utiles
dataADT newData(){
    dataADT new= calloc(1, sizeof(dataCDT));
    new->dias[0].dia="Monday";
    new->dias[1].dia="Tuesday";
    new->dias[2].dia="Wednesday";
    new->dias[3].dia="Thursday";
    new->dias[4].dia="Friday";
    new->dias[5].dia="Saturday";
    new->dias[6].dia="Sunday";
    return new;
}

//devuelve dim si no esta el sensor, o devuelve la posicion donde esta el vector, en dim tiene que estar la poscion del ultimo elemento para cargar los sensores
static int dondeEsta(const size_t id,elemQ1* sensor,const size_t posNewElem)
{
    size_t i;
    for(i=0;i<posNewElem && sensor[i].id!=id;i++);
    return i;
}

//carga los sensores en el vector para el query1. Dato extra: Pensado para que primero se fije si el sensor esta activo, y si lo esta se use la funcion
//de momento no revisa si hay id repetidos
//consideracion, se debe achicar el vector una vez que se hayan cargado todos los sensores
static int cargarActivos(const size_t id,char* name, dataADT data)
{
    if (data!=NULL)
    {
        return NOT_PROCESSED;
    }
    if (data->posNewElem==data->dimVQ1)
    {
        data->VQ1=realloc(data->VQ1,sizeof(elemQ1)*(BLOCK+data->dimVQ1));
        data->dimVQ1+=BLOCK;
        for(int i=data->posNewElem;i<data->dimVQ1;i++)
        {
            data->VQ1[i].id=0;
        }
    }
    int posElem=dondeEsta(id,data->VQ1,data->posNewElem);
    if (posElem==data->posNewElem)
    {
        if (errno==ENOMEM)
        {
            return ENOMEM;
        }
        else
        {
            data->VQ1[posElem].id=id;
            data->VQ1[posElem].name = malloc(strlen(name)+1);
            strcpy(data->VQ1[posElem].name, name);
            data->VQ1[posElem].cantP_sensor=0; //puede estar en 0 porque hago realloc
            data->posNewElem++;
        }
    }
    else{
        errno=0;
    }
    return OK;
}

int cargarSensor (size_t id, char* name, char activo, dataADT data){
    if (data==NULL)
    {
        return NOT_PROCESSED;
    }
    enum ERRORS result = OK;
    if(activo == 'A') {
        result = cargarActivos(id, name, data);
    }
    return result;
}

//carga peatones en el sensor con el mismo id
static int cargarPeatonesQ1(const size_t cantPeatones,const size_t id, dataADT data)
{
    int i=dondeEsta(id,data->VQ1,data->dimVQ1);
    if(i<data->dimVQ1)
    {
        data->VQ1[i].cantP_sensor+=cantPeatones;
        return CARGO;
    }
    return NO_CARGO;
}

static int compare(const void* elem1,const void* elem2)
{
    elemQ1 Aelem1= *(elemQ1*) elem1;
    elemQ1 Belem2= *(elemQ1*) elem2;
    int rta = Belem2.cantP_sensor - Aelem1.cantP_sensor;
    if (rta!=0)
    {
        return rta;
    }
    else
    {
        return strcmp(Aelem1.name,Belem2.name);
    }
}


static int ajusteRealloc (dataADT data){
    data->dimVQ1=data->posNewElem;
    data->VQ1=realloc(data->VQ1,sizeof(elemQ1)*data->dimVQ1);
    return OK;
}

//Ordena de forma descendiente por cantidad de persoas y en caso que la cantidad de personas sea igual, alfabeticamente
void ordenarSensors(dataADT data)
{
    if(data->VQ1!=NULL)
        ajusteRealloc(data);
        qsort(data->VQ1, data->dimVQ1, sizeof(elemQ1), compare);
}

//Agrega un año si no esta en la lista para el query 2 o le agrega la cantidad de personas de la medicion para ese año
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
    }
    else{
        l->tail = addYearRec(l->tail, year, cantPers,flag);
    }
    return l;
}

//devuelve si hubo error de memoria
static int addYear (dataADT data,const unsigned short year,const size_t cantPers)
{
    int flag=0;
    data->firstQ2=addYearRec(data->firstQ2,year,cantPers,&flag);
    return flag;
}

static void agregarPersdia(dataADT data,const unsigned short time,const size_t cantPers,const char* dia)
{
    int i;
    for(i=0;i<7 && strcmp(data->dias[i].dia,dia)!=0;i++);
    
    if (i<7)
    {
        if (time==DIURNO)
        {
            data->dias[i].cantP_diurno+=cantPers;
        }
        else if(time==NOCTURNO)
        {
            data->dias[i].cantP_nocturno+=cantPers;
        }
    }
}

static int diurno_O_nocturno (unsigned short time){
    if(time<6 || time>=18)
        return NOCTURNO; //fue nocturno. Mandar a la funcion que lo procese como nocturno.
    else
        return DIURNO; //fue diurno. Mandar a la funcion que lo procese como diurno.
}

int processLine(dataADT data,size_t id,size_t people,char* name,char activo,char* day,unsigned short year,unsigned short time)
{
    enum ERRORS result = OK;
    result = cargarPeatonesQ1(people, id, data);
    if(result == CARGO){
        result = addYear(data, year, people);
        if(result != OK)
            return result;
        time = diurno_O_nocturno(time);
        agregarPersdia(data, time, people, day);
    }
}
processAllData(){

}
/*
//Procesa la data, lee los archivo y formatea los datos para que las queries esten listas
int processData(const char* sensor, const char* reading, dataADT* data){

errno = 0;
int result = OK;
*data = newData();
if(errno == ENOMEM){
    return ENOMEM;
}
//ABRO AMBOS ARCHIVOS
FILE *sensors = fopen(sensor, "rt");
if(sensors == NULL)
    return NOT_EXIST;
FILE *readings = fopen(reading, "rt");
if(readings == NULL){
    fclose(sensors);
    return NOT_EXIST;
}

char line [MAX_LINE];
size_t id;
char* name;
char* activo;

(*data)->dimVQ1=(*data)->posNewElem;
(*data)->VQ1=realloc((*data)->VQ1,sizeof(elemQ1)*(*data)->dimVQ1);

unsigned short year, time;
char* day;
size_t people;


ordenarQ1((*data)->VQ1, (*data)->dimVQ1, compare);

//CIERRO AMBOS ARCHIVOS
fclose(sensors);
fclose(readings);
return OK;
}
*/

int toBegin(dataADT data)
{
    if (data==NULL)
    {
        return NOT_PROCESSED;
    }
    data->iterador=data->firstQ2;
    return OK;
}

int hasNext(dataADT data){
    return data!=NULL && data->iterador!=NULL;
}


size_t getCantSensores (dataADT data){
    return data->dimVQ1;
}

//devuelve los parametros de la q1
int q1Processed (dataADT data,char** name, size_t* cantP_sensors, int indice){
    if(data == NULL)
        return NOT_PROCESSED;
    *name = data->VQ1[indice].name;
    *cantP_sensors = data->VQ1[indice].cantP_sensor;
    return OK;
}

//devuelve los parametros de la q2 y pasa al siguiente elemento
int q2Processed (dataADT data, unsigned short* year, size_t* cantPerYear){
    if(data == NULL)
        return NOT_PROCESSED;
    *year = data->iterador->anio;
    *cantPerYear = data->iterador->cantP_anio;
    if(hasNext(data))
    {
        next(data);
        return OK;
    }
    else
    {
        return NO_HAY_MAS_ELEMENTO;
    }
}

//devuelve los parametros de la q3 y pasa al siguiente elemento
int q3Processed (dataADT data, char** dia, size_t* cantP_diurno, size_t* cantP_nocturno, size_t* suma, int indice){
    if(data == NULL)
        return NOT_PROCESSED;
    *dia = data->dias[indice].dia;
    *cantP_diurno = data->dias[indice].cantP_diurno;
    *cantP_nocturno = data->dias[indice].cantP_nocturno;
    *suma = data->dias[indice].cantP_diurno + data->dias[indice].cantP_nocturno;
    return OK;
}

static void freeRec(listQ2 l){
    if(l == NULL){
        return;
    }
    freeRec(l->tail);
    free(l);
}

//libera toda la memoria que esta en uso
void freeAll(dataADT data){
    freeRec(data->firstQ2);
    for (int i = 0; i < data->dimVQ1; ++i) {
        free(data->VQ1[i].name);
    }
    free(data->VQ1);
    free(data);
}