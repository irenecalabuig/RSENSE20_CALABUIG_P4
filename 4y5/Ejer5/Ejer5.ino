//------------------------------------------- PRÁCTICA 4 --------------------------------------------//
// Ejercicios 4 y 5:  Sustituye uno de los extremos por el módulo hardware siendo cliente y
//                    envía cada segundo tu hora local. Cuando se le mande “start” empiece
//                    a mandar la hora hasta que se le mande “stop”
#include <WiFi.h>
#include "time.h"

const char* ssid     = "Aise1";                // Nombre de mi wifi
const char* password = "01234567";             // Contraseña 
 
const char* ssidPC       = "PC-IRENE";         // Red creada ordenador
const char* passwordPC   = "01234567";

const char* ntpServer = "europe.pool.ntp.org"; // Servidor horario
const long  gmtOffset_sec = 3600;              // Compensar UTC para mi zona horaria en ms
const int   daylightOffset_sec = 3600;         // Compensar la luz del día 

const uint16_t port = 21;
const char * host = "192.168.137.1";
String dataSockt;
boolean enviaHora = false;

tm timeinfo;

WiFiClient client;

void printLocalTime()
{
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void conexionWifi(const char* ssid, const char* password){
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
}

void recibeData(){
  char c[5];
  int i = 0;
  if (client){                      //client devuelve un valor booleano True si esta conectado
    while (client.available()>0) {
      c[i] = client.read();
      i++;
    }
    dataSockt = String(c);
  }
}

void setup()
{
  Serial.begin(115200);
 
  //Conexión Wifi con Internet
  conexionWifi(ssidMovil, passwordMovil);
  
  //init and get the time
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);  
  //Me desconecto de la red Wifi del movil
  WiFi.disconnect(true);
  
  //Conexión Wifi en red local con el PC
  conexionWifi(ssidPC, passwordPC);

  delay(50);
  
  client.connect(host, port);
  while (!client){
    Serial.println("Conectando al socket...");
    client.connect(host, port);
  }
  if (client) {
    Serial.println("Socket abierto");
  }
  //WiFi.mode(WIFI_OFF);
}

void loop(){
  
  recibeData();
  if (dataSockt.startsWith("start")){
    enviaHora = true;
  } else if (dataSockt.startsWith("stop")){
    enviaHora = false;
  }

  if (enviaHora){
    delay(1000);    // enviar la fecha cada segundo
    getLocalTime(&timeinfo);
    client.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  }
}
