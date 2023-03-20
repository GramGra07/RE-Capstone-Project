int lEpin = 48;
int rEpin = 46;
int lCPose =0;
int i =0;
int rCPose =0;
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
double countsPerCM = 0.09;//0.45//will translate distance to counts of the encoder
int speed = 1 * 255;
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
}
void loop() {//test loop
  turnRight();
  turnLeft();
}
int getDistance(){//returns distance in cm
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  distance = int(distance);
  Serial.println(distance);//prints out distance
  return distance;
}
void turnLeft(){
  runToPosition(22,-10);//calculated distances to turn 90 degrees
}
void turnRight(){
  runToPosition(-10,27);//calculated distances to turn 90 degrees to the right
}
void runToPosition(double r,double l){
  if (not hasRun){//only first time
    r *= -countsPerCM;
    l *= -countsPerCM;
    r*=4;
    l*=4;
    if (r<1){
      r*=-1;
      rRunF = false;//is not running forward
    }
    if (l<1){
      l*=-1;
      lRunF = false;//not running forward
    }
    r = round(r);
    l = round(l);
  }
  while ((rCPose < r or lCPose<l) and not finished){
    rCPose = int(rCPose);
    lCPose = int(lCPose);
    analogWrite(enA, speed);//sets speed to 0.6
    analogWrite(enB, speed);
    if (lRunF){//l running forward
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }else{//running back
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
    if (rRunF){//r running forward
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }else{//running back
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }
    //checks to see if encoder has already been counted
    if (digitalRead(lEpin)==0 and !isHighL){
      isHighL = true;
      lCPose+=1;
    }
    //has already been counted
    if (digitalRead(lEpin)==1){
      isHighL = false;
    }
    //checks to see if r encoder has already been counted
    if (digitalRead(rEpin)==0 and !isHighR){
      isHighR = true;
      rCPose+=1;
    }
    //has already been counted
    if (digitalRead(rEpin)==1){
      isHighR = false;
    }
    // if its too far, stop
    if (lCPose >= l){
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }
    if (rCPose >= r){
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }
    if (lCPose>=l and rCPose>=r){
      finished = true;
      resetEncoders();
      break;
    }
    //stop with distance
    //if (getDistance()<=(minimumDist/10)){
    //  stopMotors();
    //  finished = true;
    //  break;
    //}
    
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
