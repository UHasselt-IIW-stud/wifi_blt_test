#include <Arduino.h>
#include "WiFi.h"

const char* SSID = "test";
const char* PASSWORD = "Test1234";
/*
void setup() { 
  Serial.begin(115200);
  //WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  Serial.print(F("Connecting to WiFi .."));
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(F('.'));
    delay(500);
  }
  Serial.printf("Connected to \"%S\", with ip: ",SSID);
  Serial.println(WiFi.localIP());
}
*/

 
void setup() {
 WiFi.disconnect();

  Serial.begin(115200);
  Serial.printf("Connecting to: \"%S\" with code: \"%S\".\n",SSID,PASSWORD);
 
  WiFi.begin(SSID, PASSWORD);
   
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("\nConnected to the WiFi network");
 
}

void loop() {

}