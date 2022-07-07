#include "dataADT.h"

#define BLOCK 20
#define MAX_LINE 1024
#define DIURNO 0
#define NOCTURNO 1

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
    listQ2 firstQ2;
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
}

//devuelve dim si no esta el sensor, o devuelve la posicion donde esta el vector
static int dondeEsta(const size_t id,elemQ1* sensor,const size_t dim)
{
    int i;
    for(i=0;i<dim && sensor[i].id!=id;i++);
    return i;
}

//carga los sensores en el vector para el query1. Dato extra: Pensado para que primero se fije si el sensor esta activo, y si lo esta se use la funcion
//de momento no revisa si hay id repetidos
//consideracion, se debe achicar el vector una vez que se hayan cargado todos los sensores
static int cargarsensores(const size_t id,const char* name, dataADT data)
{
    int posUltElem=dondeEsta(id,data->VQ1,data->dimVQ1);
    if (posUltElem == data->dimVQ1)
    {
        data->VQ1=realloc(data->VQ1,(sizeof(elemQ1)*BLOCK)+data->dimVQ1);        
    }
    if (errno==ENOMEM)
    {
        return ENOMEM;
    }
    else
    {
        data->VQ1[posUltElem].id=id;
        data->VQ1[posUltElem].name=name;
        data->VQ1[posUltElem].cantP_sensor=0;
        return OK;
    }
}

//carga peatones en el sensor con el mismo id
static void cargarPeatonesQ1(const size_t cantPeatones,const size_t id,elemQ1* sensor,const size_t dim)
{
    int i;
    for ( i=0;i<dim && id!=sensor[i].id;i++);
    if(i<dim)
    {
            sensor[i].cantP_sensor+=cantPeatones;
    }
}

static int compare(elemQ1 elem1,elemQ1 elem2)
{
    int rta;
    if ((rta=elem2.cantP_sensor - elem1.cantP_sensor)!=0)
    {
            return rta;
    }
    else
    {
        return strcmp(elem2.name,elem1.name);
    }
}

//Ordena de forma descendiente por cantidad de persoas y en caso que la cantidad de personas sea igual, alfabeticamente
static void ordenarQ1(elemQ1* VQ1,const size_t dim,int (* compare)(elemQ1 elem1,elemQ1 elem2))
{
    if (VQ1!=NULL)
    qsort(VQ1,sizeof(VQ1[0]),dim,compare);
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


static int agregarPersdia(elemQ3 dias[7],const unsigned short time,const size_t cantPers,const char* dia)
{
    int i;
    for( i=0;i<7 && strcmp(dias[i].dia,dia)!=0;i++);
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

//Procesa la data, lee los archivo y formatea los datos para que las queries esten listas
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
            time=NOCTURNO; //fue nocturno. Mandar a la funcion que lo procese como nocturno.
        }else{
            time=DIURNO; //fue diurno. Mandar a la funcion que lo procese como diurno.
        }
    }
    //CIERRO AMBOS ARCHIVOS
    fclose(sensors);
    fclose(readings);
    free(new); //este free no se deberia hacer aca, esta aca para que no tire el sanitaze. Una vez hecho el freeAll lo sacamos.
    return OK;
}

//Carga los datos del query 1 en el archivo csv
int query1(dataADT data){
    if(data == NULL){
        return NOT_PROCESSED;
    }
    FILE* query1 = fopen("query1.csv", "w");
    fprintf(query1, "sensor;counts\n");
    for (int i = 0; i < data->dimVQ1; ++i) {
        fprintf(query1, "%s;%zu\n" , data->VQ1[i].name, data->VQ1[i].cantP_sensor);
    }
    fclose(query1);
    return OK;
}

//Carga los datos del query 2 en el archivo csv
int query2(dataADT data){
    if(data == NULL){
        return NOT_PROCESSED;
    }
    FILE *query2 = fopen("query2.csv", "w");
    fprintf(query2, "year;counts\n");
    data->firstQ2->iterador = data->firstQ2;
    while(hasNext(data->firstQ2->iterador)){
        fprintf(query2,"%u;%zu\n", data->firstQ2->iterador->anio, data->firstQ2->iterador->cantP_anio);
    }
    fclose(query2);
    return OK;
}

static int hasNext(const listQ2 l){
     return l->iterador != NULL;
}

static listQ2 next(listQ2 l){
     assert(hasNext(l)); //chequeo que no tendriamos que hacer en el backend, corregir
     listQ2 aux = l->iterador;
     l->iterador = l->iterador->tail;
     return aux;
}

//Carga los datos del query 3 en el archivo csv
int query3(dataADT data);

//libera toda la memoria que esta en uso
void freeAll(dataADT data);