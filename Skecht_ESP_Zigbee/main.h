/**
 * @file main.h
 * @brief funciones del main
 * @author Rafael
 * @date 10/2020
*/

#ifndef MAIN_INCLUDE
#define MAIN_INCLUDE
#include <stdbool.h>
#include "TAREAS.h"

#ifndef ESP32 
  #define NULL ((void *)0)
#endif

//funciones que atiende los estados

extern void inicializaEstructuraTareas(TAREA * tareas[]);
extern void incializaTareas(TAREA * tareas[], unsigned long momentoActual);
extern void ejecutaTareasMain(TAREA * tareas[], unsigned long momentoActual);
extern void cambioMomento(TAREA * tareas[], unsigned long momentoActual);
extern void bucleMain(TAREA * tareas[]);
extern void setMomento(unsigned long _momento);
extern unsigned long getMomento();
//--------------------



#endif
