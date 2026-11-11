#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 8, 9, 10, 11);
int buttonPin[2] = {12, 13};
int prevButtonState[2] = {0, 0};

int currentAppState = 0;
int timeLeft = 0;
int lastTick = 0;
bool needUpdateLcd = true;

void setup() {
  lcd.begin(16, 2);
  pinMode(buttonPin[0], INPUT_PULLUP);
  pinMode(buttonPin[1], INPUT_PULLUP);

  lcd.print("Countdown clock");

  lcd.setCursor(0, 1);
  
  lcd.print("By Group 4!");
  delay(2000);
}

bool pressed(int btn) {
  int currentState = digitalRead(buttonPin[btn]);
  if (currentState != prevButtonState[btn]) {
    prevButtonState[btn] = currentState;
    if (currentState == LOW)
      return true;
  }
  return false;
}

void updateLcd() {
  switch (currentAppState) {
    case 0:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set time: ");
      lcd.print(timeLeft);
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time left: ");
      lcd.print(timeLeft);
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time paused!");
      lcd.setCursor(0, 1);
      lcd.print("Time left: ");
      lcd.print(timeLeft);
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time's up!");
  }
}

void loop() {
  bool a = pressed(0);
  bool b = pressed(1);

  // 0 is for setting, 1 is running, 2 is paused. 
  switch (currentAppState) {
    case 0:
      if (b) {
        timeLeft += 5;
        needUpdateLcd = true;
      }
      else if (a) {
        if (timeLeft <= 0)
          break;
        currentAppState = 1;
        lastTick = millis();
        needUpdateLcd = true;
      }
      break;
    case 1:
      if (a) {
        currentAppState = 2;
        needUpdateLcd = true;
      }
      else if (millis() - lastTick >= 1000) {
        lastTick += 1000;
        timeLeft--;
        if (timeLeft <= 0)
          currentAppState = 3;
        needUpdateLcd = true;
      }
      break;
    case 2:
      if (a) {
        currentAppState = 1;
        needUpdateLcd = true;
      }
      else if (b) {
        currentAppState = 0;
        timeLeft = 0;
        needUpdateLcd = true;
      }
      break;
    case 3:
      if (a || b) {
        currentAppState = 0;
        timeLeft = 0;
        needUpdateLcd = true;
      }
      break;
  }

  if (needUpdateLcd) {
    updateLcd();
    needUpdateLcd = false;
  }
}
