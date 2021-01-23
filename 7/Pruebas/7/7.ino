#include <ArduinoJson.h>
double valueTemp=0;
StaticJsonDocument<1000> doc; 


void setup() {
  // Initialize Serial port
  Serial.begin(115200);
  
}

void loop() {
 
  doc["n"] = "temp";
  doc["u"] = "cel";
  JsonArray data = doc.createNestedArray("v");   
  for(int i=0;i<10;i++){
    valueTemp=random(0,25);    
    data.add(valueTemp); 
    delay(500);
  }
 
  serializeJsonPretty(doc, Serial);

  
  delay(2000);
}
