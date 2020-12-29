#include "bufferfifiString.h"

bufferfifostring::bufferfifostring(/* args */)
{
    posIn  = 0; 
    posOut = 0;
}

bufferfifostring::~bufferfifostring()
{
}

/**
 * @brief fillComandBuffer llena el buffer
 * @param command es el valor asociado a un comando que se guarda en el buffer interprete
*/
void bufferfifostring::setStringBuffer(String myString)
{
    bufferString[posIn++] = myString;
    
    if (posIn>=MAX_BUFFER_INTERPRTE)
    {
        posIn = 0;
    }
}

/**
 * @brief getcomandBuffer llena el buffer
 * @return retonar true si hay data de interes, de lo contraio un false, por que no existe datos
*/
String bufferfifostring::getStringdBuffer()
{
    String command;

    command = bufferString[posOut++];

    if (posOut >= MAX_BUFFER_INTERPRTE)
    {
        posOut = 0;
    }

    return command;
}

char bufferfifostring::statusBuffer()
{
    return posOut != posIn;
}
