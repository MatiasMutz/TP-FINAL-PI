#include "dataADT.h"

#define BLOCK 20
#define MAX_LINE 1024
#define DIURNO 0
#define NOCTURNO 1
#define NO_CARGO 0
#define CARGO 1
#define VERIFICAR_ERRORES(result, sensors, readings) if(result != OK){\
                                                     fclose(sensors);\
                                                     fclose(readings);\
                                                     return result;}
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
static dataADT newData(){
    dataADT new= calloc(1, sizeof(dataCDT));
    new->dias[0].dia="lunes";
    new->dias[1].dia="martes";
    new->dias[2].dia="miercoles";
    new->dias[3].dia="jueves";
    new->dias[4].dia="viernes";
    new->dias[5].dia="sabado";
    new->dias[6].dia="domingo";
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
static int cargarsensores(const size_t id,char* name, dataADT data)
{
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

//carga peatones en el sensor con el mismo id
static int cargarPeatonesQ1(const size_t cantPeatones,const size_t id,elemQ1* sensor,const size_t dim)
{
    int i=dondeEsta(id,sensor,dim);
    if(i<dim)
    {
        sensor[i].cantP_sensor+=cantPeatones;
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

//Ordena de forma descendiente por cantidad de persoas y en caso que la cantidad de personas sea igual, alfabeticamente
static void ordenarQ1(elemQ1* VQ1,const size_t dim,int (* compare)(const void* elem1,const void* elem2))
{
    if (VQ1!=NULL)
        qsort(VQ1,dim,sizeof(elemQ1),compare);
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

static void agregarPersdia(elemQ3 dias[7],const unsigned short time,const size_t cantPers,const char* dia)
{
    int i;
    for(i=0;i<7 && strcmp(dias[i].dia,dia)!=0;i++);
    if (i<7)
    {
        if (time==DIURNO)
        {
            dias[i].cantP_diurno+=cantPers;
        }
        else if(time==NOCTURNO)
        {
            dias[i].cantP_nocturno+=cantPers;
        }
    }
}

static int verificoActivo (size_t id, char* name, char activo, dataADT data){
    enum ERRORS result = OK;
    if(activo == 'A') {
        result = cargarsensores(id, name, data);
    }
    return result;
}

static void leerSensors(size_t* id, char** name, char** activo, FILE* sensors, char line [MAX_LINE]){
    char* value;
    value = strtok(line, ";");
    *id = strtoul(value, NULL, 10);
    *name = strtok(NULL, ";");
    *activo = strtok(NULL, ";");
    return;
}

static void leerReadings(unsigned short* year, unsigned short* time, size_t* id, char** day, size_t* people, FILE* readings, char line [MAX_LINE]){
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
    return;
}

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
    fgets(line, MAX_LINE, readings); //para saltearme el encabezado
    while(fgets(line, MAX_LINE, sensors)){
        leerSensors(&id, &name, &activo, sensors, line);
        result = verificoActivo(id, name, activo[0], *data);
        VERIFICAR_ERRORES(result, sensors, readings)
    }
    (*data)->dimVQ1=(*data)->posNewElem;
    (*data)->VQ1=realloc((*data)->VQ1,sizeof(elemQ1)*(*data)->dimVQ1);

    unsigned short year, time;
    char* day;
    size_t people;
    fgets(line, MAX_LINE, readings); //para saltearme el encabezado
    while(fgets(line, MAX_LINE, readings)){
        leerReadings(&year, &time, &id, &day, &people, readings, line);
        result = cargarPeatonesQ1(people, id, (*data)->VQ1, (*data)->dimVQ1);
        if(result == CARGO){
            result = addYear(*data, year, people);
            VERIFICAR_ERRORES(result, sensors, readings)
            if(time<6 || time>=18){
                time=NOCTURNO; //fue nocturno. Mandar a la funcion que lo procese como nocturno.
            }else{
                time=DIURNO; //fue diurno. Mandar a la funcion que lo procese como diurno.
            }
            agregarPersdia((*data)->dias, time, people, day);
        }
    }

    ordenarQ1((*data)->VQ1, (*data)->dimVQ1, compare);

    //CIERRO AMBOS ARCHIVOS
    fclose(sensors);
    fclose(readings);
    return OK;
}

//Carga los datos del query 1 en el archivo csv
int query1(dataADT data){
    if(data == NULL){
        return NOT_PROCESSED;
    }
    FILE* query1 = fopen("query1.csv", "wt");
    fprintf(query1, "sensor;counts\n");
    for (int i = 0; i < data->dimVQ1; i++) {
        fprintf(query1, "%s;%zu\n" , data->VQ1[i].name, data->VQ1[i].cantP_sensor);
    }
    fclose(query1);
    return OK;
}


static int hasNext(const listQ2 iterador){
    return iterador!=NULL && iterador->tail != NULL;
}

static listQ2 next(listQ2 iterador){
    return iterador->tail;
}

//Carga los datos del query 2 en el archivo csv
int query2(dataADT data){
    if(data == NULL){
        return NOT_PROCESSED;
    }
    FILE *query2 = fopen("query2.csv", "wt");
    fprintf(query2, "year;counts\n");
    data->iterador = data->firstQ2;
    while(hasNext(data->iterador)){
        fprintf(query2,"%u;%zu\n", data->iterador->anio, data->iterador->cantP_anio);
        data->iterador = next(data->iterador);
    }
    fclose(query2);
    return OK;
}

//Carga los datos del query 3 en el archivo csv
int query3(dataADT data){
    if(data == NULL){
        return NOT_PROCESSED;
    }
    FILE *query3 = fopen("query3.csv", "wt");
    fprintf(query3, "day;day_counts;night_counts;total_counts\n");
    for (int i = 0; i < 7; i++) {
        fprintf(query3,"%s;%zu;%zu;%zu\n", data->dias[i].dia, data->dias[i].cantP_diurno, data->dias[i].cantP_nocturno, (data->dias[i].cantP_diurno+data->dias[i].cantP_nocturno) );
    }
    fclose(query3);
    return OK;
}

static void freeRec(listQ2 l){
    if(l == NULL){
        return;
    }
    listQ2 aux = l->tail;
    free(l);
    freeRec(aux);
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
