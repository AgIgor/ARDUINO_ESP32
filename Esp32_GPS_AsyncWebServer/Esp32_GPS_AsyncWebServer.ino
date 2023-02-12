#include <ArduinoJson.h>
#include <TinyGPS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "page.h"

#define LED 2

TinyGPS gps;

AsyncWebServer server(80);

const char* ssid = "VIVOFIBRA-9501";
const char* password = "rgw7ucm3GT";

const char* PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}//end not found


String getGPS(){
  // digitalWrite(LED, !digitalRead(LED));
  digitalWrite(LED,HIGH);
  float flat, flon;
  unsigned long age;
  int ano;
  byte mes, dia, hora, minuto, segundo;
  gps.f_get_position(&flat, &flon, &age);
  gps.crack_datetime(&ano,&mes, &dia, &hora, &minuto, &segundo);
  
  DynamicJsonDocument doc(1024);
  doc["gps"]["lat"] = flat;
  doc["gps"]["long"] = flon;
  doc["sats"] = gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites();
  doc["speed"] = gps.f_speed_kmph();
  doc["alt"] = gps.f_altitude();
  doc["data"]["ano"] = ano;
  doc["data"]["mes"] = mes;
  doc["data"]["dia"] = dia;
  doc["time"]["hora"] = hora;
  doc["time"]["minuto"] = minuto;
  doc["time"]["segundo"] = segundo;
  
  String output;
  serializeJson(doc, output);
  return output;
    
}//end get GPS


void setup(){
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
  Serial2.begin(4800);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  server.on("/gps", HTTP_GET, [](AsyncWebServerRequest *request){
      AsyncWebServerResponse *response = request->beginResponse(200, "text/json", getGPS());
      response->addHeader("Access-Control-Allow-Origin", "*");
      response->addHeader("Access-Control-Allow-Methods", "GET");
      request->send(response);
      digitalWrite(LED,LOW);
    });
      
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", webpageCore);
    });


    server.onNotFound(notFound);
    server.begin();
    
}//end setup

void loop(){
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  for (unsigned long start = millis(); millis() - start < 500;){
    while (Serial2.available()){
      char c = Serial2.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      gps.encode(c);
      // if (gps.encode(c))
      //   newData = true;
    }//END WHILE
  }//END FOR

  // if (newData){
  //   getGPS();
  // }//END IF NEW DATA
  
  gps.stats(&chars, &sentences, &failed);

  if (chars == 0)Serial.println("Sem dados");
}//end loop