#include <Arduino.h>
#include <SPI.h>
#include "WiFi.h"
#include <esp_wpa2.h> //wpa2 library for connections to Enterprise networks
#include <Adafruit_NeoPixel.h>
#include <TFT_eSPI.h>

const char *ssid = "UHasselt";
const char *password = "valve-pack-feels";
const char *identity = "visitor-3958";

uint8_t ConnectWiFi(void);

void setup() {
  Serial.begin(115200);
  ConnectWiFi();
}

void loop() {
   // put your main code here, to run repeatedly:
}


uint8_t ConnectWiFi(void){
  WiFi.disconnect(true);  //disconnect from wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode as station (normal WiFi client)
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)identity, strlen(identity)); //provide identity
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)identity, strlen(identity)); //provide username --> identity and username is same
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)password, strlen(password)); //provide password

  esp_wifi_sta_wpa2_ent_enable();
  
  WiFi.begin(ssid); //connect to wifi
  uint8_t counter = 0;

  Serial.print("\n\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if (counter >= 60) { //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }
  Serial.printf("\nWiFi connected to \"%s\" (WPA2-Enterprise), with IP: ", ssid);
  Serial.println(WiFi.localIP()); //print LAN IP

  return 1;
}