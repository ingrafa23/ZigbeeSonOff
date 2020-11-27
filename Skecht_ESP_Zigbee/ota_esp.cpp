#include "ota_esp.h"

// includes

#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#endif


#include <WiFiUdp.h>
#include <WiFiManager.h>

//-------configuracion OTA------------------------
#include <ArduinoOTA.h>
//-----------------------------------





ota_esp::ota_esp(){

}


//funciones de inicialización
void ota_esp::begin(){

	//pinMode(LedStatus,OUTPUT);
	//digitalWrite(LedStatus,0);
	//tickerLed.attach(2, parpadeoLed);
	beginWifiManager();
	//Begin WiFi And OTA --------------------------------
	beginOta();
}

void ota_esp::detec(){

	if (WiFi.status() == WL_CONNECTED){
		ArduinoOTA.handle();
	}
	else
	{
		Serial.print("Reconectando WIFI : "); Serial.println(String(WiFi.SSID()));
		WiFi.begin();
	}
	

  
}

void ota_esp::beginWifiManager(){
	#ifdef ESP32
		disableCore0WDT();
		disableCore1WDT();
	#endif

  	#ifdef ESP8266
  		ESP. wdtDisable(); 
  	#endif

	//WiFi.disconnect();
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    //WiFi.config(ip, gateway, subnet);

    //WiFi.mode(WiFi_STA); // it is a good practice to make sure your code sets wifi mode how you want it.
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;
    //reset settings - wipe credentials for testing
    //wm.resetSettings();
    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result
    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect(HOSTNAME,PASSWORD_OTA_WIFI_LOCAL); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        WiFi.disconnect();
        wm.resetSettings();
        ESP.restart();
    }
    
    #ifdef ESP8266
	if (MDNS.begin(HOSTNAME)) {
		  WiFi.hostname(HOSTNAME);
	   Serial.println("mDNS responder started");
	   MDNS.addService("http", "tcp", 80);
	  }
	  
	#endif

   #ifdef ESP32
  if (MDNS.begin(HOSTNAME)) {
 
      Serial.println("mDNS responder started");
      MDNS.addService("http", "tcp", 80);
    }
  #endif

}

void ota_esp::beginOta(){
	this->flag_wifi_ota =0;;
	// Check WiFi connection
	

	// Hostname Ota
	ArduinoOTA.setHostname(HOSTNAME);

	// No authentication by default
	//ArduinoOTA.setPassword(PASSWORD_OTA_WIFI_LOCAL);

	// Check connection
	if (WiFi.status() == WL_CONNECTED) {
		
		ArduinoOTA.onStart([]() {
	    String type;
	    if (ArduinoOTA.getCommand() == U_FLASH) {
	      type = "sketch";
	    } else { // U_FS
	      type = "filesystem";
	    }

	    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
	    Serial.println("Start updating " + type);
	  });
	  ArduinoOTA.onEnd([]() {
	    Serial.println("\nEnd");
	  });
	  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
	    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	  });
	  ArduinoOTA.onError([](ota_error_t error) {
	    Serial.printf("Error[%u]: ", error);
	    if (error == OTA_AUTH_ERROR) {
	      Serial.println("Auth Failed");
	    } else if (error == OTA_BEGIN_ERROR) {
	      Serial.println("Begin Failed");
	    } else if (error == OTA_CONNECT_ERROR) {
	      Serial.println("Connect Failed");
	    } else if (error == OTA_RECEIVE_ERROR) {
	      Serial.println("Receive Failed");
	    } else if (error == OTA_END_ERROR) {
	      Serial.println("End Failed");
	    }
	  });

		ArduinoOTA.begin();

		this->flag_wifi_ota = 1;
		//end OTA server ------------------------------------

	} //Aca enviar una alerta al panel de control
	else{
		
		this->flag_wifi_ota = 0;  
	}
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	
}
