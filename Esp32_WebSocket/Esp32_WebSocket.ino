#include <WiFi.h>
#include <WebSocketsServer.h>

const char* ssid     = "VIVOFIBRA-9501";
const char* password = "rgw7ucm3GT";

int hora;
uint8_t dest;
long int timeMillis = 0;


WebSocketsServer webSocket = WebSocketsServer(81);

const int analogInPin = 34; 
int sensorValue; 
int count;
int newValue;
int leituras = 100;
long int delayRead;

int clientsCon;

void readPot(){
  if(millis() - delayRead >= 1){
    delayRead = millis();
    count++;
    sensorValue += analogRead(analogInPin);
     
    if(count >= leituras){
      count = 0;
      sensorValue /= leituras;
      
      if(newValue != sensorValue){
        newValue = sensorValue;
        Serial.println(newValue);
        webSocket.sendTXT(dest, "pot&" + String(newValue) + "&");  
      } 
    } 
  }
}

void setup() {
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
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
  if(clientsCon > 0){
    readPot();  
  }
 
  
  if(millis() - timeMillis >= 500){
    timeMillis = millis();
    hora = millis() / 1000;
//    Serial.println(hora);
    //webSocket.sendTXT(dest, "=> " + String(millis()));
  }
}//end loop

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      {
        clientsCon --;
        Serial.printf("[%u] Disconnected!\n", num);
        break;
      }
    case WStype_CONNECTED:
      {
        clientsCon ++;
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // Envie uma mensagem para o cliente
        dest = num;
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
