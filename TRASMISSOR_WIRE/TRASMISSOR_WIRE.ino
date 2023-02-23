#include <ArduinoJson.h>
#include <Wire.h>


int val = 0;

void envia(char* chave, int valor){

  Wire.beginTransmission(4);
  Wire.write(chave);
  Wire.write(valor);
  Wire.endTransmission();
}//end envia

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("ok tx");
}

void loop() {
  val++;
  if (val == 100) {
    val = 0;
  }
  delay(1000);
  envia("conta ", val);
  envia("tempo ", millis()/1000);
}







// #include <Wire.h>

// void setup() {
//   Wire.begin();
//   pinMode(13,OUTPUT);
//   Serial.begin(9600);
//   Serial.println("ok");
// }

// byte x = 0;

// void loop() {
//   Wire.beginTransmission(8);
//   Wire.write("x is ");
//   Wire.write(x);
//   Wire.endTransmission();
//   digitalWrite(13,HIGH);

//   x++;
//   delay(10);
//   digitalWrite(13,LOW);
//   delay(10);
// }