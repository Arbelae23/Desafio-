#include <Adafruit_LiquidCrystal.h>
using namespace std;
Adafruit_LiquidCrystal lcd_1(0);

float val = 0;
int analogPin = 0;
int buttonStart = 4;
int buttonInfo = 2;
bool startData = false;
bool startInfo = false;
float tolerancia = 0.01;

int arregloSize = 200;
float *signal = new float[arregloSize];

int i = 0;

void setup()
{
  pinMode(2, INPUT);
  pinMode(4, INPUT);
  Serial.begin(9600);
  lcd_1.begin(16, 2);
}

void loop()
{ 
  if(digitalRead(buttonStart) == HIGH) {
    i = 0;
    startData = true;
  	lcd_1.setCursor(0, 0);
  	lcd_1.print("Almacenando");
  	lcd_1.setCursor(0, 1);
  	lcd_1.print("Datos...");
  }
  
  if (startData){
    almacenarDatos();
  }
  if(digitalRead(buttonInfo) == HIGH) {
    i = 0;
    startInfo = true;
  	lcd_1.setCursor(0, 0);
  	lcd_1.print("Mostrandolos");
  	lcd_1.setCursor(0, 1);
  	lcd_1.print("Datos...");
  }
  if (startInfo){
    startData = false;
    datos();
    startInfo= false;
    startData = true;
  }
  
}

void almacenarDatos() {
  val = analogRead(analogPin);
  val = val*(5.0/1023.0);
  signal[i] = val;
  delay(10);
  i++;
  if (i >= arregloSize){
    i = 0;
  }
}

void mostrarDatos(float vol, float frec){
  lcd_1.clear();
  lcd_1.setCursor(0, 1);
  lcd_1.print("V=");
  lcd_1.print(vol,2);
  lcd_1.setCursor(7, 1);
  lcd_1.print("f=");
  lcd_1.print(frec,9);
  delay(3000);
}
void ()
