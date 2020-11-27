#include "ejecutacheckqr.h"
#include "capturaimagen.h"
#include "TAREAS.h"
#include "estadobooking.h"
#include "atiendeSendJson.h"
#include "configuracionesdevice.h"
#include <ArduinoJson.h>
capturaimagen Camara;

void inicializaConfigEjecutaCheckQR(){
    Camara.begin();
}

void mainEjecutaCheckQR(){
    Serial.println("Ejecutando mainEjecutaCheckQR");
    if (getEstadoBooking() == ESTADO_RESERVED)
    {
        if(!Camara.getStatusError())
        {
            Camara.capturaImagen();
            if (Camara.getAvailableEncodingB64Imagen())
            {
                String json_encoding64_imagen;
                
                json_encoding64_imagen = F("{\"device_id\":\"");
                json_encoding64_imagen += getDeviceId();
                json_encoding64_imagen += F("\",");

                json_encoding64_imagen += F("\"encodedimage\":\"");
                json_encoding64_imagen += Camara.getdataEncodingB64Imagen().c_str();
                json_encoding64_imagen += F("\"}");

                Serial.println(json_encoding64_imagen);
                sendDataJson(json_encoding64_imagen,"/hw/check_qr");

            }
            
        }
    }
}

TAREA tareaEjecutaCheckQR = {NULL, NULL, 0, 0l};
TAREA *creaTareaEjecutaCheckQR()
{
    tareaEjecutaCheckQR.ptr_inicializaTarea =&inicializaConfigEjecutaCheckQR;
    tareaEjecutaCheckQR.ptr_tarea_main = &mainEjecutaCheckQR;
    tareaEjecutaCheckQR.periodo = 60000;
    tareaEjecutaCheckQR.momentoAnterior = 0;
    return &tareaEjecutaCheckQR;
}

