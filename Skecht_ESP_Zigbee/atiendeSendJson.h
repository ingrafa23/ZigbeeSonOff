#ifndef ATIENDE_SEND_JSON_INCLUDE
#define ATIENDE_SEND_JSON_INCLUDE

#include <string.h>
#include <Arduino.h>


extern String dataStringJson;
extern unsigned char sendDataJsonFlag;
extern String sendDataJsonApi;
extern String dataStringRecibeJson;
extern unsigned char dataStringRecibeJsonFlag;

/**
 * @brief sendDataJson asigana un nuevo estadp
 * @param myString es el string json a enviar
 * @param myApi es un String con la direcion de la Api donde se va a realizar la consulta
*/
extern void sendDataJson(String myString, String myApi);

extern unsigned char getAvailabSendDataJoson();

extern String getDataStringRecibeJson();




#endif