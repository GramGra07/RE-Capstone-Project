int lEpin = 20;
void setup() {
  pinMode(lEpin, INPUT);
  Serial.begin(9600);
}
void loop() {
  Serial.println(digitalRead(lEpin));
}
