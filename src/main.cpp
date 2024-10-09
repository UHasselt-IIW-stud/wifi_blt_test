
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>    //PlatformIO lib manager: ESP Async WebServer by Hristo Gochkov 
#include <SPIFFS.h>

//Use temporary credentials, you can use your phone or computer to share a network.
const char *SSID = "test";
const char *PASSWORD = "test1234";


AsyncWebServer server(80);		// Create AsyncWebServer object on port 80
String GetRssi(void);
String GetMillis(void);
unsigned long LastSeenMs = 0;
uint16_t TimeoutMs = 1000; //milliseconds

void setup() {
  Serial.begin(115200); 
  Serial.println("Starting setting up WiFi network.");

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
		Serial.println(F("An Error has occurred while mounting SPIFFS"));
		return;
	}

	Serial.println(F("Connecting to Wifi."));
	WiFi.mode(WIFI_STA); //Station mode
	WiFi.begin(SSID, PASSWORD); //connect to non-WPA2-Enterprise

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

  	// Print ESP32 SSID and Local IP Address
	Serial.print(F("\nDone\n\nConnect your device to \"")); Serial.print(WiFi.SSID().c_str());
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

void loop(){
  //Check if the client is still connected
  if (!WiFi.isConnected()){
    while(1);
  }
}

String GetRssi(){
  return String(WiFi.RSSI());
}

String GetMillis(){
  return String(millis());
}