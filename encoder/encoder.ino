int lEpin = 46;
int currPos =0;
boolean track = true;
boolean isHigh = false;
bool runningForward;
void setup() {
  pinMode(lEpin, INPUT);
  Serial.begin(9600);
}
void loop() {
  //Serial.println(digitalRead(lEpin));
  Serial.println(currPos);
  runToPosition(15,15);
}
void trackTicks(){
  if (digitalRead(lEpin)==0 and !isHigh){
    isHigh = true;
    currPos+=1;
  }
  else if (digitalRead(lEpin)==1 and isHigh){
    isHigh = false;
  }
}
void runToPosition(int r,int l){
  if (currPos > r){
    runningForward = false;
  }else{
    runningForward = true;
  }
  while (currPos < r and runningForward){
    Serial.println(currPos);
    //run forward
    if (digitalRead(lEpin)==0 and !isHigh){
      isHigh = true;
      currPos+=1;
    }
    else if (digitalRead(lEpin)==1 and isHigh){
      isHigh = false;
    }
  }
  while (currPos > r and !runningForward){
    Serial.println(currPos);
    //run backward
    if (digitalRead(lEpin)==0 and !isHigh){
      isHigh = true;
      currPos-=1;
    }
    else if (digitalRead(lEpin)==1 and isHigh){
      isHigh = false;
    }
  }
}