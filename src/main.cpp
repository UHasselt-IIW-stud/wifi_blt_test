#include <Arduino.h>
#include "WiFi.h"
#include <esp_wpa2.h> //wpa2 library for connections to Enterprise networks
#include <Adafruit_NeoPixel.h>

//Use temporary credentials, you can request them at wifi.uhasselt.be (they are active for approx. 24h)
const char *SSID = "UHasselt";
const char *PASSWORD = "chops-preys-part";
const char *IDENTITY = "visitor-0629";

//Addressable LED constructor
Adafruit_NeoPixel LED(1, 7, NEO_RGB + NEO_KHZ800);

bool ConnectWPA2_Enterprise(void);

void setup() {
  LED.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  LED.show();  // Turn OFF all pixels ASAP

  LED.setPixelColor(0, LED.Color(0, 0, 100));
  LED.show();   // Send the updated pixel colors to the hardware.

  //Serial.begin(115200); 
  Serial.begin(115200); 
  Serial.println("Starting setting up WiFi network.");
  ConnectWPA2_Enterprise();
}

void loop() {
  delay(5000);
  if(WiFi.isConnected()){
    Serial.printf("Wifi is still connected (%ddbm).\n",WiFi.RSSI());
    LED.setPixelColor(0, LED.Color(0, 100, 0));
    LED.show();   // Send the updated pixel colors to the hardware.
  }
  else{
    Serial.println("\n\tWiFi was disconnected, reconnecting.\n");
    LED.setPixelColor(0, LED.Color(100, 0, 0));
    LED.show();   // Send the updated pixel colors to the hardware.
    ConnectWPA2_Enterprise();
  }
}

    

//Connects to WPA2-Enterprise network
bool ConnectWPA2_Enterprise(void){
  WiFi.disconnect(true);  //disconnect from wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode as station (normal WiFi client)

  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)IDENTITY, strlen(IDENTITY)); //provide identity
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)IDENTITY, strlen(IDENTITY)); //provide username --> identity and username is same
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)PASSWORD, strlen(PASSWORD)); //provide password

  esp_wifi_sta_wpa2_ent_enable();

  WiFi.begin(SSID); //connect to wifi
  uint8_t counter = 0;

  Serial.print("\n\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if (counter >= 60) { //after 30 seconds timeout...
      return 0;
    }
  }
  Serial.printf("\nWiFi connected to \"%s\" (WPA2-Enterprise), with IP: ", SSID);
  Serial.println(WiFi.localIP()); //print LAN IP

  return 1;
}
