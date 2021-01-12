#ifndef PROTOCOLO_ZIGBEE_H
#define PROTOCOLO_ZIGBEE_H

#include <Arduino.h>
#include <inttypes.h>
#include <stdarg.h>

// Estado de configuracion
#define ESTADO_NONE 0
#define ESTADO_VINCULAR 1
#define ESTADO_VINCULANDO 2
#define ESTADO_CONFIGURACION_SENSOR 3

// buttom
#define BUTTON 16
#define LED_BLUE 13
#define RESET_ZIGBEE 4

//-----------------
//Union y Estructura de dispositivos vinvulados
#define NUM_SENSORES_MOVIMIENTO 10
#define TIME_OUT_DES 900000 //---> 15min
typedef struct protocoloZigbeeDevice
{
  unsigned char mac[8];
  unsigned char id[2];
  unsigned char valorMedida;
  unsigned char estadoVinculado;
  unsigned char maquinaEstado;
  unsigned long timeOut;
  
} Estructura_Sensor;

//Funciones

/**
 * @brief Enviar_Serial funcion que envia la cadena de comandos al Zigbee
 * @param m es la cadena de comandos a enviar
 */
void Enviar_Serial(char m, ...);

/**
 * @brief ConfigProtocolo funcion para configurar todo
 */
void ConfigProtocolo();

/**
 * @brief LeerMensaje funcion que lee el mensaje desde el Buffer del Puerto Serial
 */
void LeerMensaje(); 

/**
 * @brief ProcesarCHK funcion para chequear el ChuckSUM del mensaje 
 */
char ProcesarCHK();

/**
 * @brief ProcesarMensaje funcion para procesar el mensaje 
 */
void ProcesarMensaje();

/**
 * @brief comandosVincularNuevoSensor funcion que envia el comando para iniciar una vinculacion
 */
void comandosVincularNuevoSensor();

/**
 * @brief comandosDesvincularSensor funcion para desvincular un sensor en el zigbee
 */
void comandosDesvincularSensor();

/**
 * @brief comandosDesvincularSensorTodos funcion para desvincular todos los sensores en el zigbee
 */
void comandosDesvincularTodosSensores();



/**
 * @brief loopLeerSensoresZigbee ejecuta una secuencia de lectura de los sensores
 */
void loopLeerSensoresZigbee();

/**
 * @brief getMacBridge devuelve el un string la MAC del Puente Zigbee
 */
String getMacBridge();

/**
 * @brief getNumSensorVinvulados devuelve el numero de sensores vinculados
 */
unsigned char getNumSensorVinvulados();

/**
 * @brief blinkLed ejecuta una secuencia de parpade de el led
 * @param mT es el tiempo en ms, en el que parpadea el leg
 * @param mParp es el numero de parpadeos del led
 */
void blinkLed(unsigned int mT, unsigned char mParp);
#endif