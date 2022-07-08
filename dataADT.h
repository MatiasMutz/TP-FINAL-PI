#ifndef TPE_FINAL_DATAADT_H
#define TPE_FINAL_DATAADT_H

#define CANT_DIAS 7
#define MAX_LINE 1024

enum ERRORS {OK=0, ARG_INV, NOT_EXIST, NOT_PROCESSED ,NO_MEMORY = ENOMEM, NO_HAY_MAS_ELEMENTO, ERROR_LINE, FINISHED_READING,INCORRECT_FORMAT};

typedef struct dataCDT * dataADT;
/**
 * @brief Crea e inicializa la estructura dataCDT
 * @param dias array de strings que contiene los nombres de los dias, debe estar igual que en el archivo de mediciones
 * @return Devuelve un puntero a la estructura
 */
dataADT newData(char* dias[CANT_DIAS]);
/**
 * @brief Si corresponde, carga los datos del sensor en la estructura dataCDT
 * @param id    es el identificador del sensor
 * @param name  es el nombre del sensor
 * @param data  puntero a la estructura de almacenamiento
 * @return      devuelve un codigo de error
 */
int cargarSensor(const size_t id,char* name,char* activo, dataADT data);

/**
 * @brief procesa la informacion de una medicion
 * @param data      puntero a la estructura de almacenamiento
 * @param id        identificador del sensor
 * @param people    cantidad de personas que contabilizo el sensor
 * @param day       dia en el que se realizo la medicion (ej:Monday,Tuesday,...) 
 * @param year      año en el que se realizo la medicion
 * @param time      hora en la que se realizo la medicion
 * @return      devuelve un codigo de error
 * 
 */
int processLine(dataADT data,size_t id,size_t people,char* day,unsigned short year,unsigned short time);

/**
 * @brief Ordena la informacion de los sensores, segun sus datos
 * @param data puntero a la estructura que tiene los datos almacenados 
 * @return     devuelve un codigo de error
 * 
 */
int ordenarSensors(dataADT data);

/**
 * @brief prepara la estructura para devolver informacion de la query 2
 * @param data  puntero a la estructura de almacenamiento
 * @return      devuelve un codigo de error
 * 
 */
int toBeginQ2(dataADT data);

/**
 * @brief Utiliza un parametro de salida, para devolver la cantidad de sensores almacenados en la estructura
 * @param data  puntero a la estructura de almacenamiento
 * @param dim   parametro de salida donde se almacena la cantidad de sensores almacenados en la estructura
 * @return      devuelve un codigo de error
 * 
 */
int getCantSensores (dataADT data, size_t* dim);

/**
 * @brief extrae los datos para la query 1 utilizando parametros de salida
 * @param data          puntero a la estructura de almacenamiento
 * @param name          parametro de salidad donde se almacena el nombre del sensor
 * @param cantP_sensors parametro de salida donde se almacena la cantidad de personas registradas por el sensor
 * @param indice        debe ser un valor mayor o igual a 0 y menor a la cantidad de sensores
 * @return              devuelve un codigo de error
 * 
 */
int getDataQ1 (dataADT data,char** name, size_t* cantP_sensors, int indice);

/**
 * @brief extrae los datos para la query 2 utilizando parametros de salida
 * @param data          puntero a la estructura de almacenamiento
 * @param year          parametro de salida donde se almacena el año donde ocurrieron las mediciones
 * @param cantPerYear   parametro de salida donde se almacena la cantidad de personas registradas en ese año
 * @return              devuelve un codigo de error
 * 
 */
int getDataQ2 (dataADT data, unsigned short* year, size_t* cantPerYear);

/**
 * @brief extrae los datos para la query 2 utilizando parametros de salida
 * @param data              puntero a la estructura de almacenamiento
 * @param dia               parametro de salida donde se almacena el dia (Monday,...) en el cual ocurrieron las mediciones
 * @param cantP_diurno      parametro de salida donde se almacena la cantidad de personas que se registraron en ese dia entre las 6 y las 17 inclusive
 * @param cantP_nocturno    parametro de salida donde se almacena la cantidad de personas que se registraron en ese dia entre 
 *                          las 0 y las 5 inclusive y entre las 18 inclusive hasta las 0
 * @param suma              parametro de salida donde se almacena la cantidad total de personas registradas en ese dia
 * @param indice            parametro que debe ser entre 0 y 6 inclusive, donde cada numero representa un dia, siendo Monday el 0, el dia siguiente es el numero siguiente
 * @return                  devuelve un codigo de error
 * 
 */
int getDataQ3 (dataADT data, char** dia, size_t* cantP_diurno, size_t* cantP_nocturno, size_t* suma, int indice);

/**
 * @brief libera toda la memoria reservada para el almacenamiento que se encuentra en la estructura
 * @param data  puntero a la estructura de almacenamiento
 * 
 */
void freeAll(dataADT data);

#endif //TPE_FINAL_DATAADT_H
