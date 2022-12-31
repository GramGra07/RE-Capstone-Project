const int buttonPin = 7;     // the number of the pushbutton pin
const int ledPin =  0;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  Serial.println("Program started");
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    Serial.println("button pressed");
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("button released");
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
