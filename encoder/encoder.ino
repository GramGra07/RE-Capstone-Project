int rEpin = 47;
int lEpin = 46;

int lCPose =0;

int rCPose =0;
boolean track = true;
boolean lEHigh = false;
boolean rEHigh = false;
boolean isHigh = false;
boolean hasRun = false;
bool lRunF = true;
bool rRunF = true;
float countsPerEncoder = 10;
float wheelDiameter = 2.75;
float countsPerInch = countsPerEncoder/(wheelDiameter*3.14159265358);
int speed = 0.6 * 255;
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
	pinMode(rEpin, INPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  Serial.begin(9600);
}
void loop() {
  //Serial.println(digitalRead(lEpin));
  runToPosition(-5,5);
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