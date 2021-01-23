//------------------------------- PRÁCTICA 4 --------------------------------//
// Ejercicio 8. Subir datos al feed de Adafruit mediante MQTT

/*************************** Adafruit IO Config ***************************/

#define IO_USERNAME  "irenecalabuig"
#define IO_KEY       "aio_aGqm99xSQvRo9KZqUYC6jc4vHmks"

/****************************** ACELERÓMETRO ******************************/
#include <Wire.h> //librería para conectar por I2C
 
//Direccion I2C de la IMU
#define MPU 0x68
 
//Ratios de conversion
#define A_R ((32768.0/2.0)/9.8) // (32768/2)/9.8
 
 //MPU-6050 da los valores en enteros de 16 bits
//Valores RAW
int16_t AcX, AcY, AcZ;
double AcXX, AcYY, AcZZ;

const int pinSDA = 17; // Pin de conexión SDA (MPU6050) y pin 17 ESP32
const int pinSCL = 5;  // Pin de conexión SCL (MPU6050) y pin 5 ESP32
const int pinLED = 12; // Pin de conexión LED

/******************************** WIFI ********************************/
#include "AdafruitIO_WiFi.h"

#define WIFI_SSID "Aise1"       // Nombre de mi wifi
#define WIFI_PASS "01234567"    // Contraseña

#if defined(USE_AIRLIFT) || defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE) ||         \
    defined(ADAFRUIT_PYPORTAL)
// Configure the pins used for the ESP32 connection
#if !defined(SPIWIFI_SS) // if the wifi definition isnt in the board variant
// Don't change the names of these #define's! they match the variant ones
#define SPIWIFI SPI
#define SPIWIFI_SS 10 // Chip select pin
#define NINA_ACK 9    // a.k.a BUSY or READY pin
#define NINA_RESETN 6 // Reset pin
#define NINA_GPIO0 -1 // Not connected
#endif

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS, SPIWIFI_SS,
                   NINA_ACK, NINA_RESETN, NINA_GPIO0, &SPIWIFI);
#else
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
#endif

#include "config.h"

#define TIEMPO_ESPERA 5000 // Cada cuánto leer del sensor

AdafruitIO_Feed *datosAcelerometroX = io.feed("Aceleración_X");
AdafruitIO_Feed *datosAcelerometroY = io.feed("Aceleración_Y");
AdafruitIO_Feed *datosAcelerometroZ = io.feed("Aceleración_Z");

int ultimaVez = millis(); // Para enviar cada X segundos pero sin usar delay

void setup()
{
   // CONFIGURACIÓN I2C
  Wire.begin(pinSDA, pinSCL);  // pin 17(GPI17) = SDA , pin 5(GPIO5) = SCL
  Wire.beginTransmission(MPU); // Iniciar una transm. al disp. esclavo I2C con la direcc. dada.
  Wire.write(0x6B);            // Iniciar MPU
  Wire.endTransmission(true);
  
  // COMUNICACIÓN SERIE
  Serial.begin(115200); //Función que inicia la comunicación serie a 115200 bits por segundo
  
  // Intentar conectar a Adafruit
  io.connect();
  while (io.status() < AIO_CONNECTED)
  {
    delay(500);
  }
}

void loop()
{
  io.run();

  // Si han pasado más de 5 segundos desde la última vez enviar a Adafruit
  if (millis() - ultimaVez > TIEMPO_ESPERA)
  {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX -> DATASHEET
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,6,true);
    AcX = Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 reg -> Unir valor alto con bajo 
    AcXX =AcX/A_R;
    AcY = Wire.read()<<8|Wire.read();
    AcYY =AcY/A_R;
    AcZ = Wire.read()<<8|Wire.read(); 
    AcZZ =AcZ/A_R;
    
    if (isnan(AcXX) || isnan(AcYY)|| isnan(AcZZ)) // nos aseguramos de que el dato sea correcto
    {
      ultimaVez = millis();
      return;
    }
    datosAcelerometroX->save(AcXX);
    datosAcelerometroY->save(AcYY);
    datosAcelerometroZ->save(AcZZ);
    ultimaVez = millis();
  }
  delay(10);
}
