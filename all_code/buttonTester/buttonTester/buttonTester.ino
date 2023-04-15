void setup() {
  pinMode(50, INPUT);
  pinMode(51, INPUT);
  pinMode(52, INPUT);
  pinMode(53, INPUT);
  Serial.begin(9600);
}

void loop() {
  if (!digitalRead(50)){
    Serial.println("accept");
  }
  if (!digitalRead(51)){
    Serial.println("left");
  }
  if (!digitalRead(52)){
    Serial.println("right");
  }
  if (!digitalRead(53)){
    Serial.println("forward");
  }
}
