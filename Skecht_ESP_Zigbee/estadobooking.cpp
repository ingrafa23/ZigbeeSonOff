#include "estadobooking.h"
#include <ArduinoJson.h>
#include "atiendeSendJson.h"
#include "configuracionesdevice.h"
#include <ESP8266WiFi.h>


#include "com_zigbee.h"
#include "protocoloZigbee.h"
unsigned char estadoBooking;

/**
 * @brief setEstadoBooking asigana un nuevo estadp
 * @param mEstado valor del estado
*/
void setEstadoBooking(unsigned char mEstado, unsigned char _numSensor)
{
    //estadoBooking = mEstado;
    Sensor_CambiarEstado_MaquinaEstado(_numSensor,mEstado);
}

/**
 * @brief getEstadoBooking obtiene el valor des estado de la maquina
 * @return retorna el estado
*/
unsigned char getEstadoBooking()
{
    return estadoBooking;
}

/**
 * @brief mainEstadoUnregister es el main que ejuta las tarea de No registrado
*/
void mainEstadoUnregister(unsigned char _numSensor)
{
    Serial1.println("Ejecutando estado UNREGISTER");
    
    // Serializacion Json Estado Register
    String json_status_register;
    json_status_register = "{\"sensor_id\":\"" + getDeviceIdSensor(_numSensor) + "\",";

    json_status_register += "\"sensor_mac\":\"" + getDeviceMacSensor(_numSensor) + "\",";

    json_status_register += "\"sensor_value\":\"" + getValueSensor(_numSensor) + "\",";

    json_status_register += "\"customer_id\":\""+getCustomerId()+"\",";

    json_status_register += "\"state\":\"unregister\",";

    json_status_register += "\"space_id\":\"" + getSpaceIdt() + "\",";

    json_status_register += "\"settings\":{\"heartbeat\":"+String(getValueHeartbeat()/1000.0)+",";

    json_status_register += "\"ip_addr\":\"" + WiFi.localIP().toString() + "\",";

    json_status_register += "\"device_mac\":\"" + getMacBridge() + "\"}}";

    Serial1.print("Enviando json :"); Serial1.println(json_status_register);

    sendDataJson(json_status_register,getNodeAPI());
    // Fin Json
}

/**
 * @brief mainEstadoRegister es el main que ejuta las tarea de registrado
*/
void mainEstadoRegister(unsigned char _numSensor)
{
    Serial1.println("Ejecutando estado REGISTER");
    // Serializacion Json Estado Register
    String json_status_register;
    json_status_register = "{\"sensor_id\":\"" + getDeviceIdSensor(_numSensor) + "\",";

    json_status_register += "\"sensor_mac\":\"" + getDeviceMacSensor(_numSensor) + "\",";

    json_status_register += "\"sensor_value\":\"" + getValueSensor(_numSensor) + "\",";

    json_status_register += "\"customer_id\":\""+getCustomerId()+"\",";

    json_status_register += "\"state\":\"ready\",";

    json_status_register += "\"space_id\":\"" + getSpaceIdt() + "\",";

    json_status_register += "\"settings\":{\"heartbeat\":"+String(getValueHeartbeat()/1000.0)+",";

    json_status_register += "\"ip_addr\":\"" + WiFi.localIP().toString() + "\",";

    json_status_register += "\"device_mac\":\"" + getMacBridge() + "\"}}";

    
    
    Serial1.println(json_status_register);
    sendDataJson(json_status_register,getNodeAPI());
    // Fin Json
}

/**
 * @brief mainEstadoReserved es el main que ejuta las tarea de Reservado
*/
void mainEstadoReserved(unsigned char _numSensor)
{
    Serial1.println("Ejecutando estado RESERVED");
    
    // Serializacion Json Estado waiting_confirmation
    String json_status_register;
    json_status_register = "{\"sensor_id\":\"" + getDeviceIdSensor(_numSensor) + "\",";

    json_status_register += "\"sensor_mac\":\"" + getDeviceMacSensor(_numSensor) + "\",";

    json_status_register += "\"sensor_value\":\"" + getValueSensor(_numSensor) + "\",";
    
    json_status_register += "\"customer_id\":\""+getCustomerId()+"\",";

    json_status_register += "\"state\":\"waiting_confirmation\",";

    json_status_register += "\"space_id\":\"" + getSpaceIdt() + "\",";

    json_status_register += "\"settings\":{\"heartbeat\":"+String(getValueHeartbeat()/1000.0)+",";

    json_status_register += "\"ip_addr\":\"" + WiFi.localIP().toString() + "\",";

    json_status_register += "\"device_mac\":\"" + getMacBridge() + "\"}}";

    Serial1.print("Enviando json :"); Serial1.println(json_status_register);

    Serial1.println(json_status_register);
    sendDataJson(json_status_register,getNodeAPI());
    // Fin Json
}

/**
 * @brief mainEstadoBusy es el main que ejuta las tarea de Reservado
*/

void mainEstadoBusy(unsigned char _numSensor)
{
    Serial1.println("Ejecutando estado BUSY");
    // Serializacion Json Estado booked
    
    String json_status_register;
    json_status_register = "{\"sensor_id\":\"" + getDeviceIdSensor(_numSensor) + "\",";

    json_status_register += "\"sensor_mac\":\"" + getDeviceMacSensor(_numSensor) + "\",";

    json_status_register += "\"sensor_value\":\"" + getValueSensor(_numSensor) + "\",";

    json_status_register += "\"customer_id\":\""+getCustomerId()+"\",";

    json_status_register += "\"state\":\"booked\",";

    json_status_register += "\"space_id\":\"" + getSpaceIdt() + "\",";

    json_status_register += "\"settings\":{\"heartbeat\":"+String(getValueHeartbeat()/1000.0)+",";

    json_status_register += "\"ip_addr\":\"" + WiFi.localIP().toString() + "\",";

    json_status_register += "\"device_mac\":\"" + getMacBridge() + "\"}}";

    Serial1.print("Enviando json :"); Serial1.println(json_status_register);

    Serial1.println(json_status_register);

    sendDataJson(json_status_register,getNodeAPI());
    // Fin Json
}

/**
 * @brief mainEstadoError es el main que ejuta las tarea del sernsor no conectado
*/

void mainEstadoError(unsigned char _numSensor)
{
    Serial1.println("Ejecutando estado ERROR");

    // Serializacion Json Estado booked
    
    String json_status_register;
    json_status_register = "{\"sensor_id\":\"" + getDeviceIdSensor(_numSensor) + "\",";

    json_status_register += "\"sensor_mac\":\"" + getDeviceMacSensor(_numSensor) + "\",";

    json_status_register += "\"customer_id\":\""+getCustomerId()+"\",";

    json_status_register += "\"state\":\"no_connected\",";

    json_status_register += "\"space_id\":\"" + getSpaceIdt() + "\",";

    json_status_register += "\"settings\":{\"heartbeat\":"+String(getValueHeartbeat()/1000.0)+",";

    json_status_register += "\"ip_addr\":\"" + WiFi.localIP().toString() + "\",";

    json_status_register += "\"device_mac\":\"" + getMacBridge() + "\"}}";

    Serial1.print("Enviando json :"); Serial1.println(json_status_register);

    Serial1.println(json_status_register);

    sendDataJson(json_status_register,getNodeAPI());
    // Fin Json
}

/**
 * @brief mainEstadosBooking es la maquina de estado de las diferentes condiciones de operacion
 */
void mainEstadosBooking()
{
    for (unsigned char ki = 0; ki < getNumSensorVinvulados(); ki++)
    {
        if (getEstadoVinculadoSensor(ki)>0)
        {
            switch (getEstadoMaquinaSensor(ki))
            {
                case ESTADO_UNREGISTER:
                    mainEstadoUnregister(ki);
                    break;

                case ESTADO_REGISTER:
                    mainEstadoRegister(ki);
                    break;

                
                case ESTADO_RESERVED:
                    mainEstadoReserved(ki);
                    break;
                
                case ESTADO_BUSY:
                    mainEstadoBusy(ki);
                    break;
                
                default:
                    break;
            }
        }
        else
        {
            mainEstadoError(ki);
        }
        
        
    }
    
        
    
}
