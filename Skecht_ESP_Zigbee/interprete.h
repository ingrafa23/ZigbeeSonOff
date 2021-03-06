/**
  @file interprete.h
  @brief Interpreta todo los comandos de control del dispositivo
  @author Rafael
  @date 10/2020
*/

#ifndef INTERPRETE_INCLUDE
#define INTERPRETE_INCLUDE

#include <stdio.h>
#include <string.h>
#include <Arduino.h>

// ORDEN es una estructura que organiza las intrucciones a interpretar
//usamos una estructura para que sea flexible si en su momento require incluir valorea adicionels
typedef struct ORDEN 
{
  unsigned char numeroInstruccion;
} Orden;

#define MAX_BUFFER_INTERPRTE  16

extern Orden bufferOrdenesInterprete[MAX_BUFFER_INTERPRTE];

//funcion que trata e interpreta los mensajes
void trataMensaje (char * mensajeEntrante) ;

//funcion que compara dos string
char strEqual (char *a, char *b) ;

//funcion que llena el buffer 
void fillBuffer(unsigned int command);

//funcion que verifica si hay dato pata llenar el buffer
char getcomandBuffer(unsigned int *command);

//funcion para interpretar las instrucciones
void interpreteIntrucciones(char *commandIntrucciones, char _numSensor);

void interpreteEjecuta();

#endif