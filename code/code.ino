//lcd
#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String message = "Plz tap center button";
int x = message.length() - 1, y = 0, t = 0;
String m = "", actions = " ";
//button
int acceptBPin = 7, fBpin = 6, rBpin = 5, lBpin = 4;  // using 7-4 as inputs
int aBS = 0, fBS = 0, bBS = 0, rBS = 0, lBS = 0;            //button states
//distance
const int dPinEcho = 3, dPinTrig = 2;
long duration;
int distance, limit = 4;
//runtime
double startTime = millis(), currentTime, runtime;
//other
boolean isStarted = false, directionS = false, lengthS = false, runOVR = false, hasRun = false;
//Serial.println(message.length());
void setup() {
  pinMode(dPinTrig, OUTPUT);
  pinMode(dPinEcho, INPUT);
  pinMode(acceptBPin, INPUT);
  pinMode(fBpin, INPUT);
  pinMode(rBpin, INPUT);
  pinMode(lBpin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  print(15);  //starting messagereBind();
  lcd.cursor();
  while (not isStarted) {
    if (isAccepted()) {
      start();
    }
  }
  if (isStarted and not runOVR) {
    doSelections(directionS, lengthS);
  }
  while (isStarted and runOVR) {
    indexIntoActions();
    if (!isStarted) {
      break;
    }
  }
}
void loop() {
  //Serial.println(actions);
}
void resetLCD() {
  x = 0;
  y = 0;
  lcd.setCursor(x, y);
  lcd.clear();
  message = "";
}
void reBind() {
  aBS = digitalRead(acceptBPin);
  fBS = digitalRead(fBpin);
  rBS = digitalRead(rBpin);
  lBS = digitalRead(lBpin);
}
boolean isAccepted() {
  aBS = digitalRead(acceptBPin);
  while (aBS != HIGH) {
    //not pressed
    aBS = digitalRead(acceptBPin);
    if (aBS == HIGH) {
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
boolean check() {
  fBS = digitalRead(6);
  rBS = digitalRead(5);
  lBS = digitalRead(4);
  while (fBS != HIGH && rBS != HIGH && lBS != HIGH) {
    //not pressed
    fBS = digitalRead(6);
    rBS = digitalRead(5);
    lBS = digitalRead(4);
    if (fBS == HIGH || rBS == HIGH || lBS == HIGH) {
      return true;
      break;
    }
  }
}
void doSelections(boolean d, boolean t) {
  hasRun = false;
  resetLCD();
  if (d) {
    message = "Select direction:  ";  //last = [18]
    print(15);
    reBind();
    while (!isAccepted and not hasRun) {
      while (check() != true){
        Serial.println("j");
      }
      if (fBS == HIGH) {
        actions[-1] = "F ";
        message[-1] = "F";
        lcd.clear();
        print(15);
        Serial.println("hi");
        if (isAccepted) {
          switchToL();
          hasRun = true;
          break;
        }
      }
      if (rBS == HIGH) {
        actions[-1] = "R ";
        message[-1] = "R";
        lcd.clear();
        print(15);
        if (isAccepted) {
          switchToL();
          hasRun = true;
          break;
        }
      }
      if (lBS == HIGH) {
        actions[-1] = "L ";
        message[-1] = "L";
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
    reBind();
    resetLCD();
    message = "Select time (r = +, l = -):  ";
    print(15);
    while (!isAccepted) {
      Serial.println(t);
      if (rBS) {
        if (t < 9) {
          t += 1;
        }
      }
      if (lBS) {
        if (t >= 1) {
          t -= 1;
        }
      }
      message[-1] = t;
      actions[-1] = t;
      lcd.print(message);
      if (isAccepted) {
        switchToD();
        break;
      }
    }
  }
}
void indexIntoActions() {
  for (int i = 0; i < actions.length(); i += 2) {
    if (not isDigit(actions[i]) and isDigit(actions[i + 1])) {
      runMotors(String(actions[i]), actions[i + 1]);
    }
  }
}
void print(int start) {  //start will usually be 16 but changed to make not cut off words
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
int runDistance() {
  digitalWrite(dPinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(dPinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(dPinTrig, LOW);
  duration = pulseIn(dPinEcho, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}
void runMotors(String d, int t) {  //distance and time
  if (d == "F") {
    //go forward
    runForward(t);
    //stop
  }
  if (d == "R") {
    //turn -90
    //go forward
    runForward(t);
    //stop
  }
  if (d == "L") {
    //turn 90
    //go forward
    runForward(t);
    //stop
  }
}
void getRuntime() {
  currentTime = 0;
  currentTime = millis();
  runtime = currentTime - startTime;
}
void start() {
  isStarted = true;
  directionS = true;
}
void runForward(int t) {
  while (runtime * 1000 > t) {
    //setMotorPower
    if (runDistance < limit) {
      isStarted = false;
      break;
    }
  }
}