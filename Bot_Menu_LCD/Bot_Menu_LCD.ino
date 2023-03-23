#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define PIN_BOT_A 6
#define PIN_BOT_B 7

bool cursorBlink;
int valBotA, valBotB;
bool newBotA, newBotB, triggerBot;

unsigned long releasedTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(PIN_BOT_A, INPUT_PULLUP);
  pinMode(PIN_BOT_B, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  // lcd.setCursor(0, 0);
  // lcd.print("Hello, world!");

}  // setup()

void loop() {

  bool botA = digitalRead(PIN_BOT_A);
  bool botB = digitalRead(PIN_BOT_B);

  if (!botA && !botB) {
    int count = (millis() - releasedTime) / 1000;

    if (!triggerBot && count >= 1) {
      triggerBot = true;
      Serial.println(valBotA);
      lcd.setCursor(10, 1);
      lcd.print(valBotA);
      lcd.print(" ");
    }  //if trigger bot

  } else {
    releasedTime = millis();
    triggerBot = false;

    if (botA and !newBotA) {
      newBotA = true;
      valBotA++;
    } else if (!botA) {
      newBotA = false;
    }

    if (botB and !newBotB) {
      newBotB = true;
      valBotA--;
    } else if (!botB) {
      newBotB = false;
    }
  }  // end if bot A && bot B

  static long delayPrint;
  if (millis() - delayPrint > 200) {
    delayPrint = millis();

    // Serial.print(valBotA);
    // Serial.println();

    lcd.setCursor(0, 1);
    lcd.print(valBotA);
    lcd.print(" ");
  }  //end delayPrint

  /*   
  if(triggerBot){
    lcd.setCursor(pos, 1);
    lcd.cursor();
  }else{
    lcd.noCursor();
    static long delayBlink;
    if (millis() - delayBlink > 150) {
      delayBlink = millis();
      
      lcd.setCursor(fixPos, 1);
      lcd.print(pos);
      lcd.print(" ");

      cursorBlink = !cursorBlink;
      if (cursorBlink) {
        lcd.cursor();
      } else {
        lcd.noCursor();
      }
    }  //end delayBlink
  }//end trigger cursor */

}  // loop