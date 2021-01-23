
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "time.h"

const char* ssid = "Aise1";                  // Nombre de mi wifi
const char* password = "01234567";           // Contraseña 


const char* ntpServer = "pool.ntp.org";      // Servidor horario
const long  gmtOffset_sec = 3600;            // Compensar UTC para mi zona horaria en ms
const int   daylightOffset_sec = 3600;       // Compensar la luz del día 

boolean pulsador = false;
tm timeinfo;

String Hora;                                 // Variable para almacenar la hora completa
String horas;                                // Varible para almacenar solo las horas
String minutos;                              // Variable para almacenar solo los minutos

AsyncWebServer server(80);                   // Create AsyncWebServer object on port 80

String processor(const String& var){
  Serial.println(var); 
  Hora = get_hora(); 
  if(var == "STATE"){
    
    if(pulsador == true){                    // Al pulsar el botón: reseteo la hora
     Hora = "00:00";    
    }
    return Hora;
  }
  return String();
}
 
void setup(){
 
  // Serial port for debugging purposes
  Serial.begin(115200);
  //pinMode(ledPin, OUTPUT);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    pulsador = true;    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Start server
  server.begin();
}

String get_hora(){
  getLocalTime(&timeinfo);
  horas = timeinfo.tm_hour; 
  minutos = timeinfo.tm_min; 
  Hora = horas+":"+minutos;
  return Hora;
}

void loop(){  
}
