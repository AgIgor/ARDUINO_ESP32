#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>

#include "readFile.h"


AsyncWebServer server(443);

void setup() {
  Serial.begin(115200);
  if(!SPIFFS.begin(true)){
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }
  File cert = SPIFFS.open("/cert.pem", "r");
  File key = SPIFFS.open("/key.pem", "r");
  // Inicialize o WiFi
  WiFi.begin("ssid", "password");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Configurar o servidor HTTPS
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello, world");
  });

  // Carregar o certificado e a chave
  File cert = SPIFFS.open("/cert.pem", "r");
  File key = SPIFFS.open("/key.pem", "r");

  // Configurar o servidor HTTPS
  // server.beginSecure(cert, key);
}
