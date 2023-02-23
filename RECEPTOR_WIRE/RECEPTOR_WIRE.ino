#include <Wire.h>

void receiveEvent(int howMany){
  while(1 < Wire.available())
  {
    char c = Wire.read();
    Serial.print(c);
  }
  int x = Wire.read();
  Serial.println(x);

  // Wire.beginTransmission(4);
  // Wire.write("recebido!");
  // Wire.endTransmission();

}

void setup(){
  Wire.begin(0x27);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.println("ok rx");
}

void loop(){
  delay(1);
}