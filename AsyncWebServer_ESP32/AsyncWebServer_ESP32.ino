#include <Arduino.h>
#include <ArduinoJson.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "page.h"


AsyncWebServer server(80);
IPAddress local_IP(192,168,15,185);
IPAddress gateway(192,168,15,1);
IPAddress subnet(255,255,255,0);

const char* ssid = "VIVOFIBRA-9501";
const char* password = "rgw7ucm3GT";

const char* PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}//end not found

String getTemp(){
  long Resistance;
  double Temp;
  int RawADC = analogRead(A0);
  Resistance=((10240000/RawADC) - 10000);
  Temp = log(Resistance);
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;

  DynamicJsonDocument doc(1024);
  doc["sensor"] = String(Temp);
  String output;
  serializeJson(doc, output);
  return output;
  
}//end get temp

void setup() {

    Serial.begin(115200);
    if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());


    server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request){
      AsyncWebServerResponse *response = request->beginResponse(200, "text/json", getTemp());
      response->addHeader("Access-Control-Allow-Origin", "*");
      response->addHeader("Access-Control-Allow-Methods", "GET");
      request->send(response);
    });
      
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", webpageCore);
    });
    
    
//    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request){
//        request->send(200, "text/plain", "Ok");
//    });

    // Send a GET request to <IP>/get?message=<message>
//    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
//        String message;
//        if (request->hasParam(PARAM_MESSAGE)) {
//            message = request->getParam(PARAM_MESSAGE)->value();
//        } else {
//            message = "No message sent";
//        }
//        request->send(200, "text/plain", "Hello, GET: " + message);
//    });

    // Send a POST request to <IP>/post with a form field message set to <message>
//    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
//        String message;
//        if (request->hasParam(PARAM_MESSAGE, true)) {
//            message = request->getParam(PARAM_MESSAGE, true)->value();
//        } else {
//            message = "No message sent";
//        }
//        request->send(200, "text/plain", "Hello, POST: " + message);
//    });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {}
