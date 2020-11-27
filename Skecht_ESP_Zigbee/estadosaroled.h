#ifndef ESTADO_ARO_LED_INCLUDE
#define ESTADO_ARO_LED_INCLUDE

#define ESTADO_UNREGISTER_ARO_LED 0
#define ESTADO_REGISTER_ARO_LED 1
#define ESTADO_RESERVED_ARO_LED 2
#define ESTADO_BUSY_ARO_LED 3
#define ESTADO_VALIDATIO_ARO_LED 4

extern unsigned char estadoAroLed;
extern unsigned char isEstadoCambioAroLed;
extern void setEstadoAroLed(unsigned char mEstado);
extern unsigned char getEstadoAroLed();


#endif