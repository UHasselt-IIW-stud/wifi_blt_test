//Asynchronous Web Server Example, lib has been modified but has not been tested on actual hardware yet!

#include <Arduino.h>
#include <WiFi.h>
//#include <ESPAsyncWebServer.h>    //PlatformIO lib manager: ESP Async WebServer by Hristo Gochkov 
#include "ESPAsyncWebServer.h"    //Local version of the library, modified to work with ESP32-C3, 
                                  //to use the library version, replace the above line with this one,
                                  //and modify platformio.ini as well.
#include <SPIFFS.h>
#include <Adafruit_NeoPixel.h>

//Use temporary credentials, you can request them at wifi.uhasselt.be (they are active for approx. 24h)
const char *SSID = "test";
const char *PASSWORD = "test1234";


//Addressable LED constructor
Adafruit_NeoPixel LED(1, 7, NEO_RGB + NEO_KHZ800);

AsyncWebServer server(80);		// Create AsyncWebServer object on port 80
String GetRssi(void);
String GetMillis(void);


void setup() {
  Serial.begin(115200); 
  Serial.println("Starting setting up WiFi network.");

  //Set up Builtin LED
  LED.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  LED.show();  // Turn OFF all pixels ASAP
  LED.setPixelColor(0, LED.Color(0, 0, 100)); //RGB: Blue --> Setup
  LED.show();   // Send the updated pixel colors to the hardware.

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
		Serial.println(F("An Error has occurred while mounting SPIFFS"));
		return;
	}

  WiFi.begin(SSID, PASSWORD); //connect to non-WPA2-Enterprise

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
  LED.setPixelColor(0, LED.Color(0, 100, 0)); //RGB: Green --> WiFi connected
  LED.show();   // Send the updated pixel colors to the hardware.

  	// Print ESP32 SSID and Local IP Address
	Serial.print(F("\nReady\n\nConnect your device to \"")); Serial.print(WiFi.SSID().c_str());
	Serial.print(F("\", point your browser to: \"")); Serial.print(WiFi.localIP()); Serial.println("\".");


  //Setup the webserver
	// Route for root / web page
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(SPIFFS, "/index.html");
	});
	server.on("/Rssi", HTTP_GET, [](AsyncWebServerRequest *request) {				//Transmits the data, formatted as string 
		request->send_P(200, "text/plain", GetRssi().c_str());				//.c_str() generates a pointer to the beginning of the string
	});
	server.on("/Timems", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send_P(200, "text/plain", GetMillis().c_str());
	});


	//Host static files required for the correct implementation of the website
	server.serveStatic("/highcharts.js", SPIFFS, "/highcharts.js");				// Serve the file "/www/page.htm" when request url is "/page.htm"
	server.serveStatic("/highcharts-more.js", SPIFFS, "/highcharts-more.js");	//server.serveStatic("/RequestURL.html", SPIFFS, "/Hostfolder/page.htm");
	server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");


	// Start server
	server.begin();
	Serial.println(F("Setup Ready.\n\n"));
}

void loop() {
  delay(5000);

}


String GetRssi(){
  return String(WiFi.RSSI());
}

String GetMillis(){
  return String(millis());
}