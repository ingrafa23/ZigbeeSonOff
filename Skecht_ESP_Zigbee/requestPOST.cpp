#include "requestpost.h"

// includes
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

requestpost::requestpost()
{

}

//funciones de inicialización
void requestpost::begin(const char *mServer,uint16_t mPort)
{
	this->mSERVER = mServer;
	this->mPORT = mPort;
}

String requestpost::requestPOSTJson(String httpservice,String _mJson)
{
  String serverRes;
  String bodyPic   =  bodyJson(_mJson);
  size_t allLen    = bodyPic.length();
  String headerTxt =  headerJson(httpservice,allLen);

  WiFiClientSecure client;
  if (!client.connect(mSERVER,mPORT)) 
  {
   return("{ \"connection\" : \"failed\"}");   
  }
  client.print(headerTxt+bodyPic);
   

  delay(20);
  long tOut = millis() + TIMEOUT;
  while(client.connected() && tOut > millis()) 
  {
   if (client.available()) 
   {
     while (client.available()) 
     {
       char c = client.read();
       serverRes += String(c);
     }
   }
  }
       
  return serverRes;
}

String requestpost::headerJson(String httpservice,size_t length)
{
  String  datam;
  datam =  F("POST ");
  datam += httpservice;
  datam += F(" HTTP/1.1\r\n");
  
  datam += F("Host: ");
  datam += this->mSERVER;
  datam += F("\r\n");
  
  datam += F("Content-Type: application/json");
  datam += "\r\n";
  datam += F("content-length: ");
  datam += String(length);
  datam += "\r\n";
  datam += "\r\n";
  
  return(datam);
}

String requestpost::bodyJson(String mjson )
{
  String datam;
  datam = mjson;
  return(datam);
  
 }

void requestpost::sendJson(String _httpservice,String _mJson){
	this-> httpservice = _httpservice;
	this-> IOMessegue = _mJson;
	this-> flag_send = true;

	
	this->callbackJson();
	

}

void requestpost::callbackJson(){
	flag_available = false;
	if(this-> flag_send == true){
		IOMessegue = requestPOSTJson(this-> httpservice,this-> IOMessegue);

    Serial1.println("--------------------------------------------------");
    Serial1.print("IOMessegue "); Serial1.println(IOMessegue);
    Serial1.println("--------------------------------------------------");

		int posIn = IOMessegue.indexOf("HTTP/1.1 ");
		int posOut = IOMessegue.indexOf("\n",posIn);
		
		
		String stringAux= "";
		for(int i=posIn+9; i<posOut-2; i++){
		    stringAux += IOMessegue[i];
		}
		this->Code_request = stringAux.toInt();;

		posIn = IOMessegue.indexOf("{");
		posOut = IOMessegue.length();

		if(this->Code_request==200){
			stringAux= "";
			for(int i=posIn; i<posOut; i++){
			    stringAux += IOMessegue[i];
			}
		}

		IOMessegue = stringAux;
		flag_available = true;
		this-> flag_send = false;
	}
}

String requestpost::readJson(){
	flag_available = 0;
	return IOMessegue;
}

uint8_t requestpost::available(){
	return flag_available;
}