#include "configuracionesdevice.h"

 unsigned long valueHeartbeat = 20000;
 String spaceId = "None";
 String deviceId = "f9b0e098-0928-4473-a6eb-52e623508d51";

 void setValueHeartbeat(unsigned long vTime)
 {
     valueHeartbeat = vTime*1000;
 }
 unsigned long getValueHeartbeat()
 {
     return valueHeartbeat;
 }

 void setSpaceId(String vspaceId)
 {
     spaceId = vspaceId;
 }
 String getSpaceIdt()
 {
     return spaceId;
 }

 void setDeviceId(String vdeviceId)
 {
     deviceId = vdeviceId;
 }

 String getDeviceId()
 {
     return deviceId;
 }