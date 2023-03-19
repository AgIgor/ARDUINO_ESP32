#include <PWM.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

int32_t frequency = 10000; //frequency (in Hz)

#define hall_pin A2
#define leituras 100
#define pinPWM 9

#define up_pin 6
#define down_pin 7
bool up_bot = true;
bool down_bot = true;

double newSetpoint;
bool err = false;



float Vzero = 2.56;
float K = 0.01;
float B = 0.11;
float Vbase = 4.88;
float Vin = 0;
float I = 0;


#define kp 5.5 //0.5
#define ki 5.2 //0.2
#define kd 5.1 //0.1

double setpoint = 0.1;   // Valor de corrente desejado (em Ampères)
double maxSet = 2.5;

double output = 0.0;     // Valor de saída do controlador (em PWM)
double error = 0.0;      // Erro entre o setpoint e o input
double last_error = 0.0; // Último erro calculado
double integral = 0.0;   // Acumulador da integral do erro
double derivative = 0.0; // Derivada do erro

double output_min = 0.0;
double output_max = 255.0;

long delayMillis;
#define DELAYPRINT 200

void setup() {
  InitTimersSafe();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, world!");

  bool success = SetPinFrequencySafe(pinPWM, frequency);
  if(!success){
    while(1){
      
    }    
  }
  Serial.begin(115200);
  pinMode(hall_pin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pinPWM, OUTPUT);

  pinMode(up_pin, INPUT_PULLUP);
  pinMode(down_pin, INPUT_PULLUP);

  // Vin = analogRead(hall_pin);
  lcd.clear();  
}

void loop() {
  float soma = 0;
  float media = 0;

  // setpoint = map(analogRead(pot_pin), 0, 1023, 0, 250) * 0.01;
  
  if(!digitalRead(up_pin) && up_bot){
    setpoint = setpoint + 0.05;
    if(setpoint > maxSet){
      setpoint = maxSet;
    }
    up_bot = false;
  }
  if(digitalRead(up_pin) && !up_bot){
    up_bot = true;
  }
  if(!digitalRead(down_pin) && down_bot){
    if(err){
      setpoint = 0;
      err = false;    
    }else{
      setpoint = setpoint - 0.05;
      if(setpoint < 0){
        setpoint = 0;
      }
      down_bot = false;
    }
    
  }
  if(digitalRead(down_pin) && !down_bot){
    down_bot = true; 
  }

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

  // if(media < 0.03 && output > 10){
  //   newSetpoint = setpoint;
  //   setpoint = 0;
  //   err = true;
  // }  

  if(millis() - delayMillis >= DELAYPRINT){
    delayMillis = millis(); 
    // lcd.clear();
    // Serial.print("I = ");
    // Serial.print(media);

    // Serial.print(" error = ");
    // Serial.print(error);

    // Serial.print(" out = ");
    // Serial.println(output);

    lcd.setCursor(0, 0);
    lcd.print("I ");lcd.print(media);lcd.print(" ");

    //lcd.setCursor(0, 0);
    lcd.print("S ");lcd.print(setpoint);lcd.print(" ");

    lcd.setCursor(0, 1);
    lcd.print("PWM ");lcd.print(output,0);lcd.print(" ");

    lcd.print("E ");lcd.print(error);lcd.print(" ");


  }
  delay(1);
}
