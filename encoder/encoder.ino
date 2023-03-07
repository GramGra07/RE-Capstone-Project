int lEpin = 46;
int currPos =0;
boolean track = true;
boolean isHigh = false;
bool runningForward;
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
void setup() {
  pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	pinMode(lEpin, INPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
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
    analogWrite(enA, 255);
    analogWrite(enB, 255);

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

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
  stopMotors();
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
  stopMotors();
}
void stopMotors(){
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}