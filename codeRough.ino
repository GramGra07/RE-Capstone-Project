//lcd
#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4,d5, d6, d7);
String message="Plz tap center button";
int y = 0;
int x = message.length()-1;
int t = 0;
String m ="";
String actions="";
//button
const int acceptBPin = 7, fBpin = 6, bBpin = 5, rBpin = 4, lBpin = 3;// using 10-6 as inputs
int aBS = 0, fBS =0, bBS=0, rBS = 0, lBS =0;//button states
//motor, only room for 3, middle wheel needs to be omni
const int motorBR = 2, motorBL= 1, motorTurn = 0;
//other
boolean isStarted=false;
boolean directionS = false;
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
  if (not isStarted){
    if (aBS==HIGH){
      isStarted=true;
      directionS=true;
    }
  }
  while (isStarted){
    Serial.println(aBS);
    if (directionS){
      lengthS=false;
      if (not hasRun){
        resetLCD();
        message="Select direction:";
        print(15);
        handleButton();
      }
    }
    if (lengthS){
      directionS=false;
      if (not hasRun){
        resetLCD();
        message="Select time: ";
        print(15);
        handleButton();
      }
    }
    delay(1000);
  }
  //Serial.println(actions);
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
  if (not accepted){
    while (not aBS==true){
      setupButton();
      if (aBS==true){
        if(lengthS){
          directionS=true;
          hasRun=false;
          accepted=false;
          lengthS=false;
          //Serial.println("h");
        }
        if (directionS){
          lengthS=true;
          hasRun=false;
          directionS=false;
          accepted=false;
        }
        break;
      }
    }
  }
}
void handleButton(){
  fBS=HIGH;
  if (fBS==HIGH){
    //fwd
    if (directionS){
      actions+="F,";
      message+="F";
      lcd.clear();
      print(16);
      checkAccepted();
    }
    if (lengthS){
      t+=1;
      message+=t;
      lcd.clear();
      lcd.print(message);
      checkAccepted();
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
