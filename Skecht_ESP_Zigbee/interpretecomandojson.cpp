#include "interpretecomandojson.h"
#include "atiendeSendJson.h"
#include "interprete.h"
#include "TAREAS.h"
#include <ArduinoJson.h>
#include "estadobooking.h"
#include "configuracionesdevice.h"
#include "bufferfifiString.h"

bufferfifostring bufferfifoJson;

//funcion que llena el buffer 
void setJsonStrinfBuffer(String mStringJson){
    bufferfifoJson.setStringBuffer(mStringJson);
}


void mainInterpreteComandosJson(){
    
    if (bufferfifoJson.statusBuffer())
    {
        Serial.println("ejecutando mainInterpreteComandosJson");

        String json_received = bufferfifoJson.getStringdBuffer();
        //json_received = getDataStringRecibeJson();

        Serial.println("--------------------------------------------------");
        Serial.print("json_received "); Serial.println(json_received);
        Serial.println("--------------------------------------------------");

        StaticJsonDocument<512> doc_received;
        DeserializationError error = deserializeJson(doc_received, json_received);
        if (error) { 
            Serial.println("ejecutando mainInterpreteComandosJson error");
        }
        else{

            String device_id = doc_received["device_id"];
            String command = doc_received["command"];
            String space_id = doc_received["space_id"];
            int heartbeat = doc_received["settings"]["heartbeat"];

            //condicion de command
            if(command.length()>0){
                //Serial.print("command :"); Serial.println(command);
                char data_interprete[64];
            for (unsigned char i = 0; i < 64; i++)
                {
                    if (i < command.length()){
                        data_interprete[i] = (char)command[i];     
                    }
                    else
                    {
                        data_interprete[i]='\0';
                    }
                
                }
                interpreteIntrucciones(data_interprete); 
            }

            if (device_id.length()>0)
            {
                /* code */
                //Serial.print("device_id :"); Serial.println(device_id);
                setDeviceId(device_id);
            }


            if (space_id.length()>0)
            {
                /* code */
                //Serial.print("space_id :"); Serial.println(space_id);
                setSpaceId(space_id);
            }

            if (heartbeat>0)
            {
                /* code */
                //Serial.print("heartbeat :"); Serial.println(heartbeat);
                setValueHeartbeat(heartbeat);
                setPeriodoEstadosBooking(getValueHeartbeat());
            }
        }
    }
    

}


TAREA tareaInterpreteComandosJson = {NULL, NULL, 0, 0l};
TAREA *creaTareaInterpreteComandosJson()
{
    tareaInterpreteComandosJson.ptr_inicializaTarea =NULL;
    tareaInterpreteComandosJson.ptr_tarea_main = &mainInterpreteComandosJson;
    tareaInterpreteComandosJson.periodo = 1000;
    tareaInterpreteComandosJson.momentoAnterior = 0;
    return &tareaInterpreteComandosJson;
}