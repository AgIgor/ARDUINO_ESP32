#include <WiFi.h>
#include <WebSocketsServer.h>

const char* ssid     = "VIVOFIBRA-9501";
const char* password = "rgw7ucm3GT";

int hora;

WebSocketsServer webSocket = WebSocketsServer(81);

void setup() {
  // Inicialize a serial
  Serial.begin(115200);
  WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

  // Inicialize o servidor websocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}//end setup

void loop() {
  webSocket.loop();

  long int timeMillis = 0;
  if(millis() - timeMillis > 500){
    timeMillis = millis();
    hora = millis() / 1000;
  }
}//end loop

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // Envie uma mensagem para o cliente
        webSocket.sendTXT(num, "Conectado ao servidor!");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] Mensagem do navegador: %s\n", num, payload);

      // Envie a mensagem de volta para o cliente
      webSocket.sendTXT(num, "Millis no ESP = " + String(millis()));
      break;
    case WStype_BIN:
      Serial.printf("[%u] Binário recebido: %u bytes\n", num, length);
      //hexdump(payload, length);

      // Envie a mensagem de volta para o cliente
      webSocket.sendBIN(num, payload, length);
      break;
  }
}//webSocketEvent
