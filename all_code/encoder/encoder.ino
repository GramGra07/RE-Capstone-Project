int lEpin = 48;
int rEpin = 46;

int lCPose =0;//left pose
int i =0;
int rCPose =0;//right pose
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
int speed = 1 * 255; // sets speed
int enA = 27;
int in1 = 26;
int in2 = 25;
// Motor B connections
int enB = 23;
int in3 = 24;
int in4 = 22;

int minimumDist = 10;
int trigPin = 39;
int echoPin = 38;
// defines variables
long duration;
int distance;
void setup() {
  pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	pinMode(rEpin, INPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  turnLeft();
}
void loop() {
  //runToPosition(8,8);
}
int getDistance(){//will return distance
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
void turnLeft(){
  runToPosition(22,-10);//calculated to turn left
}
void turnRight(){
  runToPosition(-10,27);//calculated to turn right
}
void runToPosition(double r,double l){
  if (not hasRun){
    r *= -countsPerCM;
    l *= -countsPerCM;
    r*=4;
    l*=4;
    if (r<1){
      r*=-1;
      rRunF = false;
    }
    if (l<1){
      l*=-1;
      lRunF = false;
    }
    r = round(r);
    l = round(l);
  }
  while ((rCPose < r or lCPose<l) and not finished){
    rCPose = int(rCPose);
    lCPose = int(lCPose);
    //Serial.println("r");
    //Serial.println(rCPose);
    //Serial.println("l");
    //Serial.println(lCPose);

    analogWrite(enA, speed);//set it to speed
    analogWrite(enB, speed);
    if (lRunF){//if running forward
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }else{//running backward
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
    if (rRunF){//running forward
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }else{//backwards
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }
    //is already counted
    if (digitalRead(lEpin)==0 and !isHighL){
      isHighL = true;
      lCPose+=1;
    }
    //was counted, be ready to count again
    if (digitalRead(lEpin)==1){
      isHighL = false;
    }
    //check if its counted
    if (digitalRead(rEpin)==0 and !isHighR){
      isHighR = true;
      rCPose+=1;
    }
    // was counted as a one
    if (digitalRead(rEpin)==1){
      isHighR = false;
    }
    //if it has gone to far, stop
    if (lCPose >= l){
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }
    //gone to far, stop
    if (rCPose >= r){
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }
    //both to position, stop and reset
    if (lCPose>=l and rCPose>=r){
      finished = true;
      resetEncoders();
      break;
    }
    //stop for distance
    if (getDistance()<=(minimumDist/10)){
      stopMotors();
      finished = true;
      resetEncoders();
      break;
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
  finished = false;
  hasRun = false;
  lRunF = true;
  rRunF = true;
  lCPose = 0;
  rCPose = 0;
  delay(500);
}