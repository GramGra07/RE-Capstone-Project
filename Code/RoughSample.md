#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char message[]="hello worldyybyy opp";
int y = 0;
int x = sizeof(message);
String m ="";

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  print();
}

void loop() {
  lcd.cursor();
  lcd.setCursor(x, y);
}
void print(){
  if (sizeof(message)>=16){
    x=0+sizeof(message)-16;
    y=1;
    makeMessage(0,16);
    Serial.println(m);
    delay(5000);
    makeMessage(16,sizeof(message));
    Serial.println(m);
  }
  lcd.print(message);
}
void makeMessage(int start, int end){
  m ="";
  for (int i = start; i<end;i++){
    m+=message[i];
  }
}
