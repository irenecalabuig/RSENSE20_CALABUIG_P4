/******************************************************************************

ESP32-CAM remote image access via FTP. Take pictures with ESP32 and upload it via FTP making it accessible for the outisde network. 
Leonardo Bispo
July - 2019
https://github.com/ldab/ESP32_FTPClient

Distributed as-is; no warranty is given.

******************************************************************************/
#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClient.h> 
#include <ESP32_FTPClient.h>
#include "octocat.h"

#define WIFI_SSID "RSense"
#define WIFI_PASS ""

char ftp_server[] = "155.210.150.77";
char ftp_user[]   = "rsense";
char ftp_pass[]   = "rsense";

// you can pass a FTP timeout and debbug mode on the last 2 arguments
ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass, 5000, 2);

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

  ftp.OpenConnection();


  // Create the file new and write a string into it
  ftp.ChangeWorkDir("/rsense/icalabuig");
  ftp.InitFile("Type A");
  ftp.NewFile("hello_world.txt");
  ftp.Write("Hello World");
  ftp.CloseFile();

  ftp.CloseConnection();
}

void loop()
{

}
