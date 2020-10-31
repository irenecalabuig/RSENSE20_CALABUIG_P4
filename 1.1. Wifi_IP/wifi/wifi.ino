
 
//wifi esp32
#include <WiFi.h>

const char* ssid     = "Aise"; // Nombre de mi wifi
//const char* password = ""; // Contraseña de mi casa

void setup()
{
    Serial.begin(115200);
    delay(10);

    Serial.println();
    Serial.print("Conectando a: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("Conectado a wifi");
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
}
