
#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd_1(0);

int analogPin=0;
float val=0;//valor para el voltaje que quiero entregar
int buttonStart = 4;//el boton que empieza a resivir info esta en el pin 4
int buttonInfo = 2;//boton que finaliza el resivir info ubicado en el pin 2
bool startData=false; //booleano que nos ayuda a saber cuan empezamos a resivir info
bool startInfo = false;
float tolerancia = 0.03;// usamos un valor de tolerancia ya que los valores no siempre dan cerrados

int arregloSize = 230; //tamaño del arreglo 
float *signal = new float [arregloSize]; //arreglo dinamico que almacena los datos

int i = 0;//contador que usamos en almacenarDatos


void setup()
{
  pinMode(2,INPUT);//el pin 2 es de entrada 
  pinMode(4,INPUT);//pin 4 de entrada 
  Serial.begin(9600);//inicializacion del monitor serial
  lcd_1.begin(16,2);
}

void loop()
{
  /* revisa el estado de los botones
  dependiendo del estado llama la funcion que almacena los datos*/
  
  if (digitalRead(buttonStart)==HIGH){ //boton que resive
    i=0;
    startData=true;
    lcd_1.clear();//limpiar el lcd
    lcd_1.setCursor(0, 0);//inicializar mostrar en pantalla c f
    lcd_1.print("Almacenando");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Datos...");
  }
  if (startData){
    //si el bool startData es true quiere decir que debemos empezar la recoleccion de datos, asi que llamamos la funcion 
    almacenarDatos();
  }
  if(digitalRead(buttonInfo) == HIGH) { //boton que finaliza
    i=0;
    startInfo = true;
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Mostrandolos");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Datos...");
  }

  if(startInfo){
    startData=false;
    datos();
    startInfo=false;
  }
}

void almacenarDatos(){
 /*funcion que almacena los valores desde el generador 
 en el arreglo que empleamos al inicio*/
  val = analogRead(analogPin);
  val = val*(5.0/1023.0); //regla de tres para pasar los valores en terminos del voltaje
  signal[i] = val;
  Serial.println(val);
  delay(10);//usamos un tiempo de 10 ms para almacenar los datos 
  i++;//actualizamos el contador 
  if (i >= arregloSize){
    i = 0; //nos ayuda a empezar la recoleccion de datos nuevamente para asegurar la ejecucion del programa durante un tiempo prolongado
  }
 }
void mostrarDatos(float vol, float frec){
 /* funcion que muestra los 
 resultados en el lcd */
  lcd_1.setCursor(0,1);//ubicar el cursor
  lcd_1.print("v=");
  lcd_1.print(vol,2);//imprimir segudo vol y dar solo dos decimales
  lcd_1.setCursor(7,1);
  lcd_1.print("f=");
  lcd_1.print(frec,2);
}

void datos(){
  /*buscamos el max y min para los calculos, recorremos 
  el arreglo y usamos punteros*/
  lcd_1.clear();
 
  float min=signal[0];//empiezo desde la primera posicion
  float max=signal[0];
  float amplitud, frecuencia, periodo, t1, t2;
  float *maxnext;//puntero para la direccion de la señal
  float *dirsignal;//usamos puntero para no trabajar con variables contadoras
  int m=0;
  maxnext=signal;
  for(m;m<arregloSize;m++){
      if (signal[m]<min){
      min=signal[m];
    }
    if (signal[m]>max){
      max=signal[m];
      *maxnext=signal[m+1];
    }
  }
//amplitud

  if (min*max<0){
  /* si el minimo y maximo son de signos opuestos
  hacemos una suma con el abs del minimo*/
    amplitud=abs(min)+max;
  }
  else if (min*max>=0.0){
  /* si ambos son negativos o positivos su 
  multiplicacion queda positiva, por
  ende restamos los absolutos para hallar la amplitud y 
  esa resta le sacamos abs nuevamente para
  el caso de negativos*/
  
    amplitud=abs(abs(max)-abs(min));
  }

//frecuencia

  float cero= (min +max)/2; //buscamos la ubicacion del eje y que sera la mitad de la funcion
  /*Bucamos los cruces en el eje y
   para guardar las pociciones donde aparecen en t1 y t2*/
  dirsignal=signal;
  for (int j =0;j<arregloSize;j++){
    if (abs(*dirsignal-cero)<tolerancia){//
      t1=j;
      break;
    }
    dirsignal++;
  }
  for (int j=0;j<arregloSize;j++){
    if (abs(*dirsignal-cero)<tolerancia){
      t2=j;
      break;
    }
    dirsignal++;
  }
  float diferencia=abs(abs(max)-abs(*maxnext));
  //miramos cual es la diferencia, si son iguales quiere decir que es cuadrada ya que el valor maaximo siempre es el mismo por ende es cuadrada
  if (diferencia==0){
    lcd_1.setCursor(0,0);
    lcd_1.print("Cuadrada");
  }
  else if (diferencia < tolerancia && diferencia>0){
    //miramos que tan "rapido" pasa entre max y los comparamos con el valor de la tolerancia, entre mas grande sea sera una triangular
    lcd_1.setCursor(0,0);
    lcd_1.print("Senoidal");
  }
  else if (diferencia>tolerancia){
    lcd_1.setCursor(0,0);
    lcd_1.print("Triangular");
  }
  else {
    lcd_1.setCursor(0,0);
    lcd_1.print("Desconocida");
  }
  periodo= 2*10*abs(t2-t1)/100; //convierto a segundos, resto las pocisiones para saber el tiempo y multplico por dos para hallar la frecuencia completa, ya que solo hallamos la mitad del perido
  frecuencia=1/periodo; //calculamos la frecuencia con el inverso del periodo
  Serial.println(periodo);
  mostrarDatos(amplitud,frecuencia);
  delete [] signal;//libero memoria
}



