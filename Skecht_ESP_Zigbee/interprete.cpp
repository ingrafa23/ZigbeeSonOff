#include "interprete.h"
#include "estadobooking.h"
#include "bufferfifo.h"
bufferfifo fifoInteprete;



void beginserial(){
}



//lista de instrucciones
#define MAX_ORDENES 9
char *instrucciones[] = {"continue",                        //0
                         "register",                        //1
                         "unregister",                      //2
                         "prebook",                         //3
                         "booking_canceled",                //4
                         "booking_confirmation_timeout",    //5
                         "booking_verification_failed",     //6
                         "booking_verification_ok",         //7
                         "booking_finished",                //8
                         NULL};          

/**
 * @brief strEqual compara dos string
 * @param a el primer string a comparar
 * @param b el segundo string a comparar
 * @return retorna true si son iguales o false si son diferentes
*/

char strEqual(char *a, char *b)
{
    if (a == b)
        return 0xff;
    if (a == NULL || b == NULL)
        return 0x00;
    return strcmp(a, b) == 0;
}

/**
 * @brief interpreteIntrucciones
 * @param a el primer string a comparar
 * @param b el segundo string a comparar
 * @return retorna true si son iguales o false si son diferentes
*/

void interpreteIntrucciones(char *commandIntrucciones){
        
    for (unsigned int i = 0; i < MAX_ORDENES; i++)
    {
        /* code */
        if(strEqual(commandIntrucciones,instrucciones[i])){
            fifoInteprete.fillBuffer(i);
            break;
        }
    }
}


void interpreteEjecuta(){

    
    if(fifoInteprete.statusBuffer()){
        
        unsigned char command = fifoInteprete.getcomandBuffer();

        Serial1.print("Comando : ");Serial1.println(command);

        Serial1.println(instrucciones[command]);
    
        switch (command)
        {
            case 0:
                
                break;
            
            case 1:
                setEstadoBooking(ESTADO_REGISTER);
                //setEstadoAroLed(ESTADO_REGISTER_ARO_LED);
                break;
            
             case 2:
                setEstadoBooking(ESTADO_UNREGISTER);
                //setEstadoAroLed(ESTADO_UNREGISTER_ARO_LED);
                break;

            case 3:
                setEstadoBooking(ESTADO_RESERVED);
                //setEstadoAroLed(ESTADO_RESERVED_ARO_LED);
                break;

            case 4:
                setEstadoBooking(ESTADO_REGISTER);
                //setEstadoAroLed(ESTADO_REGISTER_ARO_LED);
                break;

            case 5:
                setEstadoBooking(ESTADO_REGISTER);
                //setEstadoAroLed(ESTADO_REGISTER_ARO_LED);
                break;

            case 6:
                //setEstadoAroLed(ESTADO_VALIDATIO_ARO_LED);
                break;
            
            case 7:
                setEstadoBooking(ESTADO_BUSY);
                //setEstadoAroLed(ESTADO_BUSY_ARO_LED);
                break;
                
            case 8:
                setEstadoBooking(ESTADO_REGISTER);
                //setEstadoAroLed(ESTADO_REGISTER_ARO_LED);
                break;
            
            default:
                break;
        }
    
    }

}