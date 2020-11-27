#include<stdio.h>
#include<stdlib.h>


#include "TAREAS.h"
#include "main.h"

//Include OTA
#include "ota_esp.h"

//inteprete

#include "interprete.h"


//-------------------------------------------------
// Tareas core0 start
//Atiende Tarea OTA 
//OTA being
/**
 * @brief  atiendeOTa es la tarea encargada de realizaer el proceso de conexion wifi del dispositivo y de la programacion via OTA
 */


ota_esp conexionOta;

void inicializaOta(){
    conexionOta.begin();
}
void mainOta(){
    conexionOta.detec();
}

TAREA atiendeOTa = {NULL, NULL, 0, 0l};
TAREA *creaTareaAtiendeOta(){
    atiendeOTa.ptr_inicializaTarea =&inicializaOta;
    atiendeOTa.ptr_tarea_main = &mainOta;
    atiendeOTa.periodo = 10;
    atiendeOTa.momentoAnterior = 0;
    return &atiendeOTa;
}

////OTA end

// Tareas core0 end

// -------------------

//Tareas del Core1
// Tareas core0 start

void tareaMainInterpreteEjecuta(){
    interpreteEjecuta();
}

TAREA tareaEjecutaInterprete = {NULL, NULL, 0, 0l};
TAREA *crearTareaEjecutaInterprete()
{
    tareaEjecutaInterprete.ptr_inicializaTarea =NULL;
    tareaEjecutaInterprete.ptr_tarea_main = &tareaMainInterpreteEjecuta;
    tareaEjecutaInterprete.periodo = 10;
    tareaEjecutaInterprete.momentoAnterior = 0;
    return &tareaEjecutaInterprete;
}





void tareaMainInterprete(){

    if(Serial.available()>0){
        char data_recibida[64];
        for (unsigned char i = 0; i < 64; i++)
        {
            if (Serial.available()>0){
                data_recibida[i] = (char)Serial.read();     
            }
            else
            {
                data_recibida[i]='\0';
            }
            
        }
        Serial.println(data_recibida);
        interpreteIntrucciones(data_recibida);
    }
    
}


TAREA tareaInterprete = {NULL, NULL, 0, 0l};
TAREA *creaTareaInterprete()
{
    tareaInterprete.ptr_inicializaTarea =NULL;
    tareaInterprete.ptr_tarea_main = &tareaMainInterprete;
    tareaInterprete.periodo = 100;
    tareaInterprete.momentoAnterior = 0;
    return &tareaInterprete;
}

//Atiende Serial



// Tareas core1 end




TAREA * tareasCore0 [NUMERO_TAREAS_CORE0];
TAREA * tareasCore1 [NUMERO_TAREAS_CORE1];

//Configurando Core 1 del ESP32 

//---------------------------
TaskHandle_t taskHandelLoopCore1;

void beginCore1(){
    xTaskCreatePinnedToCore(
    loopCore1,   // Task function. 
    "loopCore1",     // name of task.
    16000,       // Stack size of task
    NULL,        // parameter of the task
    1,           // priority of the task
    &taskHandelLoopCore1,      // Task handle to keep track of created task
    1); //Core
}





void setup() {

  Serial.begin(115200);
  Serial.println("Inicializando el Programa\n");

  beginCore1();

  #ifdef tickerTimeESP
  //inicializar el timer
    ContadorStart();
  #endif

  int indiceTareasCore0 = 0;
  int indiceTareasCore1 = 0; 
  //Tareas Core 0
  tareasCore0[indiceTareasCore0++] = creaTareaAtiendeOta (); //Establece conexionWifi y verifica si hay un load Via OTA
  tareasCore0[indiceTareasCore0++] = creaTareaAtiendeEstadosBooking (); //Maquina de estado Booking
  tareasCore0[indiceTareasCore0++] = creaTareaSendDataJson();
  tareasCore0[indiceTareasCore0++] = creaTareaEjecutaCheckQR();
  //Tareas Core 1
  tareasCore1[indiceTareasCore1++] = creaTareaInterprete ();
  tareasCore1[indiceTareasCore1++] = crearTareaEjecutaInterprete ();
  tareasCore1[indiceTareasCore1++] = creaTareaInterpreteComandosJson();
  tareasCore1[indiceTareasCore1++] = creaTareaAtiendeEstadosBookingAroLed();
  
  //Inicializa Tareas Core0
  incializaTareas(tareasCore0, getMomento(),NUMERO_TAREAS_CORE0);
  //Inicializa Tareas Core0
  incializaTareas(tareasCore1, getMomento(),NUMERO_TAREAS_CORE1);

}

//core 0

void loop() {
  // put your main code here, to run repeatedly:
   bucleMain(tareasCore0,NUMERO_TAREAS_CORE0);
}

//Core 1

void loopCore1( void * pvParameters ) {
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());
    for (;;){
        ejecutaTareasMain(tareasCore1, getMomento(),NUMERO_TAREAS_CORE1);
    } 
}



/**
 * @brief  inicializaEstructuraTareas asigana los valores de las estructura en NULL o en 0
 * @param tareas representa la struct TAREAS a inicializar
 */
void inicializaEstructuraTareas(TAREA * tareas[],int numeroTareas) {

    for (int x = 0;x<numeroTareas;x++) {
		        
        tareas[x] = (TAREA *)malloc(sizeof(TAREA));
        if (tareas[x]==NULL) {
            
            //En arduino
            //Serial.println("Error en asignacion de memoria");
        }
		
        tareas[x]->periodo = 0;
        tareas[x]->ptr_inicializaTarea = NULL;
        tareas[x]->ptr_tarea_main = NULL;
        tareas[x]->momentoAnterior = 0;//ULONG_MAX;
        
    }
}

/**
 * @brief  incializaTareas Ejecuta solo una vez la funcion necearia para inicializar una tarea
 * @param tareas representa la struct TAREAS a inicializar
 * @param momentoActual esta asociado al tiempo actual de ejecucion
 */
void incializaTareas(TAREA * tareas[], unsigned long momentoActual,int numeroTareas) {
    for (int x = 0;x<numeroTareas;x++) {
        if (tareas[x]->ptr_inicializaTarea!=NULL) {
            tareas[x]->ptr_inicializaTarea();
            tareas[x]->momentoAnterior = momentoActual;
        }
    }
}



/**
 * @brief  ejecutaTareasMain Ejecuta la funcion ptr_tarea_main que representa la Tarea a ejecutar en el boocle main
 * @param tareas representa la struct TAREAS a ejecutar
 * @param momentoActual esta asociado al tiempo actual de ejecucion
 */

void ejecutaTareasMain(TAREA * tareas[], unsigned long momentoActual,int numeroTareas) {
    for (int x = 0;x<numeroTareas;x++) {
        TAREA * current = tareas[x];

        
        
        if (current == NULL) continue;
      
        if (abs(momentoActual -  current->momentoAnterior)>=current->periodo && current->ptr_tarea_main!=NULL) {
            current->momentoAnterior =momentoActual;
            current->ptr_tarea_main();
        }
    }
}




/**
 * @brief  bucleMain Ejecuta las tareas en el bucle main asociada en el puntero main
 * @param tareas representa la struct TAREAS a inicializar
 */

void bucleMain(TAREA * tareas[],int numeroTareas) {


    while (true) {
        
        
        ejecutaTareasMain(tareas, getMomento(),numeroTareas);
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
