#ifndef COM_ZIGBEE_H
#define COM_ZIGBEE_H

#include <Arduino.h>
#include <inttypes.h>
#include <stdarg.h>
#include "protocoloZigbee.h"

typedef struct com_zigbee
{
  String mac;
  String id;
  unsigned char maquinaEstado;
  unsigned char valorMedida;
}com_zigbee_Sensor_Data;

char Sensor_Leer(char);
char Sensor_CambiarEstado_MaquinaEstado(char, char);
char getNumeroSensor(String _id, String _mac);
unsigned char getEstadoMaquinaSensor(unsigned char _numSensor);
unsigned char getEstadoVinculadoSensor(unsigned char _numSensor);

String getDeviceIdSensor(unsigned char _numSensor);
String getDeviceMacSensor(unsigned char _numSensor);
String getValueSensor(unsigned char _numSensor);

#endif