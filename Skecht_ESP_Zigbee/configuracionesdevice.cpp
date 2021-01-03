#include "configuracionesdevice.h"

 unsigned long valueHeartbeat = 60000;
 String spaceId = "None";
 String deviceId = "f9b0e098-0928-4473-a6eb-52e623508d51";
 String customerId = "None";

 String getCustomerId(){
     return customerId;
 }

 void setCustomerId(String m){
     customerId = m;
 }

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