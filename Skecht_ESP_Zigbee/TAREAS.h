/**
  @file TAREAS.h
  @brief controla el funcionamiento en una maquina de estado, en la que se ejecuta en un periodo determinado
  @author Rafael
  @date 10/2020
*/

#ifndef TAREAS_INCLUDE
#define TAREAS_INCLUDE
#include <stdbool.h>
#include <Arduino.h>


/**
 Define la estructura correspondiente a una tarea del bucle principal.
 Cada tarea se llamara con una temporizacion determinada.
 Las tareas tienes que ser sin bucles y suficientemente seguras
 Para que no se ejecuten, poner a NULL


*/

typedef struct
{
    void (*ptr_inicializaTarea)(void); 	
    void (*ptr_tarea_main)(void); 	
    unsigned long periodo; 
    unsigned long momentoAnterior;
} TAREA;

void asignarTareas(TAREA *t, void (*fi)(), void (*fm)(), unsigned long p, unsigned long m);
void incializaTareas(TAREA *tareas, unsigned long momentoActual, unsigned char numeroTareas); 
void ejecutaTareas(TAREA *tareas, unsigned long momentoActual, unsigned char numeroTareas);
void setMomento(unsigned long _momento);
unsigned long getMomento();

#endif
