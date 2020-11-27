#include "ledRGB.h"
//Configuracion de Puertosy Pines I/0
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels(NUMPIXELS, pinRingLedRGB, NEO_GRB + NEO_KHZ800);

ledRGB::ledRGB(){

};
//Configuración inicial
void ledRGB::Setup(){
	// INITIALIZE NeoPixel strip object (REQUIRED)
	pixels.begin();
	pixels.show();
}

//Secuencia WAIT-----------------
void ledRGB::Wait(uint8_t R,uint8_t G,uint8_t B){
	//delay_wait = delay_wait/20;
	if(this->flagLed){
		setColorRing(R,G,B,100,1);
		this->flagLed=0;
	}
}

//Secuencia Reservado
void ledRGB::Reserved(uint8_t R,uint8_t G,uint8_t B){
	Wait(R,G,B);
}
//Secuencia Validate
void ledRGB::Validate(uint8_t R,uint8_t G,uint8_t B,int delay_validate){
	//delay_wait = delay_wait/20;

	if(this->flagLed){
		setColorRing(R,G,B,delay_validate,1);
		this->flagLed=0;
	}		
}

//Secuencia Reservado
void ledRGB::Busy(uint8_t R,uint8_t G,uint8_t B){

	Wait(R,G,B);
}

//---------------------------------
//Secuencia estado disponible
void ledRGB::setColorRing(uint8_t R,uint8_t G,uint8_t B,int delay_blink, bool mshow){
	//pixels.clear(); // Set all pixel colors to 'off'

	for(int i=0; i<NUMPIXELS; i++) { 
		// For each pixel...
		// pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
		// Here we're using a moderately bright green color:
		pixels.setPixelColor(i, pixels.Color(R, G, B));
		pixels.show();   // Send the updated pixel colors to the hardware.
		if(mshow){
			delay(delay_blink); // Pause before next pass through loop
		};
	}
}

//------------------------

void ledRGB::run(){
	this->flagLed = 1;
	//Serial.print("this->flagLed = ");Serial.println(getStatus());
}

void ledRGB::stop(){
	this->flagLed = 0;
	//Serial.print("this->flagLed = ");Serial.println(getStatus());
	delay(10);
}

uint8_t ledRGB::getStatus(){
	return this->flagLed;
}

