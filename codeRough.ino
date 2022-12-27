//lcd
#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4,d5, d6, d7);
String message="Plz tap center button";
int y = 0;
int x = message.length()-1;
String m ="";
String actions="";
//button
const int acceptBPin = 7, fBpin = 6, bBpin = 5, rBpin = 4, lBpin = 3;// using 10-6 as inputs
int aBS = 0, fBS =0, bBS=0, rBS = 0, lBS =0;//button states
//motor, only room for 3, middle wheel needs to be omni
const int motorBR = 2, motorBL= 1, motorTurn = 0;
//other
//modes: selecting direction, selecting length, accept?
boolean isStarted=true;
boolean directionS = true;
boolean lengthS = false;
boolean accepted = false;
boolean runOVR = false;
boolean hasRun = false;
//Serial.println(message.length());
void setup() {
  pinMode(acceptBPin,INPUT);
  pinMode(fBpin,INPUT);
  pinMode(bBpin,INPUT);
  pinMode(rBpin,INPUT);
  pinMode(lBpin,INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  print(15);//starting message
}
void loop() {
  setupButton();
  lcd.cursor();
  if (aBS==HIGH){
    if (not isStarted){
      isStarted=true;
      directionS=true;
    }
    checkAccepted();
  }
  if (isStarted){
    lcd.setCursor(x, y);
    if (directionS){
      lengthS=false;
      if (not hasRun){
        resetLCD();
        message="Select direction:";
        print(15);
        handleButton();
        hasRun=true;
      }
    }
    
    if (lengthS){
      directionS=false;
      if (not hasRun){
        resetLCD();
        message="Select time:";
        print(15);
        handleButton();
        hasRun=true;
      }
    }
  }
  Serial.println(actions);
}
void resetLCD(){
  x=0;
  y=0;
  lcd.setCursor(x,y);
  lcd.clear();
  message="";
}
void setupButton(){
  aBS = digitalRead(acceptBPin);
  fBS = digitalRead(fBpin);
  bBS = digitalRead(bBpin);
  rBS = digitalRead(rBpin);
  lBS = digitalRead(lBpin);
}
void checkAccepted(){
  while (not aBS==HIGH and not accepted and Serial.available()==0){
    if (aBS == HIGH){
      accepted=true;
    }
  }
}
void handleButton(){
  //add button like inputs for testing
  while (Serial.available()==0){
  }
  if (Serial.readString() == "f"){
    fBS=HIGH;
  }if (Serial.readString() == "b"){
    bBS=HIGH;
  }if (Serial.readString() == "l"){
    lBS=HIGH;
  }if (Serial.readString() == 'r'){
    rBS=HIGH;
  }
  //that doesnt work ^
  //end button inputs
  if (fBS==HIGH){
    //fwd
    if (directionS){
      actions+="F,";
      accepted=false;
      directionS=false;
      lengthS=true;
      resetLCD();
    }
    
  }
  if (bBS==HIGH){
    //bwd
    if (directionS){
      actions+="B,";
      accepted=false;
      directionS=false;
      lengthS=true;
      resetLCD();
    }
  }
  if (rBS==HIGH){
    //right
    if (directionS){
      actions+="R,";
      accepted=false;
      directionS=false;
      lengthS=true;
      resetLCD();
    }
  }
  if (lBS==HIGH){
    //left
    if (directionS){
      actions+="L,";
      accepted=false;
      directionS=false;
      lengthS=true;
      resetLCD();
    }
  }
}
void print(int start){//start will usually be 16 but changed to make not cut off words
  if (message.length()>=16){
    makeMessage(0,start);
    lcd.print(m);
    x=message.length()-start;
    y=1;
    lcd.setCursor(0,1);
    makeMessage(start,message.length());
    lcd.print(m);
  }else{
    lcd.print(message);
  }
}
void makeMessage(int s, int e){
  m ="";
  for (int i = s; i<e;i++){
    m+=message[i];
  }
}
void runMotors(int d, int t){//distance and time
  //   1
  //2    3
  //   4
  if (d==1){
    //go forward
    delay(t*1000);
    //stop
  }
  if (d==2){
    //turn -90
    //go forward
    delay(t*1000);
    //stop
  }
  if (d==3){
    //turn 90
    //go forward
    delay(t*1000);
    //stop
  }
  if (d==4){
    //go backward
    delay(t*1000);
    //stop
  }
}
