#ifndef BUFFER_FIFO__STRING_INCLUDE
#define BUFFER_FIFO__STRING_INCLUDE

#include <string.h>
#include <Arduino.h>

#define MAX_BUFFER_INTERPRTE  8

class bufferfifostring
{
private:
    /* data */
    

    String bufferString[MAX_BUFFER_INTERPRTE];
    unsigned int posIn; 
    unsigned int posOut;

public:
    bufferfifostring(/* args */);
    ~bufferfifostring();

    void setStringBuffer(String myString);
    String getStringdBuffer();
    char statusBuffer();
};




#endif