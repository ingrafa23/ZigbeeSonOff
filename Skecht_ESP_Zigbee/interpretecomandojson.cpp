#include "interpretecomandojson.h"
#include "atiendeSendJson.h"
#include "interprete.h"
#include <ArduinoJson.h>
#include "estadobooking.h"
#include "configuracionesdevice.h"
#include "bufferfifiString.h"
#include "com_zigbee.h"

bufferfifostring bufferfifoJson;

//funcion que llena el buffer 
void setJsonStrinfBuffer(String mStringJson)
{
    bufferfifoJson.setStringBuffer(mStringJson);
}

void mainInterpreteComandosJson()
{    
    //Serial1.println("ejecutando mainInterpreteComandosJson");
    if (bufferfifoJson.statusBuffer())
    {
        String json_received = bufferfifoJson.getStringdBuffer();
        //json_received = getDataStringRecibeJson();

        Serial1.println("--------------------------------------------------");
        Serial1.print("json_received "); Serial1.println(json_received);
        Serial1.println("--------------------------------------------------");

        StaticJsonDocument<512> doc_received;
        DeserializationError error = deserializeJson(doc_received, json_received);
        if (error) 
        { 
            Serial1.println("ejecutando mainInterpreteComandosJson error");
        }
        else
        {
            String sensor_id = doc_received["sensor_id"];
            String sensor_mac = doc_received["sensor_mac"];
            String command = doc_received["command"];
            String space_id = doc_received["space_id"];
            //int heartbeat = doc_received["settings"]["heartbeat"];

            //condicion de command
            if(command.length() > 0)
            {
                //Serial1.print("command :"); Serial1.println(command);
                char data_interprete[64];
                for (unsigned char i = 0; i < 64; i++)
                {
                    if (i < command.length())
                    {
                        data_interprete[i] = (char)command[i];     
                    }
                    else
                    {
                        data_interprete[i]='\0';
                    }
                }

                char myNumSensor = getNumeroSensor(sensor_id,sensor_mac);
                if (myNumSensor>=0)
                {
                    interpreteIntrucciones(data_interprete,myNumSensor);
                }
                
                 
            }

            if (sensor_id.length()>0)
            {
                /* code */
                //Serial1.print("sensor_id :"); Serial1.println(sensor_id);
                setDeviceId(sensor_id);
            }

            if (space_id.length()>0)
            {
                /* code */
                //Serial1.print("space_id :"); Serial1.println(space_id);
                setSpaceId(space_id);
            }

            /*
            if (heartbeat>0)
            {
                
                //Serial1.print("heartbeat :"); Serial1.println(heartbeat);
                setValueHeartbeat(heartbeat);                
            }
            */
        }
    }
}
