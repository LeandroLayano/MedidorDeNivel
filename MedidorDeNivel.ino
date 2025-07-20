/*
Proyecto: Medidor de nivel
Alumnos: Layano Leandro y Rodas Cynthia
Profesor: Ing. Diego Oscar R. Almeida
Asignatura: Laboratorio III
Instituto: I.T.S.
*/

//Se importan las librerias necesarias
//para la comunicacion con la pantalla LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Se inicializa la comunicacion con la pantalla LCD
LiquidCrystal_I2C lcd(0x27, 16,2);

//Se asignan alias a los pines que controlan el sensor HC-SR04
#define trigger 9
#define echo 8

//Se almacena en dos variables el tiempo en microsegundos
//que le llevaria al sonido en ir y volver a las distancias
//de 10cm y 90cm
int nivMax = 583; //10cm
int nivMin = 5248; //90cm

//Se crea y almacena el valor de 1/2 de la velocidad
//del sonido en cm/µs
float sound = 0.01718213058;

//Se crea una variable que almacenara el tiempo leido por el sensor
float time;

//Se crea la funcion "print", encargada de mostrar en pantalla
//la plantilla en la que se mostraran las mediciones
void print()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Nivel:");
  lcd.setCursor(14,0);
  lcd.print("%");
  lcd.setCursor(14,1);
  lcd.print("cm");
}


void setup()
{
//Se configuran los pines correspondientes a los terminales
//trigger y echo como salida y entrada respectivamente
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  
//Se limpian los pines trigger y echo
  digitalWrite(trigger, LOW);
  digitalWrite(echo, LOW);
  
//Se inicializa el LCD y se inciende la luz de fondo
  lcd.init();
  lcd.backlight();
  
//Se imprime en pantalla la plantilla en la que se
//mostraran las mediciones
  print(); 

}

void loop()
{
//Se enciende el pin trigger por 10µS
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
//Se obtiene el tiempo que a tardado en llegar el eco
  time = pulseIn(echo, HIGH);
  
//Se verifique que la medicion este dentro del rango establecido
  if (nivMax<=time && time<=nivMin){
  //Si la condicion se cumple:
  //Se imprime en pantalla la medicion en porcentaje de carga
    lcd.setCursor(8,0);
    lcd.print(100.-(((time-nivMax)*100.)/(nivMin-nivMax)));
    
  //Se imprime en pantalla la medicion en cm
    lcd.setCursor(8,1);
    lcd.print(90-time*sound);
    
  } else {
  //Si la condicion no se cumple:
  //Se imprime en pantalla el texto "-----" en lugar de las mediciones
    lcd.setCursor(8,0);
    lcd.print("-----");
    lcd.setCursor(8,1);
    lcd.print("-----");
  }
  
//Se espera medio segundo antes de realizar la siguiente medicion
  delay(500);
 
}
