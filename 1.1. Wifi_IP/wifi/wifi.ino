//---------------------------------------- PRÁCTICA 4 ----------------------------------------//
// Ejercicio 1 (PARTE 1): Conéctate a la red wifi creada por el móvil como punto de acceso. Extrae tu IP
 
//wifi esp32
#include <WiFi.h>

const char* ssid     = "Aise1"; // Nombre de mi wifi
const char* password = "01234567"; // Contraseña 

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

void loop(){
}
