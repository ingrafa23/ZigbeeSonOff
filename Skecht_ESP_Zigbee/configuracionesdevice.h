#ifndef CONFIGURACIONES_DEVICE_INCLUDE
#define CONFIGURACIONES_DEVICE_INCLUDE

#include <string.h>
#include <Arduino.h>

extern unsigned long valueHeartbeat;
extern String spaceId;
extern String deviceId;

void setValueHeartbeat(unsigned long vTime);
unsigned long getValueHeartbeat();

void setSpaceId(String vspaceId);
String getSpaceIdt();

void setDeviceId(String vdeviceId);
String getDeviceId();

 String getCustomerId();
 void setCustomerId(String m);


#endif