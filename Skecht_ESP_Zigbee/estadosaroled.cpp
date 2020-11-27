#include "estadosaroled.h"
#include "estadobooking.h"
#include "TAREAS.h"
#include "ledRGB.h"
ledRGB ringLedRGB;


 unsigned char estadoAroLed;
 unsigned char isEstadoCambioAroLed;
 void setEstadoAroLed(unsigned char mEstado){
     estadoAroLed = mEstado;
     isEstadoCambioAroLed = 0xff;
 }

 unsigned char getEstadoAroLed(){
     return estadoAroLed;
 }

 /**
 * @brief mainEstadoUnregisterAroLed es el main que ejecuta las tarea de No registrado para el aro de led
*/
void mainEstadoUnregisterAroLed(){
    ringLedRGB.run();
    ringLedRGB.Wait(0,0,0);
}


/**
 * @brief mainEstadoRegisterAroLed es el main que ejecuta las tarea de registrado para el aro de led
*/
void mainEstadoRegisterAroLed(){
    //Iniciamos con el estado disponibel
      ringLedRGB.run();
      ringLedRGB.Wait(0,20,0);
}


/**
 * @brief mainEstadoReservedAroLed es el main que ejecuta las tarea de Reservado para el aro de led
*/
void mainEstadoReservedAroLed(){
    ringLedRGB.run();
    ringLedRGB.Reserved(32,16,0);
}


/**
 * @brief mainEstadoBusyAroLed es el main que ejecuta las tarea de Reservado para el aro de led
*/

void mainEstadoBusyAroLed(){
    //Estado Ocupado
    ringLedRGB.run();
    ringLedRGB.Busy(30,0,0);
}

/**
 * @brief mainEstadoValidationAroLed es el main que ejecuta las tarea de Reservado para el aro de led
*/

void mainEstadoValidationAroLed(){
    if (getEstadoBooking()==ESTADO_RESERVED)
    {
        for (char i = 0; i < 2; i++)
        {
            //Validacion--------------------------
            ringLedRGB.run();
            ringLedRGB.Validate(20,20,20,50);
            ringLedRGB.run();
            ringLedRGB.Wait(0,0,0);
        }
        setEstadoAroLed(getEstadoAroLed());
    }
}



/**
 * @brief inicializarAroLed inicializa todas las configuraciones necesarias para el aro de led
*/

void inicializarAroLed(){
    ringLedRGB.Setup();
    
    ringLedRGB.run();
    ringLedRGB.Wait(0,0,0);
}

/**
 * @brief mainEstadosBookingAroLed es la maquina de estado de las diferentes condiciones de operacion para el aro de led
*/

void mainEstadosBookingAroLed(){
    
    if (isEstadoCambioAroLed)
    {
        Serial.println("ejecutando mainEstadosBookingAroLed");
        switch (getEstadoAroLed())
        {
        case ESTADO_UNREGISTER_ARO_LED:
            mainEstadoUnregisterAroLed();
            break;

        case ESTADO_REGISTER_ARO_LED:
            mainEstadoRegisterAroLed();
            break;

        
        case ESTADO_RESERVED_ARO_LED:
            mainEstadoReservedAroLed();
            break;
        
        case ESTADO_BUSY_ARO_LED:
            mainEstadoBusyAroLed();
            break;

        case ESTADO_VALIDATIO_ARO_LED:
            mainEstadoValidationAroLed();
            break;
        
        default:
            break;
        }

    isEstadoCambioAroLed = 0;
    }
}



TAREA tareaEstadosBookingAroLed = {NULL, NULL, 0, 0l};
TAREA *creaTareaAtiendeEstadosBookingAroLed()
{
    tareaEstadosBookingAroLed.ptr_inicializaTarea =&inicializarAroLed;
    tareaEstadosBookingAroLed.ptr_tarea_main = &mainEstadosBookingAroLed;
    tareaEstadosBookingAroLed.periodo = 2000;
    tareaEstadosBookingAroLed.momentoAnterior = 0;
    return &tareaEstadosBookingAroLed;
}