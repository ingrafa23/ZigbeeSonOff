#include <EEPROM.h>
#include "protocoloZigbee.h"

//flag de los estados de configuracion
unsigned char flagConectado = 0;
unsigned char flagVinculado = 0;
unsigned char flagBorrando = 0;

// indica que hay dato
unsigned char dataDisponible = 0;

//Mac del bridge
unsigned char MAC[8];
String getMacBridge(){
    return String(MAC[0], HEX) + ":" + 
                 String(MAC[1], HEX) + ":" + 
                 String(MAC[2], HEX) + ":" + 
                 String(MAC[3], HEX) + ":" + 
                 String(MAC[4], HEX) + ":" + 
                 String(MAC[5], HEX) + ":" + 
                 String(MAC[6], HEX) + ":" + 
                 String(MAC[7], HEX); 
}

//ID del sensor a vincular
unsigned ID[2];

//Mac del sensor Zigbee a vinculas
unsigned char MACID[8];

float Valor1;  // Si es M, aca va el Movimiento, si es D aca va la Temperatura//valor humedad
float Valor2;  // Si es M, esto no importa, y si es D aca va la Humedad //valor tem

char c, k = 0, total, chk = 0, i;
char mensaje[128];
float Temperatura;
char  Temperatura_Cad[10];
float Humedad;
char  Humedad_Cad[10];

// EEPROM
unsigned char Num_Vinculados;
unsigned char getNumSensorVinvulados(){
  return Num_Vinculados;
}

// TIMEOUT
unsigned long timeout = 0;
unsigned long timeoutAnterior = 0;
//----
char ID_mensaje[2],       // Vector para guardar el ID que llega en el mensaje
               ID_leido_eeprom[2];  // Vector para guardar el ID que se va leyendo de la EEPROM
char Valor_mensaje;       // Valor para guardar el Valor que llega en el Mensaje

//Union y Estructura de dispositivos vinvulados
Estructura_Sensor ZigbeeSensorMovimiento[NUM_SENSORES_MOVIMIENTO];
//----------------------------------------------------
struct protocoloZigbeeDesvincularTodo
{
  unsigned char flag;
  unsigned char cont;
}DesvincularTodo;

//-------------------------

//Funcion de enviar los comandos por el puerto serial 0 del ESP hacia el Zigbee
void Enviar_Serial(char n, ...)
{
  char t, chk = 0xFE ^ (n+3);

  va_list c;
  va_start(c, n);

  Serial.write(0xFE);
  Serial.write(n + 3);
  for(unsigned char i = 0; i < n; i++)
  {   
    t = va_arg(c,int);
    Serial.write(t);
    chk = chk ^ t;
  }
  Serial.write(chk);
}

void ConfigProtocolo()
{
  //Inicializamos el Butto
  pinMode(BUTTON,INPUT);
  pinMode(LED_BLUE,OUTPUT);
  pinMode(RESET_ZIGBEE,OUTPUT);
  digitalWrite(RESET_ZIGBEE,0);
  //Inicializando Serial 0
  Serial.begin(115200);
  //Inicializando serial 1
  Serial1.begin(115200);
  //Serial1.flush();
  Serial1.println("Soy el Serial 1");
  delay(1000);
  digitalWrite(RESET_ZIGBEE,1);
  Serial1.println("Iniciando.....");

  // Reservar el Espacio en la EEPROM 
  EEPROM.begin(128); // 128 BYTES
  Num_Vinculados = EEPROM.read(0);
  // Si no hay nada en la EEPROM supongo que lee 255
  if(Num_Vinculados == 255) 
    Num_Vinculados = 0;
  
  //la cambie
  for(char j = 0; j < NUM_SENSORES_MOVIMIENTO; j++)
  {
    if (j < Num_Vinculados)
    {
      ZigbeeSensorMovimiento[j].id[0]  = EEPROM.read( 1 + j*10);
      ZigbeeSensorMovimiento[j].id[1]  = EEPROM.read( 2 + j*10);
      ZigbeeSensorMovimiento[j].mac[0] = EEPROM.read( 3 + j*10);
      ZigbeeSensorMovimiento[j].mac[1] = EEPROM.read( 4 + j*10);
      ZigbeeSensorMovimiento[j].mac[2] = EEPROM.read( 5 + j*10);
      ZigbeeSensorMovimiento[j].mac[3] = EEPROM.read( 6 + j*10);
      ZigbeeSensorMovimiento[j].mac[4] = EEPROM.read( 7 + j*10);
      ZigbeeSensorMovimiento[j].mac[5] = EEPROM.read( 8 + j*10);
      ZigbeeSensorMovimiento[j].mac[6] = EEPROM.read( 9 + j*10);
      ZigbeeSensorMovimiento[j].mac[7] = EEPROM.read(10 + j*10);
      ZigbeeSensorMovimiento[j].valorMedida = 0;
      ZigbeeSensorMovimiento[j].estadoVinculado = 1;
    }
    else
    {
      ZigbeeSensorMovimiento[j].estadoVinculado = 0;
    }
  }

  //Print de todo los sensores Vinculados
  Serial1.print("Nº Sensores Vinvulados: "); Serial1.println(Num_Vinculados);
  for(unsigned char j = 0; j < Num_Vinculados; j++)
  {
    if (ZigbeeSensorMovimiento[j].estadoVinculado == 1)
    {
      Serial1.print("Sensor Nº: "); Serial1.println(String(j,DEC));
      String strTextprint = String(ZigbeeSensorMovimiento[j].id[0],HEX);
      strTextprint += " ";
      strTextprint += String(ZigbeeSensorMovimiento[j].id[1],HEX);
      Serial1.print("ID Sensor: "); Serial1.println(strTextprint);
      //-----Print MAC
      strTextprint = String(ZigbeeSensorMovimiento[j].mac[7],HEX);
      strTextprint += " ";
      Serial1.println(strTextprint);
      for (unsigned char mk = 7; mk > 0; mk--)
      {
        strTextprint += String(ZigbeeSensorMovimiento[j].mac[mk-1],HEX);
        strTextprint += " ";
      }
      Serial1.print("MAC Sensor: "); Serial1.println(strTextprint);
      Serial1.println("------------------------");
    }
  }
  
  //------------------------------------------------
}

//comando para desvincular todo los sensores
void comandosDesvincularTodosSensores(){
  char ID_leido_eeprom[2];
  unsigned char MAC_ID_leido_eeprom[8];
  /***************************************
   * Posiciones de la EEPROM :
   * 
   * NV  ID0 MAC0 ID1 MAC1 ID2 MAC2 ... 
   * 0    1    3   11  13   21  23  ... 
   *      2    4   12  14   22  24  ... 
   *           5       15       25  ... 
   *           6       16       26  ... 
   *           7       17       27  ... 
   *           8       18       28  ... 
   *           9       19       29  ... 
   *          10       20       30  ... 
   ***************************************/
  // Recorre la EERPOM
 if(DesvincularTodo.flag>0){
      if (Num_Vinculados > 0 )
      {
        unsigned char j = DesvincularTodo.cont;
        if (ZigbeeSensorMovimiento[j].estadoVinculado == 1)
        {
          ZigbeeSensorMovimiento[j].id[0]  = EEPROM.read( 1 + j*10);
          ZigbeeSensorMovimiento[j].id[1]  = EEPROM.read( 2 + j*10);
          ZigbeeSensorMovimiento[j].mac[0] = EEPROM.read( 3 + j*10);
          ZigbeeSensorMovimiento[j].mac[1] = EEPROM.read( 4 + j*10);
          ZigbeeSensorMovimiento[j].mac[2] = EEPROM.read( 5 + j*10);
          ZigbeeSensorMovimiento[j].mac[3] = EEPROM.read( 6 + j*10);
          ZigbeeSensorMovimiento[j].mac[4] = EEPROM.read( 7 + j*10);
          ZigbeeSensorMovimiento[j].mac[5] = EEPROM.read( 8 + j*10);
          ZigbeeSensorMovimiento[j].mac[6] = EEPROM.read( 9 + j*10);
          ZigbeeSensorMovimiento[j].mac[7] = EEPROM.read(10 + j*10);
          //Enviando comando de desvinculacion
          
          Serial1.println("DESVINCULANDO DIVICE :Recv Data: FE 12 02 17 02 I0 I1 01 0F M7 M6 M5 M4 M3 M2 M1 M0 XX");
          Enviar_Serial(15, 0x02, 0x17, 0x02, ZigbeeSensorMovimiento[j].id[0], ZigbeeSensorMovimiento[j].id[1], 0x01, 0x0F, 
                                              ZigbeeSensorMovimiento[j].mac[7], ZigbeeSensorMovimiento[j].mac[6], 
                                              ZigbeeSensorMovimiento[j].mac[5], ZigbeeSensorMovimiento[j].mac[4], 
                                              ZigbeeSensorMovimiento[j].mac[3], ZigbeeSensorMovimiento[j].mac[2], 
                                              ZigbeeSensorMovimiento[j].mac[1], ZigbeeSensorMovimiento[j].mac[0]);

          ZigbeeSensorMovimiento[j].estadoVinculado = 0;
          
          Num_Vinculados--; 
          EEPROM.write(0,Num_Vinculados);
          EEPROM.commit();
          DesvincularTodo.cont++;
        }
      }
  }
  else
  {
    DesvincularTodo.flag = 0;
  }
  
}

//comando desvincular
void comandosDesvincularSensor()
{
  char ID_leido_eeprom[2];

  Serial1.println("DESVINCULANDO DIVICE :Recv Data: FE 12 02 17 02 I0 I1 01 09 M7 M6 M5 M4 M3 M2 M1 M0 XX");
  Enviar_Serial(15, 0x02, 0x17, 0x02, ID[0], ID[1], 0x01, 0x09, MACID[7], MACID[6], MACID[5], MACID[4], MACID[3], MACID[2], MACID[1], MACID[0]);
  // Ese esta grabado en la EEPROM? 
  // Si es SI, pues hay que buscarlo en la EEPROM y borrarlo

  /***************************************
   * Posiciones de la EEPROM :
   * 
   * NV  ID0 MAC0 ID1 MAC1 ID2 MAC2 ... 
   * 0    1    3   11  13   21  23  ... 
   *      2    4   12  14   22  24  ... 
   *           5       15       25  ... 
   *           6       16       26  ... 
   *           7       17       27  ... 
   *           8       18       28  ... 
   *           9       19       29  ... 
   *          10       20       30  ... 
   ***************************************/
  // Recorre la EERPOM para determinar si ese ID esta guardado en la EEPROM
  for(char j = 0; j < Num_Vinculados; j++)
  {
    // Lee el ID de la EEPROM
    ID_leido_eeprom[0] = EEPROM.read(1 + j*10);
    ID_leido_eeprom[1] = EEPROM.read(2 + j*10);
    // Compara el ID leido de la EEPROM con el recibido en el Mensaje 
    if((ID_mensaje[0] == ID_leido_eeprom[0]) && (ID_mensaje[1] == ID_leido_eeprom[1]))
      break; // Si son iguales se sale del bucle 
  }
  /*
  if(j < Num_Vinculados) // Lo encontro
  {
    // No lo complete pues no se si sea necesario 
  }
  */
}

//Comando para iniciar una vinculacion
void comandosVincularNuevoSensor()
{
  if (!digitalRead(BUTTON))
  {  
    unsigned long lastTime = millis();
    unsigned long actualityTime = 0;
    delay(100);
    while (!digitalRead(BUTTON)) 
    {
      delay(1);
    }

    actualityTime = abs(lastTime - millis());
    Serial1.print("actualityTime : "); Serial1.println(actualityTime);
    if ((actualityTime > 500) && (actualityTime < 2000))
    {
      if(flagConectado == ESTADO_VINCULAR)
      {
        timeout = 1; // iniciar el timeout
        timeoutAnterior = millis();
        flagConectado = ESTADO_VINCULANDO;
        flagVinculado = 0;
        blinkLed(200,5);
        Serial1.println("Enviando Vinculacion : FE 0B 02 01 02 F9 FF 00 0D 37 C8");
        Enviar_Serial(8, 0x02, 0x01, 0x02, 0xF9, 0xFF, 0x00, 0x0D, 0x37);
        delay(500);
      }
    }
    else
    {
      if ((actualityTime > 4000) && (actualityTime < 10000))
      {
        blinkLed(100,10);
        //comandosDesvincularSensor();
        DesvincularTodo.flag = 1;
        DesvincularTodo.cont = 0;
        flagBorrando = 0;
      }
    }
  }
  else
  {
    digitalWrite(LED_BLUE,1);
  }
}

void loopLeerSensoresZigbee()
{
  timeout = millis();
  
  if(abs(timeout - timeoutAnterior) >= 30000) // 30 Segundos 30 * 1000  --> TIMEOUT
  {
    if(flagConectado != ESTADO_VINCULAR) // si no termino de Vincular
    {
      flagConectado = ESTADO_VINCULAR;  // Se reinicia el proceso de Vinculación
      Serial1.println("Tiempo de vinculacion superado");
    }
  }
    
      
  comandosVincularNuevoSensor();
  LeerMensaje();

  //Desvincular todo los sensores
  comandosDesvincularTodosSensores();

  for (unsigned char ki = 0; ki < Num_Vinculados; ki++)
  {
    if (abs(millis() - ZigbeeSensorMovimiento[ki].timeOut) > TIME_OUT_DES)
    {
      //Serial1.print("Sensor "); Serial1.print(ki);Serial1.println(" No conectado");
      ZigbeeSensorMovimiento[ki].estadoVinculado = 0;
      ZigbeeSensorMovimiento[ki].valorMedida = 0;
    }
    
  }
  
}

void LeerMensaje()
{
  if (Serial.available())
  { 
    c = Serial.read();
    // Inicio del Mensaje
    if((k == 0)&&(c == 0xFE))
    {
      mensaje[0] = c;
      k = 1;
      Serial1.print("Inicio del Mensaje "); 
    }
    // Tamaño del Mensaje
    else if(k == 1)
    {
      total = c;
      mensaje[1] = c;
      k = 2;
      Serial1.print("Tamaño Recibido: "); 
      Serial1.println(String(c, HEX));
    }
    // Mensaje
    else if(k > 1)
    {
      mensaje[k] = c;
      k++;
      if(k == total)
      {
        Serial1.print("Mensaje Recibido: "); 
        
        chk = ProcesarCHK();

        Serial1.print(" --> CHKSUM: ");
        Serial1.println(String(mensaje[k - 1], HEX));

        if(chk == 0)
        {
          ProcesarMensaje();
          chk = 0;
          k = 0;
        }
      }
    }
  }
}

char ProcesarCHK()
{
  char chk = 0;

  for(i = 0; i < total; i++)
  {
    Serial1.print(String(mensaje[i],HEX));
    Serial1.print(" ");
    chk = chk ^ mensaje[i];
  }

  return chk;
}

void EnviarPasswordBridge()
{
  if(mensaje[2] == 0x01)
  {
    switch(mensaje[3])
    {
      case 0x80: //Solicitando de Password
        Serial1.println("Enviando Password : FE 0A 01 01 02 00 00 00 43 B5");
        Enviar_Serial(7, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x43);
        break;
      case 0x41: // Solicitando MAC al Bridge
        Serial1.println("Enviando Solicitud del MAC Bridge : FE 0C 01 04 02 00 00 00 43 00 03 B5");
        Enviar_Serial(9, 0x01, 0x04, 0x02, 0x00, 0x00, 0x00, 0x43, 0x00, 0x03);
        break;
      case 0x44:
        MAC[7] = mensaje[12];
        MAC[6] = mensaje[13];
        MAC[5] = mensaje[14];
        MAC[4] = mensaje[15];
        MAC[3] = mensaje[15];
        MAC[2] = mensaje[17];
        MAC[1] = mensaje[18];
        MAC[0] = mensaje[19];

        Serial1.print("MAC = ");
        for(unsigned char i = 0; i < 7; i++)
        {
          Serial1.print(String(MAC[i],HEX));
          Serial1.print(":");
        }
        Serial1.println(String(MAC[7],HEX));
        flagConectado = ESTADO_VINCULAR;
        break;
    }
  }
}

void ProcesarMensaje()
{
  EnviarPasswordBridge();

  if (flagConectado>=ESTADO_VINCULANDO)
  {
    flagVinculado = 1;
		switch(mensaje[2])
		{
			case 0x02:
				switch(mensaje[3])
				{
					case 0x41:
            if (flagConectado == (ESTADO_VINCULANDO + 17)){
              Serial1.println("Vinculado");
              flagConectado = ESTADO_VINCULAR;
              //timeout = 0;   // para detener el conteo del timeout
              flagVinculado = 1;
              // Se incrementa el numero de Vinculados
              Num_Vinculados++;
              // Se guardan el numero de Vinculados, y el ID y la MACID
              EEPROM.write(0,Num_Vinculados);
              EEPROM.write((Num_Vinculados - 1) * 10 +  1, ID[0]);
              EEPROM.write((Num_Vinculados - 1) * 10 +  2, ID[1]);
              EEPROM.write((Num_Vinculados - 1) * 10 +  3, MACID[0]);
              EEPROM.write((Num_Vinculados - 1) * 10 +  4, MACID[1]);
              EEPROM.write((Num_Vinculados - 1) * 10 +  5, MACID[2]);
              EEPROM.write((Num_Vinculados - 1) * 10 +  6, MACID[3]);
              EEPROM.write((Num_Vinculados - 1) * 10 +  7, MACID[4]);
              EEPROM.write((Num_Vinculados - 1) * 10 +  8, MACID[5]);
              EEPROM.write((Num_Vinculados - 1) * 10 +  9, MACID[6]);
              EEPROM.write((Num_Vinculados - 1) * 10 + 10, MACID[7]);
              EEPROM.commit();
              ZigbeeSensorMovimiento[Num_Vinculados - 1].id[0]  = ID[0];
              ZigbeeSensorMovimiento[Num_Vinculados - 1].id[1]  = ID[1];
              ZigbeeSensorMovimiento[Num_Vinculados - 1].mac[0] = MACID[0];
              ZigbeeSensorMovimiento[Num_Vinculados - 1].mac[1] = MACID[1];
              ZigbeeSensorMovimiento[Num_Vinculados - 1].mac[2] = MACID[2];
              ZigbeeSensorMovimiento[Num_Vinculados - 1].mac[3] = MACID[3];
              ZigbeeSensorMovimiento[Num_Vinculados - 1].mac[4] = MACID[4];
              ZigbeeSensorMovimiento[Num_Vinculados - 1].mac[5] = MACID[5];
              ZigbeeSensorMovimiento[Num_Vinculados - 1].mac[6] = MACID[6];
              ZigbeeSensorMovimiento[Num_Vinculados - 1].mac[7] = MACID[7];
              ZigbeeSensorMovimiento[Num_Vinculados - 1].valorMedida = 0;
              ZigbeeSensorMovimiento[Num_Vinculados - 1].estadoVinculado = 1;
              ZigbeeSensorMovimiento[Num_Vinculados - 1].maquinaEstado = 0;
            }	
						break;
					case 0x43:
						//if ((mensaje[5] == ID[0]) && (mensaje[6]== ID[1]))
						//{
							if (flagConectado == (ESTADO_VINCULANDO + 2))
							{
								flagConectado = ESTADO_VINCULANDO + 3;
							}	
						//}
						break;
					case 0x44:
						if ((mensaje[5] == ID[0]) && (mensaje[6]== ID[1]))
						{
							if (flagConectado == (ESTADO_VINCULANDO + 4))
							{
								flagConectado = ESTADO_VINCULANDO + 5;
							}	
						}
						break;
					case 0x47:
            if ((mensaje[5] == ID[0]) && (mensaje[6]== ID[1]))
						{
							if (flagConectado == (ESTADO_VINCULANDO + 11))
							{
								flagConectado = ESTADO_VINCULANDO + 12;
							}	
						}
						break;
					case 0x82:
						if (flagConectado==ESTADO_VINCULANDO)
  					{
							ID[0] = mensaje[5];
							ID[1] = mensaje[6];

							MACID[7] = mensaje[11];
							MACID[6] = mensaje[12];
							MACID[5] = mensaje[13];
							MACID[4] = mensaje[14];
							MACID[3] = mensaje[15];
							MACID[2] = mensaje[16];
							MACID[1] = mensaje[17];
							MACID[0] = mensaje[18];

							Serial1.print("ID = "); 
							Serial1.print(String(ID[0],HEX));
							Serial1.println(String(ID[1],HEX));
							
							Serial1.print("MAC ID = ");
							for(unsigned char i = 0; i < 7; i++)
							{
								Serial1.print(String(MACID[i],HEX));
								Serial1.print(":");
							}
							Serial1.println(String(MACID[7],HEX));

							//--------------------------
							Serial1.println("Enviando iniciar configuracion : FE 0B 02 01 02 F9 FF 00 02 00 F0");

							//Enviar_Serial(8, 0x02, 0x01, 0x02, 0xF9, 0xFF, 0x00, 0x02, 0x00);
							Enviar_Serial(7, 0x02, 0x03, 0x02, ID[0], ID[1], 0x00, 0x00);

							flagConectado = ESTADO_VINCULANDO + 1;
						}
						break;
					case 0x83:
						if ((mensaje[5] == ID[0]) && (mensaje[6]== ID[1]))
						{
							if (flagConectado == (ESTADO_VINCULANDO + 3))
							{
								Serial1.println("Enviando : FE 0A 02 04 02 5A 0E 01 00 CC");
								Enviar_Serial(7, 0x02, 0x04, 0x02, ID[0], ID[1], 0x01, 0x00);
								flagConectado = ESTADO_VINCULANDO + 4;
							}	
						}
						break;
					case 0x84:
						if ((mensaje[5] == ID[0]) && (mensaje[6]== ID[1]))
						{
							if (flagConectado == (ESTADO_VINCULANDO + 5))
							{
								Serial1.println("Enviando : FE 0E 03 01 02 I0 I1 01 11 00 05 01 00 XX");
								Enviar_Serial(11, 0x03, 0x01, 0x02, ID[0], ID[1], 0x01, 0x11, 0x00, 0x05, 0x01, 0x00);
								flagConectado = ESTADO_VINCULANDO + 6;
							}	
						}
						break;
					case 0x87:
            if ((mensaje[5] == ID[0]) && (mensaje[6]== ID[1]))
						{
							if (flagConectado == (ESTADO_VINCULANDO + 12))
							{
                Serial1.println("Enviando : FE 0E 03 01 02 I0 I1 01 12 00 05 02 00 XX");
							  Enviar_Serial(11, 0x03, 0x01, 0x02, ID[0], ID[1], 0x01, 0x12, 0x00, 0x05, 0x02, 0x00);
								flagConectado = ESTADO_VINCULANDO + 13;
							}	
						}				
						break;
					case 0x95:
						break;
				}
			case 0x03:
				switch (mensaje[3])
				{
				case 0x41:
					if ((mensaje[5] == ID[0]) && (mensaje[6]== ID[1]))
					{
						if (flagConectado == (ESTADO_VINCULANDO + 6))
						{
							Serial1.println("Enviando : FE 0A 04 01 02 I0 I1 01 86 XX");
							Enviar_Serial(8, 0x04, 0x01, 0x02, ID[0], ID[1], 0x01, 0x86, 0X00);
							flagConectado = ESTADO_VINCULANDO + 7;
						}
            else
            {
              
              if (flagConectado == (ESTADO_VINCULANDO + 13))
              {
                Serial1.println("Enviando : FE 0C 04 10 02 I0 I1 01 13 00 00 XX");
                Enviar_Serial(9, 0x04, 0x10, 0x02, ID[0], ID[1], 0x01, 0x13, 0x00, 0x00);
                flagConectado = ESTADO_VINCULANDO + 14;
              }	
            }
					}
					break;
				case 0x81:
					if ((mensaje[5] == ID[0]) && (mensaje[6]== ID[1]))
					{
						if (flagConectado == (ESTADO_VINCULANDO + 8))
						{
              Serial1.println("Enviando : FE 1F 02 07 02 I0 I1 00 05 M7 M6 M5 M4 M3 M2 M1 M0 01 02 04 03 E1 CD 45 FE FF 23 A4 60 01 XX");
                                        //  FE 1F 02 07 02 2C 82 00 10 C0 82 67 1F 00 4B 12 00 01 01 00 03 E1 CD 45 FE FF 23 A4 60 01 A4
              Enviar_Serial(28, 0x02, 0x07, 0x02, ID[0], ID[1], 0x00, 0x05, MACID[7], MACID[6], MACID[5], MACID[4], MACID[3], MACID[2], MACID[1], MACID[0], 0x01, 0x02, 0x04, 0x03, 0xE1, 0xCD, 0x45, 0xFE, 0xFF, 0x23, 0xA4, 0x60, 0x01);
              flagConectado = ESTADO_VINCULANDO + 12; 
            //flagConectado = ESTADO_VINCULANDO + 9;
						}
            else{
              if (flagConectado == (ESTADO_VINCULANDO + 15))
              {
                flagConectado = ESTADO_VINCULANDO + 16;
              }	
            }	
					}
					break;
				}
				break;
			case 0x04:
				switch(mensaje[3])
				{
					case 0x41:
						if ((mensaje[5] == ID[0]) && (mensaje[6]== ID[1]))
						{
							if (flagConectado == (ESTADO_VINCULANDO + 7))
							{
								flagConectado = ESTADO_VINCULANDO + 8;
							}	
						}
						break;
					case 0x50:
            if ((mensaje[5] == ID[0]) && (mensaje[6]== ID[1]))
						{
							if (flagConectado == (ESTADO_VINCULANDO + 14))
							{
								flagConectado = ESTADO_VINCULANDO + 15;
							}	
						}
            break;
					case 0x81:
						if ((mensaje[5] == ID[0]) && (mensaje[6]== ID[1]))
						{
							if (flagConectado == (ESTADO_VINCULANDO + 9))
							{
                Serial1.println("Enviando : FE 1F 02 07 02 I0 I1 00 05 M7 M6 M5 M4 M3 M2 M1 M0 01 02 04 03 E1 CD 45 FE FF 23 A4 60 01 XX");
                                        //  FE 1F 02 07 02 2C 82 00 10 C0 82 67 1F 00 4B 12 00 01 01 00 03 E1 CD 45 FE FF 23 A4 60 01 A4
								Enviar_Serial(28, 0x02, 0x07, 0x02, ID[0], ID[1], 0x00, 0x05, MACID[7], MACID[6], MACID[5], MACID[4], MACID[3], MACID[2], MACID[1], MACID[0], 0x01, 0x02, 0x04, 0x03, 0xE1, 0xCD, 0x45, 0xFE, 0xFF, 0x23, 0xA4, 0x60, 0x01);
								flagConectado = ESTADO_VINCULANDO + 12;
							}	
						}
						break;
					case 0x90:
            if ((mensaje[5] == ID[0]) && (mensaje[6]== ID[1]))
              {
                if (flagConectado == (ESTADO_VINCULANDO + 16)){
                  Serial1.println("Enviando : FE 0B 02 01 02 F9 FF 00 14 00 XX");
                  Enviar_Serial(8, 0x02, 0x01, 0x02, 0xF9, 0xFF, 0x00, 0x14, 0x00);
                  flagConectado = ESTADO_VINCULANDO + 17;
                }	
              }
						break;
				}
				break;
			case 0x09:
				if(mensaje[3] == 0x83)
				{
					if((mensaje[5] == ID[0]) && (mensaje[6]== ID[1])){
						if (flagConectado == (ESTADO_VINCULANDO + 1))
						{//nuevo
							Serial1.println("Enviando : FE 0A 02 03 02 ID[0] ID[0] 00 00 XX");
							Enviar_Serial(7, 0x02, 0x03, 0x02, ID[0], ID[0], 0x00, 0x00);
							flagConectado = ESTADO_VINCULANDO + 2;
						}else
						{
							if (flagConectado == (ESTADO_VINCULANDO + 10))
							{
								Serial1.println("Enviando : FE 1F 02 07 02 I0 I1 00 05 M7 M6 M5 M4 M3 M2 M1 M0 01 02 04 03 E1 CD 45 FE FF 23 A4 60 01 XX");
                                        //  FE 1F 02 07 02 2C 82 00 10 C0 82 67 1F 00 4B 12 00 01 01 00 03 E1 CD 45 FE FF 23 A4 60 01 A4
								Enviar_Serial(28, 0x02, 0x07, 0x02, ID[0], ID[1], 0x00, 0x05, MACID[7], MACID[6], MACID[5], MACID[4], MACID[3], MACID[2], MACID[1], MACID[0], 0x01, 0x02, 0x04, 0x03, 0xE1, 0xCD, 0x45, 0xFE, 0xFF, 0x23, 0xA4, 0x60, 0x01);
								flagConectado = ESTADO_VINCULANDO + 11;
							}	
						}
					}
					else
					{
            //Aqui se lee la data
            Serial1.print("ID :"); Serial1.print(String(ID[0],HEX));  Serial1.print(String(ID[1],HEX));
						Serial1.print(" Valor SM:");
						Serial1.println(String(mensaje[9],HEX));
					}          
				}
				break;
		}
	}
  else
  {
    switch(mensaje[2])
		{
      case 0x02:
        switch (mensaje[3])
        {
        //estado borrando
        case 0x57:
          if (flagBorrando==1)
          {
            flagBorrando = 2;
          } 
        break;

        case 0x95:
          if (flagBorrando==2)
          {
            flagBorrando = 0;
          } 
        break;
        
        default:
          break;
        }
      break;
      
			case 0x09:
      if(mensaje[3]==0x83)
      {
        if(Num_Vinculados > 0)
        {
          /*
          char ID_mensaje[2],       // Vector para guardar el ID que llega en el mensaje
               ID_leido_eeprom[2];  // Vector para guardar el ID que se va leyendo de la EEPROM
          char Valor_mensaje;       // Valor para guardar el Valor que llega en el Mensaje
          */

          ID_mensaje[0] = mensaje[5];  // Lee el ID del Mensaje
          ID_mensaje[1] = mensaje[6];
          Valor_mensaje = mensaje[9];  // Leer el Valor del Mensaje

          /***************************************
           * Posiciones de la EEPROM :
           * 
           * NV  ID0 MAC0 ID1 MAC1 ID2 MAC2 ... 
           * 0    1    3   11  13   21  23  ... 
           *      2    4   12  14   22  24  ... 
           *           5       15       25  ... 
           *           6       16       26  ... 
           *           7       17       27  ... 
           *           8       18       28  ... 
           *           9       19       29  ... 
           *          10       20       30  ... 
           ***************************************/
          // Recorre la EERPOM para determinar si ese ID esta guardado en la EEPROM
          for(char j = 0; j < Num_Vinculados; j++)
          {
            // Lee el ID de la EEPROM
            ID_leido_eeprom[0] = EEPROM.read(1 + j*10);
            ID_leido_eeprom[1] = EEPROM.read(2 + j*10);
            // Compara el ID leido de la EEPROM con el recibido en el Mensaje 
            if((ID_mensaje[0] == ID_leido_eeprom[0]) && (ID_mensaje[1] == ID_leido_eeprom[1]))
            {
              // Si son iguales Imprime el ID y el Valor
              Serial1.print("ID :"); 
              Serial1.print(String(ID_mensaje[0],HEX)); 
              Serial1.print(String(ID_mensaje[1],HEX)); 
              Serial1.print(" Valor SM: ");
              Serial1.println(String(Valor_mensaje,HEX));
              ZigbeeSensorMovimiento[j].valorMedida = Valor_mensaje;
              ZigbeeSensorMovimiento[j].estadoVinculado = 1;
              //Actualizamos timeOut
              ZigbeeSensorMovimiento[j].timeOut = millis();
            }
            // Si no son iguales no imprime nada
          }
        }
      }
      break;
    }
  }
  Serial1.print("flagConectado : "); Serial1.println(flagConectado);
}

void blinkLed(unsigned int mT, unsigned char mParp){
  for (unsigned char ki = 0; ki < 2*mParp; ki++)
  {
    digitalWrite(LED_BLUE,!digitalRead(LED_BLUE));
    delay(mT);
  }
}

/*
//EEPROM
#include <EEPROM.h>

EN EL SETUP SE DE DEBE RESERVAR CUANTOS BYTES
EEPROM.begin(128); --> 128 BYTES

// PARA ESCRIBIR
EEPROM.write(posicion, valor);
EEPROM.commit();

// leer
lectura = EEPROM.read(posicion);
*/