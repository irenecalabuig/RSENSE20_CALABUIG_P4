//------------------------------------- PRÁCTICA 4 -------------------------------------//
// Ejercicio 3:  3. Monta un chat una aplicación software PC 

#include <WiFi.h>
 
const char* ssid     = "Aise1";           // Nombre de mi wifi
const char* password = "01234567";        // Contraseña 

 
const uint16_t port = 8091;
const char * host = "127.0.0.1";          // IP ADDRESS
 
void setup()
{
  Serial.begin(115200); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  } 
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
 
}
 
void loop()
{
    WiFiClient client;
     if (!client.connect(host, port)) {
      Serial.println("Connection to host failed"); 
      delay(1000);
      return;
    }
 
    Serial.println("Connected to server successful!"); 
    client.print("Hello from ESP32!"); 
    Serial.println("Disconnecting...");
    client.stop(); 
    delay(10000);
}
