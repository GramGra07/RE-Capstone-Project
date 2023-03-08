//lcd
#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//message altercations
String message = "Plz tap center button";
int x = message.length() - 1, y = 0, t = 0;
String m = "";
String actions= " ";
int accepted = 0;
//button
int acceptBPin = 7, fBpin = 6, rBpin = 5, lBpin = 4; 
int begindist = 5, dist = begindist;
boolean firstTime = true,isHigh = false;
//encoders
int lEpin = 46;
int lCPose =0;
int rEpin = 47;
int rCPose =0;
boolean lEHigh = false;
boolean rEHigh = false;
float countsPerEncoder = 10;
float wheelDiameter = 2.75;
float countsPerInch = countsPerEncoder/(wheelDiameter*3.14159265358);
//motors
int speed = 0.6 * 255;
bool lRunF = true;
bool rRunF = true;
int enA = 27, in1 = 26, in2 = 25;
int enB = 23, in3 = 24, in4 = 22;
//control
boolean isStarted = false, runOVR = false,hasRun = false;
//distance
int trigPin = 39,echoPin = 38, distance, minimumDist = 5;
long duration;
//run robot
int d = 0;
String dir = "";
void setup() {
  //motors
  pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
  //encoder
	pinMode(lEpin, INPUT);
	pinMode(rEpin, INPUT);
  //distance
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
	// Turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  //button inputs
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
    d = int(actions[i+1]);
    dir = String(actions[i]);
    if (dir == "R"){
      //turn right
    }else if (dir == "L"){
      //turn left
    }
    runToPosition(d,d);
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
int getDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  distance = int(distance);
  Serial.println(distance);
  return distance;
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
    if (getDistance()<minimumDist){
      stopMotors();
      break;
    }
    //analogWrite(enA, speed);
    analogWrite(enB, speed);
    if (rRunF){
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }else{
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }
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
    if (getDistance()<minimumDist){
      stopMotors();
      break;
    }
    //analogWrite(enA, speed);
    analogWrite(enA, speed);
    if (lRunF){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }else{
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
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