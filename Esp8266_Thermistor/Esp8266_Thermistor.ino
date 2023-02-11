

void setup() {
  Serial.begin(115200);
}

String getTemp(){
  long Resistance;
  double Temp;
  int RawADC = analogRead(A0);
  Resistance=((10240000/RawADC) - 10000);
  Temp = log(Resistance);
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;
  return String(Temp);
}

void loop() {
//  int RawADC = analogRead(A0);
//  Resistance=((10240000/RawADC) - 10000);
//  Temp = log(Resistance);
//  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
//  Temp = Temp - 273.15;


  Serial.print(getTemp());
  Serial.println();
  delay(500);

}
