#include <PWM.h>
int32_t frequency = 10000; //frequency (in Hz)

#define hall_pin A2
#define leituras 100
#define pinPWM 9

float Vzero = 2.56;
float K = 0.01;
float B = 0.11;
float Vbase = 4.88;
float Vin = 0;
float I = 0;


#define kp 1.5 //0.5
#define ki 1.2 //0.2
#define kd 1.1 //0.1

double setpoint = 0.5;   // Valor de corrente desejado (em Ampères)

double output = 0.0;     // Valor de saída do controlador (em PWM)
double error = 0.0;      // Erro entre o setpoint e o input
double last_error = 0.0; // Último erro calculado
double integral = 0.0;   // Acumulador da integral do erro
double derivative = 0.0; // Derivada do erro

double output_min = 0.0;
double output_max = 255.0;

long delayMillis;
#define DELAYPRINT 100

void setup() {
  InitTimersSafe(); 
  bool success = SetPinFrequencySafe(pinPWM, frequency);
  if(!success){
    while(1){
      
    }    
  }
  Serial.begin(115200);
  pinMode(hall_pin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pinPWM, OUTPUT);

  Vin = analogRead(hall_pin);
}

void loop() {
  float soma = 0;
  float media = 0;

  for(int x = 0; x < leituras; x ++){
    Vin = analogRead(hall_pin);
    Vin = (Vin * Vbase) / 1023;
    I = (Vin - Vzero)/(K*B);
    if(I<0)I=0;
    I = I * 0.01;
    soma = soma + I;
  }

  media = soma/leituras;
  
  error = setpoint - media;
  integral = integral + error;
  
  if (integral > output_max / ki) {// Limitação da integral do erro
    integral = output_max / ki;
  }
  else if (integral < output_min / ki) {
    integral = output_min / ki;
  }
  
  derivative = error - last_error;// Cálculo da derivada do erro
  output = kp * error + ki * integral + kd * derivative;// Cálculo da saída do controlador
  
  if (output > output_max) {// Limitação da saída do controlador
    output = output_max;
  }
  else if (output < output_min) {
    output = output_min;
  }
  
  pwmWrite(pinPWM, output);
  last_error = error;

  if(millis() - delayMillis >= DELAYPRINT){
    delayMillis = millis();
    Serial.print("I = ");
    Serial.print(media);

    // Serial.print(" error = ");
    // Serial.print(error);

    Serial.print(" out = ");
    Serial.println(output);
  }
  delay(1);
}
