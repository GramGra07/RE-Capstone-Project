#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String message = "Select distance (r=+,l=-):  ";//starting message
int x = message.length() - 1, y = 0, t = 0;//gives constant variables
String m = "";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.noCursor();
  print(16);
}

void loop() {
  // put your main code here, to run repeatedly:

}
void resetLCD() {
  x = 0;
  y = 0;
  //resets cursor
  lcd.setCursor(x, y);
  lcd.clear();
  //clears message
  message = "";
}
void print(int start) {  //start will usually be 16 but changed to make not cut off words
  if (message.length() >= 16) {
    makeMessage(0, start);//make a message only through start
    lcd.print(m);//prints first line
    x = message.length() - start;//changes line
    y = 1;//next line
    lcd.setCursor(0, 1);
    makeMessage(start, message.length());//make a latter half message
    lcd.print(m);//print it out
  } else {
    lcd.print(message);//will print the message
  }
}
void makeMessage(int s, int e) {
  m = "";
  for (int i = s; i < e; i++) {
    m += message[i];
  }
}