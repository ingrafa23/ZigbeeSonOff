#ifndef ESTADO_BOOBKING_INCLUDE
#define ESTADO_BOOBKING_INCLUDE

#include <string.h>
#include <Arduino.h>


#define ESTADO_UNREGISTER 0
#define ESTADO_REGISTER 1
#define ESTADO_RESERVED 2
#define ESTADO_BUSY 3

extern unsigned char estadoBooking;
void setEstadoBooking(unsigned char mEstado, unsigned char _numSensor);
unsigned char getEstadoBooking();
void mainEstadosBooking();


#endif