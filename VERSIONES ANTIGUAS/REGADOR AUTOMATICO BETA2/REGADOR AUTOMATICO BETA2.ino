#include <DHT.h>                 //LIBRERIAS SENSOR AMBIENTAL
#include <DHT_U.h>               //LIBRERIAS SENSOR AMBIENTAL
#include "SSD1306Ascii.h"        //LIBRERIA PANTALLA OLED
#include "SSD1306AsciiAvrI2c.h"  //LIBRERIA PANTALLA OLED


// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C  //CODIGO DE SERIE DE PANTALLA OLED

// Defina RST_PIN adecuado si es necesario.
#define RST_PIN 10

SSD1306AsciiAvrI2c oled;
//SACADO EJEMPLO PANTALLA

//SENSOR AMBIENTE
int sensor = 3;
int temperatura;
int humedad;
DHT dht(sensor, DHT11);

int sensorTierra = A0;  //el sensor de la tierra esta conectado al pin analogico A0
int boton = 13;         //boton en el pin 13
int bomba = 7;          //bomba en el pin 7
int cooler = 2;         // cooler en el pin 2
int pantalla = 10;

unsigned long millisInicial = 0;
unsigned long millisBoton = 0;
unsigned long millisPantalla = 0;
int estadoBotonAnterior = LOW;
int estadoBoton = LOW;
int estadoPantalla = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  //SENSOR AMBIENTE
  pinMode(3, INPUT);   //temperatura + humedad ambiente
  pinMode(7, OUTPUT);  //bomba
  pinMode(A0, INPUT);  //sensor en la tierra
  pinMode(13, INPUT);  //boton
  pinMode(2, OUTPUT);  //cooler
  oled.setFont(Adafruit5x7);

//EJEMPLO PANTALLA
#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else   // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif  // RST_PIN >= 0
  // Llame a oled.setI2cClock(frequency) para cambiar la frecuencia predeterminada
  //EJEMPLO PANTALLA
}


void imprimirValores(int temperatura, int humedad, int suelo, int millisActuales) {

  if ((millisActuales - millisPantalla) >= 2000) {
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
        humedadTierra = constrain(humedadTierra, 12, 1022);    //rango depende lo que mida el que tenemos en clase
        humedadTierra = map(humedadTierra, 1022, 12, 0, 100);  //porcentajes
        oled.print(humedadTierra);
        oled.println("%");
        estadoPantalla--;
        break;
    }
  }
}

//Prende la bomba por 5 segundos.
void regar() {
  oled.clear();
  oled.println("----REGANDO----");
  digitalWrite(bomba, HIGH);  //Se activa la bomba
  delay(5000);                //durante 5 segundos
  digitalWrite(bomba, LOW);
  oled.clear();
}


void loop() {
  unsigned long millisActuales = millis();
  estadoBoton = digitalRead(boton);
  int humedadTierra = analogRead(sensorTierra);
  Serial.println(humedadTierra);
  //SENSOR AMBIENTE
  temperatura = dht.readTemperature();
  humedad = dht.readHumidity();
  imprimirValores(temperatura, humedad, humedadTierra, millisActuales);


  if (humedadTierra >= 700) {
    regar();
  } else {
    digitalWrite(bomba, LOW);  //No se activa la bomba;
  }

  //programacion riego manual
  if ((estadoBoton == HIGH) && (estadoBotonAnterior == LOW)) {
    if ((millisActuales - millisBoton) >= 5) {
      Serial.println("Prendido");
      regar();
      Serial.println("Apagado");
      estadoBotonAnterior = estadoBoton;
    }
  } else {
    millisBoton = millis();
    estadoBotonAnterior = estadoBoton;
  }

  if (temperatura > 26) {  //falta calibrar
    digitalWrite(cooler, HIGH);
  } else {
    digitalWrite(cooler, LOW);
  }
}