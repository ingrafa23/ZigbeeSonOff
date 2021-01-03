#include "com_zigbee.h"

extern Estructura_Sensor ZigbeeSensorMovimiento[NUM_SENSORES_MOVIMIENTO];
extern com_zigbee_Sensor_Data DataSensorMovimiento;

char Sensor_Leer(char sensor)
{
    
    if(ZigbeeSensorMovimiento[sensor].estadoVinculado == 1)
    {
        //--------ID-----------------
        DataSensorMovimiento.id = String(ZigbeeSensorMovimiento[sensor].id[0],HEX);
        DataSensorMovimiento.id += String(ZigbeeSensorMovimiento[sensor].id[1],HEX);
        //---------------------------
        //-----Print MAC
        DataSensorMovimiento.mac = String(ZigbeeSensorMovimiento[sensor].mac[7],HEX);
        DataSensorMovimiento.mac += ":";
        Serial1.println(DataSensorMovimiento.mac);
        for (unsigned char mk = 7; mk > 1; mk--)
        {
            DataSensorMovimiento.mac += String(ZigbeeSensorMovimiento[sensor].mac[mk-1],HEX);
            DataSensorMovimiento.mac += ":";
        }
        DataSensorMovimiento.mac += String(ZigbeeSensorMovimiento[sensor].mac[0],HEX);
        DataSensorMovimiento.valorMedida = ZigbeeSensorMovimiento[sensor].valorMedida;
        DataSensorMovimiento.maquinaEstado = ZigbeeSensorMovimiento[sensor].maquinaEstado;

        return 1;
    }
    else
        return 0;
}

char Sensor_CambiarEstado_MaquinaEstado(char sensor, char nuevo_estado)
{
    if(ZigbeeSensorMovimiento[sensor].estadoVinculado == 1)
    {
        ZigbeeSensorMovimiento[sensor].maquinaEstado = nuevo_estado;       
        return 1;
    }
    else
        return 0;
}

char getNumeroSensor(String _id, String _mac)
{
    String cadID;
    String cadMAC;

    for (unsigned char kt = 0; kt < NUM_SENSORES_MOVIMIENTO; kt++)
    {
        cadID = "0x" + String(ZigbeeSensorMovimiento[kt].id[0],HEX) 
                     + String(ZigbeeSensorMovimiento[kt].id[1],HEX);

        cadMAC = String(ZigbeeSensorMovimiento[kt].mac[0], HEX) + ":" + 
                 String(ZigbeeSensorMovimiento[kt].mac[1], HEX) + ":" + 
                 String(ZigbeeSensorMovimiento[kt].mac[2], HEX) + ":" + 
                 String(ZigbeeSensorMovimiento[kt].mac[3], HEX) + ":" + 
                 String(ZigbeeSensorMovimiento[kt].mac[4], HEX) + ":" + 
                 String(ZigbeeSensorMovimiento[kt].mac[5], HEX) + ":" + 
                 String(ZigbeeSensorMovimiento[kt].mac[6], HEX) + ":" + 
                 String(ZigbeeSensorMovimiento[kt].mac[7], HEX);  

        if ((cadID.indexOf(_id) >= 0) && (cadMAC.indexOf(_mac) >= 0))
        {
            return kt;
        }
    }
    return -1;    
}

unsigned char getEstadoMaquinaSensor(unsigned char _numSensor)
{
    return ZigbeeSensorMovimiento[_numSensor].maquinaEstado;
}

unsigned char getEstadoVinculadoSensor(unsigned char _numSensor)
{
    return ZigbeeSensorMovimiento[_numSensor].estadoVinculado;
}

String getDeviceIdSensor(unsigned char _numSensor)
{
    return "0x" + String(ZigbeeSensorMovimiento[_numSensor].id[0],HEX) 
                + String(ZigbeeSensorMovimiento[_numSensor].id[1],HEX);

}
String getDeviceMacSensor(unsigned char _numSensor)
{
    return String(ZigbeeSensorMovimiento[_numSensor].mac[0], HEX) + ":" + 
           String(ZigbeeSensorMovimiento[_numSensor].mac[1], HEX) + ":" + 
           String(ZigbeeSensorMovimiento[_numSensor].mac[2], HEX) + ":" + 
           String(ZigbeeSensorMovimiento[_numSensor].mac[3], HEX) + ":" + 
           String(ZigbeeSensorMovimiento[_numSensor].mac[4], HEX) + ":" + 
           String(ZigbeeSensorMovimiento[_numSensor].mac[5], HEX) + ":" + 
           String(ZigbeeSensorMovimiento[_numSensor].mac[6], HEX) + ":" + 
           String(ZigbeeSensorMovimiento[_numSensor].mac[7], HEX); 
}

String getValueSensor(unsigned char _numSensor)
{
    if (ZigbeeSensorMovimiento[_numSensor].valorMedida)
    {
        return "1";
    }
    
    return "0";
}


