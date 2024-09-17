// C++ code
//
#include <Adafruit_liquidCrystal.h>


int analogPin=0;
int val=0;
int button1Pin=2;
int button2Pin=4;
bool startData=false;

void setup()
{
  pinMode(2,INPUT);
  pinMode(4,INPUT);
  Serial.begin(9600);
  lcd_1.begin(16,2);
}

void loop()
{
  lcd_1.setCursor(2,1);
  if (digitalRead(button1Pin)==HIGH){
    startData=true;
  }
  if (digitalRead(button2Pin)==HIGH){
    startData=false;
  }
  if (startData){
    val=analogRead(analogPin);
  }
  else{
    val=0;
  }
  Serial.Println(val);
  lcd_1.clear();
}
