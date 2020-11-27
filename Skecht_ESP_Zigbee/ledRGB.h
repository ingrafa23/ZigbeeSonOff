/*Libreria
*/
#ifndef ledrgb_h
#define ledrgb_h
// Libreria asociada a las rutinas basicas de arduino
#include <Arduino.h>
#include <inttypes.h>
//Pin Data Ring LED
#define pinRingLedRGB        2 //
//Numeros de LED's
#define NUMPIXELS 24 // Popular NeoPixel ring size
#define DELAYLEDWait 250 // Time (in milliseconds) to pause between pixels

class ledRGB
{
public:
	ledRGB();
	void Setup();
	void setColorRing(uint8_t R,uint8_t G,uint8_t B,int delay_blink, bool mshow);
	void Wait(uint8_t R,uint8_t G,uint8_t B);
	void Reserved(uint8_t R,uint8_t G,uint8_t B);
	void Validate(uint8_t R,uint8_t G,uint8_t B,int delay_validate);
	void Busy(uint8_t R,uint8_t G,uint8_t B);
	void stop();
	void run();
	uint8_t getStatus();

private:
	volatile uint8_t flagLed;

};




#endif
