/*Libreria
*/
#ifndef ota_esp_h
#define ota_esp_h
// Libreria asociada a las rutinas basicas de arduino
#include <Arduino.h>
#include <inttypes.h>
#define HOSTNAME "device-infinito" ///< Hostename. The setup function adds the Chip ID at the end.
#define PASSWORD_OTA_WIFI_LOCAL "123456789"

class ota_esp
{
public:
	ota_esp();
	void begin();
	void detec();
	

private:

	void beginWifiManager();
	void beginOta();
	volatile uint8_t flag_wifi_ota;
	
};

#endif
