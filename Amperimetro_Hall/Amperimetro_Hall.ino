#define hall_pin A2
#define leituras 100

float Vzero = 2.56;
float K = 0.01;
float B = 0.11;
float Vbase = 4.88;
float Vin = 0;
float I = 0;


void setup() {
  Serial.begin(115200);
  pinMode(hall_pin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Vin = analogRead(hall_pin);
}

void loop() {
  float soma = 0;
  float media = 0;

  for(int x = 0; x < leituras; x ++){
    Vin = analogRead(hall_pin);
    Vin = (Vin * Vbase) / 1024;
    I = (Vin - Vzero)/(K*B);
    if(I<0)I=0;
    I = I * 0.01;
    soma = soma + I;
  }

  media = soma/leituras;

  Serial.print("I = ");
  Serial.println(media);
  delay(100);
}
