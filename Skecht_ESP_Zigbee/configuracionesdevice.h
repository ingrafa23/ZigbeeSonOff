#ifndef CONFIGURACIONES_DEVICE_INCLUDE
#define CONFIGURACIONES_DEVICE_INCLUDE

#include <string.h>
#include <Arduino.h>

extern unsigned long valueHeartbeat;
extern String spaceId;
extern String deviceId;

extern void setValueHeartbeat(unsigned long vTime);
extern unsigned long getValueHeartbeat();

extern void setSpaceId(String vspaceId);
extern String getSpaceIdt();

extern void setDeviceId(String vdeviceId);
extern String getDeviceId();


#endif