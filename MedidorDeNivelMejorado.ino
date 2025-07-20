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

//Se asignan alias a los pines correspondientes a los
//pulsadores encargados de realizar las calibraciones
#define calMax 4
#define calMin 3
#define calDefault 2

//Se crean dos variables que almacenan los niveles maximo y minimo en cm
float nivMax=10;//Nivel maximo
float nivMin=90;//Nivel minimo

//Se crean dos variables que almacenaran la lectura de tiempo
//que se tendra en los niveles maximos y minimos
int timeMin;
int timeMax;

//Se crea y almacenara la velocidad del sonido en cm/µs segun la temperatura
float sound;

//Se crea una variable que almacenara el tiempo leido por el sensor
float time;

//Se crea una variable que almacenara temporalmente el nuevo
//nivel que se calibrara
float newDis;

//Se crean variables que almacenan mensajes que se mostraran en
//pantalla cuando se calibre un nuevo nivel
String msgNew = "Nuevo nivel";
String msgMin = "minimo fijado";
String msgMax = "maximo fijado";
String msgReset1 = "Valores de nivel";
String msgReset2 = "restaurados";
String msgError1 = "Error, nivel";
String msgError2 = "no permitido";
 
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

//Se crea la funcion "printMsg", encargada de mostrar en pantalla
//mensajes al realizar ajustes de nivel
void printMsg(String msg1 , String msg2){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(msg1);
      lcd.setCursor(0,1);
      lcd.print(msg2);
      delay(5000);
      print();
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
  
//Se configuran los pines pertenecientes a los pulsadores
//como entradas PULLUP
  pinMode(calMin, INPUT_PULLUP);
  pinMode(calMax, INPUT_PULLUP);
  pinMode(calDefault, INPUT_PULLUP);

//Se inicializa el LCD y se inciende la luz de fondo
  lcd.init();
  lcd.backlight();
  
//Se imprime en pantalla la plantilla en la que se
//mostraran las mediciones
  print(); 
}

void loop()
{
//En la variable sound, se almacena 1/2 de la
//velocidad del sonido en cm/µs segun la temperatura
  sound = ((331.3+0.6*(((analogRead(A2)*500.)/1023.)-50))*0.00005);
  
//Se calculan los tiempos que tardara en llegar el eco
//en los niveles minimo y maximo
  timeMin=nivMax/sound;
  timeMax=nivMin/sound;
  
//Se enciende el pin trigger por 10µS
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
//Se obtiene el tiempo que a tardado en llegar el eco
  time = pulseIn(echo, HIGH);
  
//Se verifique que la medicion este dentro del rango establecido
  if (timeMin<=time && time<=timeMax){
  //Si la condicion se cumple:
  //Se imprime en pantalla la medicion en porcentaje de carga
    lcd.setCursor(8,0);
    lcd.print(100.-(((time-timeMin)*100.)/(timeMax-timeMin)));
    
    //Se imprime en pantalla la medicion en cm
    lcd.setCursor(8,1);
    lcd.print(nivMin-time*sound);
    
  } else {
  //Si la condicion no se cumple:
  //Se imprime en pantalla el texto "-----" en lugar de las mediciones
    lcd.setCursor(8,0);
    lcd.print("-----");
    lcd.setCursor(8,1);
    lcd.print("-----");
  }
  
//Se verifica si se a activado el pulsador
//correspondiente al nivel maximo
  if (digitalRead(calMax)==0){
  //Si se a activado se esperan tres segundos
    delay(3000);
  //Se almacena la ultima distancia medida
    newDis=time*sound;
  //Se vuelve a verificar que el pulsador sigue activado para
  //comprobar que no se trata de un error, ademas se verifica
  //que la distancia al nuevo nivel maximo sea menor a la distancia al nivel minimo,
  //que sea mayor o igual a 10cm y que sea menor a 200cm
    if (digitalRead(calMax)==0 && newDis<nivMin  && newDis>=10 && newDis<=200){
      //Si todas las condiciones se cumplen, se asigna el nuevo nivel y se muestra
      //en pantalla un mensaje de que se fijado un nuevo nivel maximo
      nivMax = newDis;
      printMsg(msgNew, msgMax);
    } else {
      //Si la condicion no se cumple, se muestra un mensaje de error
      printMsg(msgError1, msgError2);
    }
    
  } 
//Se verifica si se a activado el pulsador
//correspondiente al nivel minimo
  	else if (digitalRead(calMin)==0){
  //Si se a activado se esperan tres segundos
    delay(3000);
  //Se almacena la ultima distancia medida
    newDis=time*sound;
  //Se vuelve a verificar que el pulsador sigue activado para
  //comprobar que no se trata de un error, ademas se verifica
  //que la distancia al nuevo nivel maximo sea menor a la distancia al nivel minimo,
  //que sea mayor o igual a 10cm y que sea menor a 200cm
    if (digitalRead(calMin)==0 && newDis>nivMax && newDis<=200 && newDis>=10){
    //Si todas las condiciones se cumplen, se asigna el nuevo nivel y se muestra
    //en pantalla un mensaje de que se fijado un nuevo nivel maximo
      nivMin = newDis;
      printMsg(msgNew, msgMin);
    } else {
    //Si la condicion no se cumple, se muestra un mensaje de error
      printMsg(msgError1, msgError2);
    }
    
  } 
//Se verifica si se a activado el pulsador
//correspondiente a la calibracion por defecto
  else if (digitalRead(calDefault)==0){
    //Si se a activado se esperan tres segundos
    delay(3000);
    if (digitalRead(calDefault)==0){
    //Si la condicion se cumple, se restauran los valores de nivel originales
    //y se muestra un mensaje en pantalla
      nivMax=10, nivMin=90;
      printMsg(msgReset1, msgReset2);
    
    }
  }
//Se espera medio segundo antes de realizar la siguiente medicion
  delay(500);
 
}
