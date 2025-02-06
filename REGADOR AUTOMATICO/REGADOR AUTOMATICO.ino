#include <DHT.h>                 //LIBRERIAS SENSOR AMBIENTAL
#include <DHT_U.h>               //LIBRERIAS SENSOR AMBIENTAL
#include "SSD1306Ascii.h"        //LIBRERIA PANTALLA OLED
#include "SSD1306AsciiAvrI2c.h"  //LIBRERIA PANTALLA OLED


// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C  //CODIGO DE SERIE DE PANTALLA OLED

// Defina RST_PIN adecuado si es necesario.
#define RST_PIN 10 

SSD1306AsciiAvrI2c oled; //clase + objeto, manejamos hardware con "oled"


//SENSOR AMBIENTE
int sensor = 3; //Sensor DHT11 PIN 3
int temperatura;
int humedad;
DHT dht(sensor, DHT11); //Configuracion de sensor, pin + tipo

//SENSOR TIERRA
int sensorTierra = A0;  //el sensor de la tierra esta conectado al pin analogico A0
int boton = 13;         //boton en el pin 13
int bomba = 7;          //bomba en el pin 7
int cooler = 2;         // cooler en el pin 2
int pantalla = 10;

unsigned long millisBoton = 0; //almacena el valor hasta de un numero entero de 32bits
unsigned long millisPantalla = 0; //almacena el valor hasta de un numero entero de 32bits
int estadoBotonAnterior = LOW; //inicializa los estados del boton
int estadoBoton = LOW;
int estadoPantalla = 0;

void setup() {
  Serial.begin(9600);
  dht.begin(); //inicializa sensores de humedad y temperatura
 
  pinMode(3, INPUT);   //temperatura + humedad ambiente
  pinMode(7, OUTPUT);  //bomba
  pinMode(A0, INPUT);  //sensor en la tierra
  pinMode(13, INPUT);  //boton
  pinMode(2, OUTPUT);  //cooler
  oled.setFont(Adafruit5x7); //"INICIA PANTALLA"


#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN); //tipo de pantalla, direccion y resetpin
#else   // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);//inicializa pantalla sin resetpin
#endif  // RST_PIN >= 0  

}


void imprimirValores(int temperatura, int humedad, int suelo, int millisActuales) { //FUNCION

  if ((millisActuales - millisPantalla) >= 5000) { //cada 5 segundos muestra valores en pantalla.
    millisPantalla = millisActuales;
    switch (estadoPantalla) {
      case 0:
        oled.clear();
        oled.println("----SENSOR AMBIENTE----");
        oled.print("Temperatura: ");
        oled.print(temperatura);
        oled.print("C");
        oled.println();
        oled.print("Humedad: ");
        oled.print(humedad);
        oled.println("%");
        estadoPantalla++;
        break;
      case 1:
        oled.clear();
        oled.println("----SENSOR TIERRA----");
        oled.println("La humedad actual de la tierra es: ");
        int humedadTierra = suelo;
        humedadTierra = constrain(humedadTierra, 12, 1022);    //rango de mediciones tomadas en clase
        humedadTierra = map(humedadTierra, 1022, 12, 0, 100);  //porcentajes (12 es mojado, 1022 es SECO)
        oled.print(humedadTierra);
        oled.println("%");
        estadoPantalla--;
        break;
    }
  }
}

//Prende la bomba por 5 segundos.
void regar() { //FUNCION que activa la bomba durante 5 seg
  oled.clear();
  oled.println("----REGANDO----");
  digitalWrite(bomba, HIGH);  
  delay(5000);                
  digitalWrite(bomba, LOW);
  oled.clear();
}


void loop() {
  unsigned long millisActuales = millis();
  estadoBoton = digitalRead(boton);
  int humedadTierra = analogRead(sensorTierra);
  Serial.println(humedadTierra);
 
  temperatura = dht.readTemperature();
  humedad = dht.readHumidity();
  imprimirValores(temperatura, humedad, humedadTierra, millisActuales);


  if (humedadTierra >= 700) {
    regar();
  } else {
    digitalWrite(bomba, LOW);  //No se activa la bomba;
  }

  //RIEGO MANUAL
  if ((estadoBoton == HIGH) && (estadoBotonAnterior == LOW)) {
    if ((millisActuales - millisBoton) >= 2) {
      Serial.println("Prendido");
      regar();
      Serial.println("Apagado");
      estadoBotonAnterior = estadoBoton;
    }
  } else {
    millisBoton = millis();
    estadoBotonAnterior = estadoBoton;
  }


  if (temperatura > 26) { 
    digitalWrite(cooler, HIGH); //COOLER ACTIVADO
  } else {
    digitalWrite(cooler, LOW);
  }
}