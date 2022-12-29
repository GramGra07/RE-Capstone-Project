//lcd
#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String message = "Plz tap center button";
int y = 0;
int x = message.length() - 1;
int t = 0;
String m = "";
String actions = "";
//button
const int acceptBPin = 7, fBpin = 6, rBpin = 5, lBpin = 4;// using 7-4 as inputs
int aBS = 0, fBS = 0, bBS = 0, rBS = 0, lBS = 0; //button states
const int motorBR = 3, motorBL = 2, motorFR = 1, motorFL = 0;
//other
boolean isStarted = false;
boolean directionS = false;
boolean lengthS = false;
boolean accepted = false;
boolean runOVR = false;
boolean hasRun = false;
//Serial.println(message.length());
void setup() {
  pinMode(acceptBPin, INPUT);
  pinMode(fBpin, INPUT);
  pinMode(rBpin, INPUT);
  pinMode(lBpin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  print(15);//starting message
}
void loop() {
  setupButton();
  lcd.cursor();
  if (not isStarted) {
    if (aBS == HIGH) {
      isStarted = true;
      directionS = true;
    }
  }
  while (isStarted) {
    doSelections(directionS, lengthS);
  }
  //Serial.println(actions);
}
void resetLCD() {
  x = 0;
  y = 0;
  lcd.setCursor(x, y);
  lcd.clear();
  message = "";
}
void setupButton() {
  aBS = digitalRead(acceptBPin);
  fBS = digitalRead(fBpin);
  rBS = digitalRead(rBpin);
  lBS = digitalRead(lBpin);
}
boolean isAccepted() {
  int a = digitalRead(acceptBPin);
  while (a != HIGH) {
    //not pressed
    a = digitalRead(acceptBPin);
    if (a == HIGH) {
      return true;
      break;
    }
  }
}
void switchToL() {
  lengthS = true;
  directionS = false;
}
void switchToD() {
  directionS = true;
  lengthS = false;
}
void reBind() {
  int a = digitalRead(acceptBPin);
  int f = digitalRead(fBpin);
  int r = digitalRead(rBpin);
  int l = digitalRead(lBpin);
}
void doSelections(boolean d, boolean t) {
  hasRun = false;
  if (d) {
    resetLCD();
    message = "Select direction:  "; //last = [18]
    print(15);
    int a = digitalRead(acceptBPin);
    int f = digitalRead(fBpin);
    int r = digitalRead(rBpin);
    int l = digitalRead(lBpin);
    while (!isAccepted and not hasRun) {
      if (f == HIGH) {
        actions += "F";
        message[17] = "F";
        lcd.clear();
        print(15);
        if (isAccepted) {
          switchToL();
          hasRun = true;
          break;
        }
      }
      if (r == HIGH) {
        actions += "R";
        message += " R";
        lcd.clear();
        print(15);
        if (isAccepted) {
          switchToL();
          hasRun = true;
          break;
        }
      }
      if (l == HIGH) {
        actions += "L";
        message += " L";
        lcd.clear();
        print(15);
        if (isAccepted) {
          switchToL();
          hasRun = true;
          break;
        }
      }
    }
  }
  if (t) {
    t = 0;
    int a = digitalRead(acceptBPin);
    int f = digitalRead(fBpin);
    int r = digitalRead(rBpin);
    int l = digitalRead(lBpin);
    resetLCD();
    message = "Select time (r = +, l = -): ";
    print(15);
    while (!isAccepted) {
      if (r) {
        t += 1;
      }
      if (l) {
        if (t > 1) {
          t -= 1;
        }
      }
      message += t;
      lcd.print(message);
      if (isAccepted) {
        switchToD();
        break;
      }
    }
  }
}
void print(int start) { //start will usually be 16 but changed to make not cut off words
  if (message.length() >= 16) {
    makeMessage(0, start);
    lcd.print(m);
    x = message.length() - start;
    y = 1;
    lcd.setCursor(0, 1);
    makeMessage(start, message.length());
    lcd.print(m);
  } else {
    lcd.print(message);
  }
}
void makeMessage(int s, int e) {
  m = "";
  for (int i = s; i < e; i++) {
    m += message[i];
  }
}
void runMotors(int d, int t) { //distance and time
  //   1
  //2    3
  if (d == 1) {
    //go forward
    delay(t * 1000);
    //stop
  }
  if (d == 2) {
    //turn -90
    //go forward
    delay(t * 1000);
    //stop
  }
  if (d == 3) {
    //turn 90
    //go forward
    delay(t * 1000);
    //stop
  }
}
