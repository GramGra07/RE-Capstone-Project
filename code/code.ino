//used pins (13,12,11,10,9,8,7,6,5,4)
//lcd
#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String message = "Plz tap center button";
int x = message.length() - 1, y = 0, t = 0;
String m = "";
String actions= " ";
int accepted = 0;
//button
int acceptBPin = 7, fBpin = 6, rBpin = 5, lBpin = 4;  // using 7-4 as inputs
int beginTime = 5, time = beginTime;
boolean firstTime = true,isHigh = false;
//distance
//const int dPinEcho = 3, dPinTrig = 2;
//long duration;
//int distance, limit = 4;
//runtime
//double startTime = millis(), currentTime, runtime;
//other
boolean isStarted = false, runOVR = false;
void setup() {
  pinMode(acceptBPin, INPUT);
  pinMode(fBpin, INPUT);
  pinMode(rBpin, INPUT);
  pinMode(lBpin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  print(15);  //starting messageBind();
  lcd.cursor();
  while (not isStarted) {
    if (digitalRead(acceptBPin)) {
      isHigh = true;
      doSelections(true, false);
      isStarted = true;
    }
  }
}
void loop() {
  
}
void resetLCD() {
  x = 0;
  y = 0;
  lcd.setCursor(x, y);
  lcd.clear();
  message = "";
}
void switchToT() {
  doSelections(false,true);
}
void switchToD() {
  doSelections(true,false);
}
void doSelections(boolean d, boolean t) {
  while (d and !runOVR) {
    message = "Select direction:  ";  //last = [18]
    if (firstTime){
      lcd.clear();
      print(15);
      firstTime = false;
    }
    while (!digitalRead(acceptBPin)) {
      if (!digitalRead(fBpin) and !digitalRead(rBpin) and !digitalRead(lBpin) and !digitalRead(acceptBPin)){
        isHigh = false;
      }
      if (digitalRead(fBpin) and !isHigh) {
        actions[actions.length()-1]='F';
        message[message.length()-1]= 'F';
        lcd.clear();
        print(15);
      }
      if (digitalRead(rBpin) and !isHigh) {
        actions[actions.length()-1] = 'R';
        message[message.length()-1] = 'R';
        lcd.clear();
        print(15);
      }
      if (digitalRead(lBpin) and !isHigh) {
        actions[actions.length()-1] = 'L';
        message[message.length()-1] = 'L';
        lcd.clear();
        print(15);
      }
      if (digitalRead(acceptBPin) and message[message.length()-1] == ' ' and !isHigh){
        //Serial.println("here");
        t = false;
        d = false;
        runOVR = true;
        resetLCD();
        indexIntoActions();
        return;
      }
      else if (digitalRead(acceptBPin) and !isHigh){
        actions+=' ';
        Serial.println(actions);
        resetLCD();
        firstTime = true;
        doSelections(false,true);
      }
    }
  }
  while (t and !runOVR) {
    time = beginTime;
    message = "Select time (r = +, l = -):  "+String(time);
    if (firstTime){
      lcd.clear();
      print(15);
      firstTime = false;
    }
    while (!digitalRead(acceptBPin)) {
      if (!digitalRead(rBpin) and !digitalRead(lBpin)){
        isHigh = false;
      }
      if (digitalRead(rBpin) and !isHigh) {
        if (time < 9) {
          time ++;
        }
        lcd.clear();
        String tl = String(time);
        message[message.length()-1] = tl[0];
        print(15);
        isHigh = true;
      }
      if (digitalRead(lBpin) and !isHigh) {
        if(time > 1){
          time --;
        }
        lcd.clear();
        String tl = String(time);
        message[message.length()-1] = tl[0];
        print(15);
        isHigh = true;
      }
      if (digitalRead(acceptBPin)){
        String tl = String(time);
        actions[actions.length()-1] = tl[0];
        actions+=' ';
        Serial.println(actions);
        resetLCD();
        firstTime = true;
        doSelections(true,false);
      }
    }
  }
}
void indexIntoActions() {
  message = "Running, watch out!";
  print(15);
  actions.replace(" ","");
  for (int i = 0; i < actions.length(); i += 2) {
    runMotors(String(actions[i]), actions[i + 1]);
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
void runMotors(String d, char t) {  //distance and time
  //Serial.println(t);
  t = int(t)-int('0');
  Serial.println(t);
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
void runForward(int t) {
  while (runtime * 1000 > t) {
    //setMotorPower
    if (runDistance < limit) {
      isStarted = false;
      break;
    }
  }
}