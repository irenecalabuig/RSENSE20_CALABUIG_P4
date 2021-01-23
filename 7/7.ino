//----------------------------------------- PRÁCTICA 4 ------------------------------------------//
// Ejercicio 7. Basándote en el estándar SENML crea un fichero jsonque se genere cada 10 segundos,
//              que contenga datos de temperatura inventados y las unidades

#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClient.h> 
#include <ESP32_FTPClient.h>
#include "octocat.h"

#define WIFI_SSID "RSense"   // Wifi de la clase
#define WIFI_PASS ""

char documento[1000];

char ftp_server[] = "155.210.150.77";  // ip ftp_servidor
char ftp_user[]   = "rsense";          // usuario ftp_servidor
char ftp_pass[]   = "rsense";          // constraseña ftp_servidor

// you can pass a FTP timeout and debbug mode on the last 2 arguments
ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass, 5000, 2);

#include <ArduinoJson.h>
double valueTemp=0;
StaticJsonDocument<1000> doc; 

void setup()
{
  Serial.begin( 115200 );

  WiFi.begin( WIFI_SSID, WIFI_PASS );
  
  Serial.println("Connecting Wifi...");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
  doc["n"] = "temp";
  doc["u"] = "cel";
  JsonArray data = doc.createNestedArray( "v");   
  for(int i=0;i<10;i++){
    valueTemp=random(0,25);    
    data.add(valueTemp); 
    delay(500);
  }
  serializeJsonPretty(doc, documento);
  ftp.OpenConnection();
  // Create the file new and write a string into it
  ftp.ChangeWorkDir("/rsense/icalabuig");   // directorio a guardar
  ftp.InitFile("Type A");
  ftp.NewFile("temperatura.json");          // crear archivo json
  ftp.Write(documento);
  ftp.CloseFile();
  ftp.CloseConnection();
}

void loop()
{

}
