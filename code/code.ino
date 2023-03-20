//lcd
#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//message altercations
String message = "Plz tap center button";//starting message
int x = message.length() - 1, y = 0, t = 0;//gives constant variables
String m = "";
String actions= " ";//string for the entirety of actions
int accepted = 0;//will tell us if it will run or not
//button
int acceptBPin = 7, fBpin = 6, rBpin = 5, lBpin = 4; //button pins 
int begindist = 3, dist = begindist;//used for distance in actions
boolean firstTime = true,isHigh = false;
//encoders
int lEpin = 46;//pin
int lCPose = 0;//pose
int rEpin = 47;//pin
int rCPose = 0;//pose
boolean lEHigh = false;
boolean rEHigh = false;
boolean isHighR = false;
boolean isHighL = false;
boolean hasRun = false;
boolean finished = false;
bool lRunF = true;
bool rRunF = true;
int countsPerEncoder = 10;
int wheelDiameter = 7;
double countsPerCM = 0.09;//0.45 // converts cm to counts
//motors
int speed = 0.6 * 255;
bool lRunF = true;
bool rRunF = true;
int enA = 27, in1 = 26, in2 = 25;
int enB = 23, in3 = 24, in4 = 22;
//control
boolean isStarted = false, runOVR = false,hasRun = false;
//distance
int trigPin = 39,echoPin = 38, distance, minimumDist = 7;
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
  while (not isStarted) {//waits for start
    if (digitalRead(acceptBPin)) {//start pressed
      isHigh = true;
      doSelections(true, false);//will start selection process
      isStarted = true;//breaks out of loop
    }
  }
}
void loop() {
}
void doSelections(boolean d, boolean t) {
  while (d and !runOVR) {
    message = "Select direction:  ";  // ask for direction
    if (firstTime){
      lcd.clear();
      print(15);//function to alter the string to make it multi-line
      firstTime = false;//breaks out of running this if again
    }
    while (!digitalRead(acceptBPin)) {//while not accepted
      if (!digitalRead(fBpin) and !digitalRead(rBpin) and !digitalRead(lBpin) and !digitalRead(acceptBPin)){//checks that all buttons not pressed
        isHigh = false;
      }
      if (digitalRead(fBpin) and !isHigh) {// forward has been pressed
        actions[actions.length()-1]='F';
        message[message.length()-1]= 'F';
        lcd.clear();
        print(15);
      }
      if (digitalRead(rBpin) and !isHigh) {//right has been pressed
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
      if (digitalRead(acceptBPin) and message[message.length()-1] == ' ' and !isHigh){//will start going into actions and run the robot
        t = false;
        d = false;
        runOVR = true;
        resetLCD();
        indexIntoActions();
        return;
      }
      else if (digitalRead(acceptBPin) and !isHigh){
        actions+=' ';//adds blank space to indicate next is empty
        Serial.println(actions);
        resetLCD();
        firstTime = true;
        doSelections(false,true);//switch selection to distance
      }
    }
  }
  while (t and !runOVR) {
    dist = begindist;//automaically selects beginDist as a distance
    message = "Select distance (r = +, l = -):  "+String(dist);//print out on the lcd
    if (firstTime){
      lcd.clear();
      print(15);
      firstTime = false;
    }
    while (!digitalRead(acceptBPin)) {//not accepted
      if (!digitalRead(rBpin) and !digitalRead(lBpin)){//makes sure none was pressed
        isHigh = false;
      }
      if (digitalRead(rBpin) and !isHigh) {//pressed up/right
        if (dist < 9) {
          dist ++;//increase if less than nine
        }
        lcd.clear();
        String tl = String(dist);
        message[message.length()-1] = tl[0];
        print(15);//prints out new distance
        isHigh = true;
      }
      if (digitalRead(lBpin) and !isHigh) {//left/- pressed
        if(dist > 1){
          dist --;//decrease
        }
        lcd.clear();
        String tl = String(dist);
        message[message.length()-1] = tl[0];
        print(15);//show new message
        isHigh = true;
      }
      if (digitalRead(acceptBPin)){//has been accepted
        String tl = String(dist);
        actions[actions.length()-1] = tl[0];//add it to actions
        actions+=' ';
        Serial.println(actions);
        resetLCD();
        firstTime = true;
        doSelections(true,false);//switch selection
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
//lcd
void resetLCD() {
  x = 0;
  y = 0;
  //resets cursor
  lcd.setCursor(x, y);
  lcd.clear();
  //clears message
  message = "";
}
void print(int start) {  //start will usually be 16 but changed to make not cut off words
  if (message.length() >= 16) {
    makeMessage(0, start);//make a message only through start
    lcd.print(m);//prints first line
    x = message.length() - start;//changes line
    y = 1;//next line
    lcd.setCursor(0, 1);
    makeMessage(start, message.length());//make a latter half message
    lcd.print(m);//print it out
  } else {
    lcd.print(message);//will print the message
  }
}
void makeMessage(int s, int e) {
  m = "";
  for (int i = s; i < e; i++) {
    m += message[i];
  }
}
//distance
int getDistance(){//will return distance in cm
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
//motors/encoders

void stopMotors(){//stop all
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}
void resetEncoders(){//reset encoders to 0
  lCPose = 0;
  rCPose = 0;
}