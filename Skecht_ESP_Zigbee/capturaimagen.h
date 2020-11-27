#ifndef CAPTURA_IMAGE_INCLUDE
#define CAPTURA_IMAGE_INCLUDE

#include <string.h>
#include <Arduino.h>

class capturaimagen
{
private:
    /* data */
    unsigned char statusError;
    String dataImageEncodingBase64;
    unsigned char dataImageEncodingBase64Status;
public:
    capturaimagen(/* args */);
    ~capturaimagen();
    void begin();
    unsigned char getStatusError();
    void capturaImagen();
    String encodingB64Imagen(uint8_t *data_pic,size_t size_pic);

    unsigned char getAvailableEncodingB64Imagen();
    String getdataEncodingB64Imagen();
    
};






#endif