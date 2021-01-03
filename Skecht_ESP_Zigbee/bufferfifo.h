#ifndef BUFFER_FIFO_INCLUDE
#define BUFFER_FIFO_INCLUDE

#include <string.h>
#include <Arduino.h>

#define MAX_BUFFER_INTERPRTE  16

class bufferfifo
{
    private:
        unsigned char bufferOrdenesInterprete[MAX_BUFFER_INTERPRTE];
        unsigned char bufferNumSensor[MAX_BUFFER_INTERPRTE];
        
        unsigned char posIn; 
        unsigned char posOut;

    public:
        bufferfifo(/* args */);
        ~bufferfifo();

        void fillBuffer(unsigned char command, unsigned char _numSensor);
        
        unsigned char getcomandBuffer(unsigned char *_command,unsigned char *_numSensor);
        char statusBuffer();
};

#endif