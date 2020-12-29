/*Libreria
*/
#ifndef requestpost_h
#define requestpost_h
// Libreria asociada a las rutinas basicas de arduino
#include <Arduino.h>
#include <inttypes.h>


#define TIMEOUT 2000

class requestpost
{
	public:
		requestpost();
		void begin(const char *mServer,uint16_t mPort);
		void sendJson(String _httpservice,String _mJson);
		String readJson();
		String requestPOSTJson(String httpservice,String _mJson);
		uint8_t available();
		uint16_t Code_request;
		
	private:
		void callbackJson();
		String headerJson(String httpservice,size_t length);
		String bodyJson(String mjson );
		uint8_t flag_send;
		uint8_t flag_available;
		const char *mSERVER;
		uint16_t mPORT;
		String httpservice;
		String IOMessegue;
};

#endif
