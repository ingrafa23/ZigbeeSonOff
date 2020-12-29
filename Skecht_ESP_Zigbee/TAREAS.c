#include <stddef.h>
#include <stdlib.h>

#include "TAREAS.h"

void asignarTareas(TAREA *t, void (*fi)(), void (*fm)(), unsigned char p, unsigned long m)
{
    t->ptr_inicializaTarea = fi;
    t->ptr_tarea_main = fm;
    t->periodo = p;
    t->momentoAnterior = m;
}

void incializaTareas(TAREA *tareas, unsigned long momentoActual, unsigned char numeroTareas) 
{
    for (int x = 0; x < numeroTareas; x++) 
    {
        if (tareas[x].ptr_inicializaTarea != NULL) 
        {
            tareas[x].ptr_inicializaTarea();
            tareas[x].momentoAnterior = momentoActual;
        }
    }
}

void ejecutaTareas(TAREA *tareas, unsigned long momentoActual, unsigned char numeroTareas) 
{
    for (int x = 0;x<numeroTareas;x++)
    {
        if(tareas[x].ptr_tarea_main == NULL)
        	continue;
      
        if (abs((unsigned char)(momentoActual -  tareas[x].momentoAnterior)) >= tareas[x].periodo)
        {
            tareas[x].momentoAnterior =  momentoActual;
            tareas[x].ptr_tarea_main();
        }
    }
}


/**
 * @brief  setMomento asigna el momento o tiempo actual de la ejecucion
 * @param _momento representa el momento actual o el timpo de ejecucion
 */

void setMomento(unsigned long _momento)
{
    
}

/**
 * @brief  getMomento obtiene el momento o tiempo actual de la ejecucion
 * @return retorna el momento actual o el timpo de ejecucion
 */

unsigned long getMomento()
{
    return millis();
}

