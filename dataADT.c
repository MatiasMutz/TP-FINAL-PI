#include "dataADT.h"



#define BLOCK 20
#define DIURNO 0
#define NOCTURNO 1
#define NO_CARGO 0
#define CARGO 1
#define CHECK_ERRNO if(errno==ENOMEM){return ENOMEM;}

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
    elemQ3 dias[CANT_DIAS];
}dataCDT;

dataADT newData(char* dias[CANT_DIAS]){
    dataADT new= calloc(1, sizeof(dataCDT));
    if(errno==ENOMEM)
    {
        return NULL;
    }
    for(int i=0;i<CANT_DIAS;i++)
    {
        new->dias[i].dia=dias[i];
    }
    return new;
}

/**
 * @brief busca si el sensor esta en el array y su posicion
 * @param id            es el identificador del sensor
 * @param sensor        array que contiene todos los sensores cargados
 * @param posNewElem    posicion donde se debe almacenar un nuevo sensor al array
 * @return              devuelve la posicion donde esta almacenado el sensor dentro del array si es que esta repetido o devuelve la posicion en el array donde 
 *                      se debe almacenar si no esta almacenado
 * 
 */
static int dondeEsta(const size_t id,elemQ1* sensor,const size_t posNewElem)
{
    size_t i;
    for(i=0;i<posNewElem && sensor[i].id!=id;i++);
    return i;
}


/**
 * @brief carga los datos del sensor en la estructura
 * @param id    es el identificador del sensor
 * @param name  es el nombre del sensor
 * @param data  puntero a la estructura de almacenamiento
 * @return      devuelve un elemento del codigo de errores
 * 
 */
static int cargarActivos(const size_t id,char* name, dataADT data)
{
    
    int posElem=dondeEsta(id,data->VQ1,data->posNewElem);
    if (posElem==data->posNewElem) //Entra si no esta repetido
    {
        if (data->posNewElem==data->dimVQ1)
        {
            data->VQ1=realloc(data->VQ1,sizeof(elemQ1)*(BLOCK+data->dimVQ1));
            CHECK_ERRNO
            data->dimVQ1+=BLOCK;
        }
        data->VQ1[posElem].id=id;
        data->VQ1[posElem].name = malloc(strlen(name)+1);
        CHECK_ERRNO
        strcpy(data->VQ1[posElem].name, name);
        data->VQ1[posElem].cantP_sensor=0; //tengo que ponerlo en 0 porque puede haber basura
        data->posNewElem++;
    }
    return OK;
}

int cargarSensor(size_t id, char* name, char* activo, dataADT data){
    if (data==NULL)
    {
        return NOT_PROCESSED;
    }
    enum ERRORS result = OK;
    if(activo[0] == 'A') {
        result = cargarActivos(id, name, data);
    }
    return result;
}

/**
 * @brief carga datos de las mediciones en los elemento que tenga el mismo id y se encuentre almacenado en la estructura
 * @param cantPeatones  cantidad de peatones de la medicion
 * @param id            identificador del sensor
 * @param data          puntero a la estructura de almacenamiento
 * @return              devuelve un elemento del codigo de errores
 * 
 */
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

/**
 * @brief compara dos sensores
 *        Lo utilizamos para ordenar los sensores por dos criterios:
 *          1) cantidad de personas registradas por sensor descendiente
 *          2) alfabeticamente (cuando la cantidad de personas sea la misma)
 * @param elem1 son los elementos a compara
 * @param elem2
 * @return      devuelve un numero distinto de 0
 * 
 */
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

/**
 * @brief ajusta el tamaño del vector que contiene los sensores, ya que como se agregan bloques, puede sobrar memoria
 * @param data          puntero a la estructura de almacenamiento
 */
static int ajusteRealloc (dataADT data){
    data->dimVQ1=data->posNewElem;
    data->VQ1=realloc(data->VQ1,sizeof(elemQ1)*data->dimVQ1);
    CHECK_ERRNO
    return OK;
}


int ordenarSensors(dataADT data)
{
    if(data==NULL)
        return NOT_PROCESSED;
    if(data->VQ1!=NULL){
        int flagMem=OK;
        flagMem=ajusteRealloc(data);
        if (flagMem!=OK)
        {
            return NO_MEMORY;
        }
        qsort(data->VQ1, data->dimVQ1, sizeof(elemQ1), compare);
    }
    return OK;
}


/**
 * @brief Agrega un año si no esta en la lista para el query 2 o le agrega la cantidad de personas de la medicion para ese año
 * @param l     nodo de la lista de elementos de la query 2
 * @param year      año en el que se realizo la medicion
 * @param cantPers  cantidad de personas que contabilizo el sensor en la medicion
 * @param flag      parametro de salida que almacena un codigo de error
 * @return          devuelve la direccion a un nodo de la lista
 * 
 */
static listQ2 addYearRec(listQ2 l,const unsigned short year,const size_t cantPers,int* flag){
    if(l == NULL || year > l->anio){
        listQ2 aux = malloc(sizeof(elemQ2));
        if (errno==ENOMEM)
        {
            *flag=ENOMEM;
        }
        else
        {
            aux->anio = year;
            aux->cantP_anio = cantPers;
            aux->tail=l;
            return aux;
        }
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
/**
 * @brief agrega informacion a la lista con la informacion de query2
 * @param data      puntero a la estructura de almacenamiento
 * @param year      año en el que se realizo la medicion
 * @param cantPers  cantidad de personas que contabilizo el sensor en la medicion
 * @return          devuelve un elemento del codigo de errores
 * 
 */
static int addYear (dataADT data,const unsigned short year,const size_t cantPers)
{
    int flag=OK;
    data->firstQ2=addYearRec(data->firstQ2,year,cantPers,&flag);
    return flag;
}

/**
 * @brief agrega la informacion de la medicion al dia correspondiente
 * @param data      puntero a la estructura de almacenamiento
 * @param time      hora en la que se realizo la medicion
 * @param cantPers  cantidad de personas que contabilizo el sensor en la medicion
 * @param dia       dia en el que se realizo la medicion (ej:Monday,Tuesday,...) 
 * 
 */
static void agregarPersdia(dataADT data,const unsigned short time,const size_t cantPers,const char* dia)
{
    int i;
    for(i=0;i<CANT_DIAS && strcmp(data->dias[i].dia,dia)!=0;i++);

    if (i<CANT_DIAS)
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

/**
 * @brief determina si un horario es diurno o nocturno.
 *        Es diurno si la medicion se realizo entre las 6 y 17 inclusive
 *        Es nocturno si la medicion se realizo entre las 0 y 5 inclusive o entre las 18 y 0 inclusive
 * @param time hora en la que se realizo la medicion
 * @return devuelve DIURNO si es un horario diurno o NOCTURNO si es un horario nocturno
 * 
 */
static int diurno_O_nocturno (unsigned short time){
    if(time<6 || time>=18)
        return NOCTURNO; //fue nocturno. Mandar a la funcion que lo procese como nocturno.
    else
        return DIURNO; //fue diurno. Mandar a la funcion que lo procese como diurno.
}


int processLine(dataADT data,size_t id,size_t people,char* day,unsigned short year,unsigned short time)
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
    return OK;
}

int toBeginQ2(dataADT data)
{
    if (data==NULL)
    {
        return NOT_PROCESSED;
    }
    data->iterador=data->firstQ2;
    return OK;
}

/**
 * @brief revisa si la estructura no es NULL y si el iterador de la estructura no es NULL
 * @param data      puntero a la estructura de almacenamiento
 * @return 1 si cumple o 0 si no cumple
 * 
 */
static int hasNext(dataADT data){
    return data!=NULL && data->iterador!=NULL;
}

/**
 * @brief guarda en los parametros de salida la informacion y devuelve la direccion del proximo elemento de la lista
 * @param list        nodo de la lista
 * @param year        parametro de salida donde se almacena el año en el que se realizaron las mediciones
 * @param cantPerYear parametro de salida donde se almacena las cantidad de personas que se contabilizaron en ese año
 * @return            devuelve la direccion del nodo
 * 
 */
static listQ2 next(listQ2 list, unsigned short* year, size_t* cantPerYear){
    *year = list->anio;
    *cantPerYear = list->cantP_anio;
    return list->tail;
}

int getCantSensores (dataADT data, size_t* dim){
    if(data == NULL)
        return NOT_PROCESSED;
    *dim = data->dimVQ1;
    return OK;
}

int getDataQ1 (dataADT data,char** name, size_t* cantP_sensors, int indice){
    if(data == NULL)
        return NOT_PROCESSED;
    *name = data->VQ1[indice].name;
    *cantP_sensors = data->VQ1[indice].cantP_sensor;
    return OK;
}

int getDataQ2 (dataADT data, unsigned short* year, size_t* cantPerYear){
    if(data == NULL)
        return NOT_PROCESSED;
    if(hasNext(data))
    {
        data->iterador = next(data->iterador, year, cantPerYear);
        return OK;
    }
    else
    {
        return NO_HAY_MAS_ELEMENTO;
    }
}

int getDataQ3 (dataADT data, char** dia, size_t* cantP_diurno, size_t* cantP_nocturno, size_t* suma, int indice){
    if(data == NULL)
        return NOT_PROCESSED;
    *dia = data->dias[indice].dia;
    *cantP_diurno = data->dias[indice].cantP_diurno;
    *cantP_nocturno = data->dias[indice].cantP_nocturno;
    *suma = data->dias[indice].cantP_diurno + data->dias[indice].cantP_nocturno;
    return OK;
}

/**
 * @brief libera todos los espacios de memoria reservados en el heap usado por los nodos de la lista
 * @param l puntero a un nodo de una lista
 * 
 */
static void freeRec(listQ2 l){
    if(l == NULL){
        return;
    }
    freeRec(l->tail);
    free(l);
}

void freeAll(dataADT data){
    if(data!=NULL){
        freeRec(data->firstQ2);
        for (int i = 0; i < data->dimVQ1; ++i) {
            free(data->VQ1[i].name);
        }
        free(data->VQ1);
        free(data);
    }
}