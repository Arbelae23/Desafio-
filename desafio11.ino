// C++ code
//
#include <Adafruit_liquidCrystal.h>
void setup()
{

}

int analogPin=0;
int val=0;

void loop()
Adafruit_liquidCrystal 1cd_1(0);

void setup()
{
  lcd_1.begin(16,2);
  Serial.begin(9600);
  lcd_1.clear();
}

void loop(){
  lcd_1.setCursor(0,0);
  val=analogRead(analogPin);
  lcd_1.print(val);
  delay(500);
  Serial.Println(val);
  lcd_1.clear();
}
