#include "estadobooking.h"
#include "TAREAS.h"
#include <ArduinoJson.h>
#include "atiendeSendJson.h"
#include "configuracionesdevice.h"
#include <WiFi.h>


unsigned char estadoBooking;

/**
 * @brief setEstadoBooking asigana un nuevo estadp
 * @param mEstado valor del estado
*/
void setEstadoBooking(unsigned char mEstado){
    estadoBooking = mEstado;

}

/**
 * @brief getEstadoBooking obtiene el valor des estado de la maquina
 * @return retorna el estado
*/
unsigned char getEstadoBooking(){
    return estadoBooking;
}


/**
 * @brief mainEstadoUnregister es el main que ejuta las tarea de No registrado
*/
void mainEstadoUnregister(){
 Serial.println("Ejecutando estado UNREGISTER");
    // Serializacion Json
    String json_status_unregister;
    
    json_status_unregister = F("{\"device_id\":\"");
    json_status_unregister += getDeviceId();
    json_status_unregister += F("\",");
    json_status_unregister += F("\"customer_id\":\"None\"");
    json_status_unregister += F(",");
    json_status_unregister += F("\"state\":\"unregistered\"");
    json_status_unregister += F(",");
    json_status_unregister += F("\"space_id\":\"");
    json_status_unregister += getSpaceIdt();
    json_status_unregister += F("\",");
    json_status_unregister += F("\"settings\":{\"heartbeat\":");
    json_status_unregister += getValueHeartbeat()/1000.0;
    json_status_unregister += F(",");
    json_status_unregister += F("\"ip_addr\":\"");
    json_status_unregister += WiFi.localIP().toString();
    json_status_unregister += F("\"}}");

    Serial.print("Enviando json :"); Serial.println(json_status_unregister);

    sendDataJson(json_status_unregister,"/hw/check_status");
    // Fin Json

}


/**
 * @brief mainEstadoRegister es el main que ejuta las tarea de registrado
*/
void mainEstadoRegister(){
    Serial.println("Ejecutando estado REGISTER");
    // Serializacion Json Estado Register
    String json_status_register;
    json_status_register = F("{\"device_id\":\"");
    json_status_register += getDeviceId();
    json_status_register += F("\",");
    json_status_register += F("\"customer_id\":\"None\"");
    json_status_register += F(",");
    json_status_register += F("\"state\":\"ready\"");
    json_status_register += F(",");
    json_status_register += F("\"space_id\":\"");
    json_status_register += getSpaceIdt();
    json_status_register += F("\",");
    json_status_register += F("\"settings\":{\"heartbeat\":");
    json_status_register += getValueHeartbeat()/1000.0;
    json_status_register += F(",");
    json_status_register += F("\"ip_addr\":\"");
    json_status_register += WiFi.localIP().toString();
    json_status_register += F("\"}}");

    
    Serial.println(json_status_register);
    sendDataJson(json_status_register,"/hw/check_status");
    // Fin Json
}



/**
 * @brief mainEstadoReserved es el main que ejuta las tarea de Reservado
*/
void mainEstadoReserved(){
    Serial.println("Ejecutando estado RESERVED");
    
    // Serializacion Json Estado Register
    String json_status_register;
    
    json_status_register = F("{\"device_id\":\"");
    json_status_register += getDeviceId();
    json_status_register += F("\",");
    json_status_register += F("\"customer_id\":\"None\"");
    json_status_register += F(",");
    json_status_register += F("\"state\":\"waiting_confirmation\"");
    json_status_register += F(",");
    json_status_register += F("\"space_id\":\"");
    json_status_register += getSpaceIdt();
    json_status_register += F("\",");
    json_status_register += F("\"settings\":{\"heartbeat\":");
    json_status_register += getValueHeartbeat()/1000.0;
    json_status_register += F(",");
    json_status_register += F("\"ip_addr\":\"");
    json_status_register += WiFi.localIP().toString();
    json_status_register += F("\"}}");

    Serial.println(json_status_register);
    sendDataJson(json_status_register,"/hw/check_status");
    // Fin Json
}


/**
 * @brief mainEstadoBusy es el main que ejuta las tarea de Reservado
*/

void mainEstadoBusy(){
    Serial.println("Ejecutando estado BUSY");
    Serial.println("Ejecutando estado RESERVED");
    Serial.println("Ejecutando estado REGISTER");
    // Serializacion Json Estado Register
    String json_status_register;
    

    json_status_register = F("{\"device_id\":\"");
    json_status_register += getDeviceId();
    json_status_register += F("\",");
    json_status_register += F("\"customer_id\":\"None\"");
    json_status_register += F(",");
    json_status_register += F("\"state\":\"booked\"");
    json_status_register += F(",");
    json_status_register += F("\"space_id\":\"");
    json_status_register += getSpaceIdt();
    json_status_register += F("\",");
    json_status_register += F("\"settings\":{\"heartbeat\":");
    json_status_register += getValueHeartbeat()/1000.0;
    json_status_register += F(",");
    json_status_register += F("\"ip_addr\":\"");
    json_status_register += WiFi.localIP().toString();
    json_status_register += F("\"}}");


    
    Serial.println(json_status_register);
    sendDataJson(json_status_register,"/hw/check_status");
    // Fin Json
}

/**
 * @brief mainEstadosBooking es la maquina de estado de las diferentes condiciones de operacion
*/

void mainEstadosBooking(){
    
    switch (getEstadoBooking())
    {
    case ESTADO_UNREGISTER:
        mainEstadoUnregister();
        break;

    case ESTADO_REGISTER:
        mainEstadoRegister();
        break;

    
    case ESTADO_RESERVED:
        mainEstadoReserved();
        break;
    
    case ESTADO_BUSY:
        mainEstadoBusy();
        break;
    
    default:
        break;
    }
}



TAREA tareaEstadosBooking = {NULL, NULL, 0, 0l};
TAREA *creaTareaAtiendeEstadosBooking()
{
    tareaEstadosBooking.ptr_inicializaTarea =NULL;
    tareaEstadosBooking.ptr_tarea_main = &mainEstadosBooking;
    tareaEstadosBooking.periodo = getValueHeartbeat();
    tareaEstadosBooking.momentoAnterior = 0;
    return &tareaEstadosBooking;
}


void setPeriodoEstadosBooking(unsigned long mperiodo){
    tareaEstadosBooking.periodo = mperiodo;
}

