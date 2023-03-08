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
int acceptBPin = 7, fBpin = 6, rBpin = 5, lBpin = 4; 
int begindist = 5, dist = begindist;
boolean firstTime = true,isHigh = false;

int lEpin = 46;
int lCPose =0;
int rEpin = 47;
int rCPose =0;
boolean lEHigh = false;
boolean rEHigh = false;
int speed = 0.6 * 255;
bool lRunF = true;
bool rRunF = true;
float countsPerEncoder = 10;
float wheelDiameter = 2.75;
float countsPerInch = countsPerEncoder/(wheelDiameter*3.14159265358);

int enA = 27;
int in1 = 26;
int in2 = 25;
// Motor B connections
int enB = 23;
int in3 = 24;
int in4 = 22;

boolean hasRun = false;
//distance
//const int dPinEcho = 3, dPinTrig = 2;
//long duration;
//int distance, limit = 4;
//runtime
//double startTime = millis(), currentTime, runtime;
//other
boolean isStarted = false, runOVR = false;
void setup() {
  pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	pinMode(lEpin, INPUT);
	pinMode(rEpin, INPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);

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
    dist = begindist;
    message = "Select distance (r = +, l = -):  "+String(dist);
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
        if (dist < 9) {
          dist ++;
        }
        lcd.clear();
        String tl = String(dist);
        message[message.length()-1] = tl[0];
        print(15);
        isHigh = true;
      }
      if (digitalRead(lBpin) and !isHigh) {
        if(dist > 1){
          dist --;
        }
        lcd.clear();
        String tl = String(dist);
        message[message.length()-1] = tl[0];
        print(15);
        isHigh = true;
      }
      if (digitalRead(acceptBPin)){
        String tl = String(dist);
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
  delay(2000);
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
void runMotors(String d, char t) {  //distance and dist
  //Serial.println(t);
  t = int(t)-int('0');
  Serial.println(t);
  if (d == "F") {
    //go forward
    runToPosition(t,t);
    //stop
  }
  if (d == "R") {
    //turn -90
    //go forward
    runToPosition(t,t);
    //stop
  }
  if (d == "L") {
    //turn 90
    //go forward
    runToPosition(t,t);
    //stop
  }
}
void runToPosition(int r,int l){
  if (not hasRun){
    r *= countsPerInch;
    l *= countsPerInch;
    if (r<1){
      r*=-1;
      rRunF = false;
    }
    if (l<1){
      l*=-1;
      lRunF = false;
    }
  }
  while (rCPose < r){
    //analogWrite(enA, speed);
    analogWrite(enB, speed);
    if (rRunF){
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }else{
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }

    Serial.println(rCPose+"right");
    //run forward
    if (digitalRead(rEpin)==0 and !isHigh){
      isHigh = true;
      rCPose+=1;
    }
    else if (digitalRead(rEpin)==1 and isHigh){
      isHigh = false;
    }
    if (rCPose >= r){
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }
  }
  while (lCPose < l){
    //analogWrite(enA, speed);
    analogWrite(enA, speed);
    if (lRunF){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }else{
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }

    Serial.println(lCPose+"left");
    //run forward
    if (digitalRead(lEpin)==0 and !isHigh){
      isHigh = true;
      lCPose+=1;
    }
    else if (digitalRead(lEpin)==1 and isHigh){
      isHigh = false;
    }
    if (lCPose >= l){
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }
  }
}
void stopMotors(){
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}
void resetEncoders(){
  lCPose = 0;
  rCPose = 0;
}