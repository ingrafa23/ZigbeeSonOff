#include<stdio.h>
#include<stdlib.h>
#include "TAREAS.h"

//Include OTA
#include "ota_esp.h"
//inteprete
#include "interprete.h"
//
#include "estadobooking.h"
#include "configuracionesdevice.h"
#include "atiendeSendJson.h"
#include "interpretecomandojson.h"
//----------------------------------------------------------------
//Protocolo de comunicacion y conexion con los sensores Zigbee
#include "protocoloZigbee.h"
//-------------------------------------------------
//Tareas del Core0
#define NUMERO_TAREAS_CORE0 7
// Tareas core0 start
//Atiende Tarea OTA 
//OTA being
/**
 * @brief  atiendeOTa es la tarea encargada de realizaer el proceso de conexion wifi del dispositivo y de la programacion via OTA
 */


ota_esp conexionOta;

void inicializaOta()
{
    Serial1.println("Inicializando Ota");
    conexionOta.begin();
}
void mainOta()
{
    conexionOta.detec();
}


////OTA end

// Tareas core0 end

// -------------------

//Tareas del Core1
// Tareas core0 start

void tareaMainInterpreteEjecuta()
{
    interpreteEjecuta();
}

void tareaMainInterprete()
{
    
    if(Serial1.available() > 0)
    {
        char data_recibida[64];
        for (unsigned char i = 0; i < 64; i++)
        {
            if (Serial1.available() > 0)
            {
                data_recibida[i] = (char)Serial1.read();     
            }
            else
            {
                data_recibida[i]='\0';
            }            
        }
        Serial1.println(data_recibida);
        //interpreteIntrucciones(data_recibida);
    }    
}

TAREA tareasCore0 [NUMERO_TAREAS_CORE0];

void setup() 
{
    Serial1.begin(115200);
    Serial1.println("\nInicializando el Programa\n");

    int indiceTareasCore0 = 0;

    //Tareas Core 0
    asignarTareas(&tareasCore0[indiceTareasCore0++], inicializaOta, mainOta,  10, 0); //Establece conexionWifi y verifica si hay un load Via OTA
    asignarTareas(&tareasCore0[indiceTareasCore0++], NULL, mainEstadosBooking,  20000, 0); //Maquina de estado Booking
    asignarTareas(&tareasCore0[indiceTareasCore0++], beginConfigSendDataJson, mainSendDataJson,  1000, 0);
    asignarTareas(&tareasCore0[indiceTareasCore0++], NULL, tareaMainInterprete,  10, 0);
    asignarTareas(&tareasCore0[indiceTareasCore0++], NULL, tareaMainInterpreteEjecuta,  10, 0);
    asignarTareas(&tareasCore0[indiceTareasCore0++], NULL, mainInterpreteComandosJson,  1000, 0);
    //Tarea que ejecuta el protocolo Zigbee
    asignarTareas(&tareasCore0[indiceTareasCore0++], ConfigProtocolo, loopLeerSensoresZigbee,  10, 0);

    //Inicializa Tareas Core0
    incializaTareas(tareasCore0, getMomento(),NUMERO_TAREAS_CORE0);
}

//core 0

void loop() 
{
  // put your main code here, to run repeatedly:
  ejecutaTareas(tareasCore0, getMomento(), NUMERO_TAREAS_CORE0);
  ESP.wdtDisable(); 
  
}
