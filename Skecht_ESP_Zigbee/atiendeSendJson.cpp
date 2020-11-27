#include "atiendeSendJson.h"
#include "interpretecomandojson.h"
#include "bufferfifiString.h"
#include "TAREAS.h"
#include <Arduino.h>

#include "requestpost.h"
requestpost requestServer;

#include <WiFiClient.h>
WiFiClient client;
#define SERVER     "api.infinito.apeiroo.com"
#define PORT     443

bufferfifostring buffersendDataJsonApi;
bufferfifostring bufferdataStringJson;

String dataStringJson;
unsigned char sendDataJsonFlag = 0;
String sendDataJsonApi;
String dataStringRecibeJson;
unsigned char dataStringRecibeJsonFlag = 0;

void sendDataJson(String myString, String myApi){

    buffersendDataJsonApi.setStringBuffer(myApi);
    bufferdataStringJson.setStringBuffer(myString);


}


unsigned char getAvailabSendDataJoson(){
    return buffersendDataJsonApi.statusBuffer() && bufferdataStringJson.statusBuffer();
}


String getDataStringRecibeJson(){
    return dataStringRecibeJson;
}

void beginConfigSendDataJson(){
    requestServer.begin(SERVER,PORT);
}

void mainSendDataJson(){
    
    if (getAvailabSendDataJoson()>0)
    {
        Serial.println("ejecutando mainSendDataJson");

        sendDataJsonApi = buffersendDataJsonApi.getStringdBuffer();
        dataStringJson = bufferdataStringJson.getStringdBuffer();

        requestServer.sendJson(sendDataJsonApi,dataStringJson);
        if (requestServer.available())
        {
            dataStringRecibeJson = requestServer.readJson();
            setJsonStrinfBuffer(dataStringRecibeJson);
        }
        
    } 
}


TAREA tareaSendDataJson = {NULL, NULL, 0, 0l};
TAREA *creaTareaSendDataJson()
{
    tareaSendDataJson.ptr_inicializaTarea =&beginConfigSendDataJson;
    tareaSendDataJson.ptr_tarea_main = &mainSendDataJson;
    tareaSendDataJson.periodo = 1000;
    tareaSendDataJson.momentoAnterior = 0;
    return &tareaSendDataJson;
}