/**
  @file TAREAS.h
  @brief controla el funcionamiento en una maquina de estado, en la que se ejecuta en un periodo determinado
  @author Rafael
  @date 10/2020
*/

#ifndef TAREAS_INCLUDE
#define TAREAS_INCLUDE
#include <stdbool.h>



/**
 Define la estructura correspondiente a una tarea del bucle principal.
 Cada tarea se llamara con una temporizacion determinada.
 Las tareas tienes que ser sin bucles y suficientemente seguras
 Para que no se ejecuten, poner a NULL


*/

typedef struct {
    void (* ptr_inicializaTarea )( void ) ; // a este metodo se le llamara cuando antes del bule principal
    void (* ptr_tarea_main  )(void) ; // Tarea a realizar en estado operacinal de la tarjeta, se le pasa el momento en que se razliza
    unsigned long periodo; 
    unsigned long momentoAnterior;

} TAREA;



//Tareas del Core0
#define NUMERO_TAREAS_CORE0 4
extern TAREA *creaTareaAtiendeOta(void);
extern TAREA *creaTareaAtiendeEstadosBooking(void);
extern TAREA * creaTareaSendDataJson(void);
extern TAREA *creaTareaEjecutaCheckQR(void);


//Tareas del Core1
#define NUMERO_TAREAS_CORE1 4

extern TAREA * crearTareaEjecutaInterprete (void);
extern TAREA * creaTareaInterprete (void);
extern TAREA *creaTareaInterpreteComandosJson(void);
extern TAREA *creaTareaAtiendeEstadosBookingAroLed(void);

#endif
