//----------------------------------------------- PRÁCTICA 4 -----------------------------------------------//
// Ejercicio 2:  Pon en hora el módulo mediante un servidor NTP (Network Time Protocol) 

#include <WiFi.h>
#include "time.h"

const char* ssid     = "Aise1";           // Nombre de mi wifi
const char* password = "01234567";        // Contraseña 

const char* ntpServer = "pool.ntp.org";   // Servidor horario
const long  gmtOffset_sec = 3600;         // Compensar UTC para mi zona horaria en ms
const int   daylightOffset_sec = 3600;    // Compensar la luz del día 

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }  
  Serial.println(&timeinfo, " %H:%M:%S");  // para imprimir sólo la hora
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); // inicializar el cliente NTP 
                                                            // obtener la fecha y la hora de un servidor NTP.
  printLocalTime();                                         // imprimir hora actual.

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  delay(1000);
  printLocalTime();                                         // imprimir hora actual.
}
