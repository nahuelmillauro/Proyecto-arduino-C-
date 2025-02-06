// C++ code
//

#define dt 1000
int sensorTierra = A0;  //el sensor de la tierra esta conectado al pin analogico A0
int sensorTemperatura = A1;
int boton = 13;  //boton en el pin 13
int bomba = 7;   //bomba en el pin
int suelo = 0;   //inicializo

unsigned long millisInicial = 0; //alamcena el valor hasta un numero entero de 32bits
unsigned long millisBoton = 0; //alamcena el valor hasta un numero entero de 32bits
int estadoBotonAnterior = LOW; //inicializa los estados del boton
int estadoBoton = LOW;

void setup() {
  pinMode(3, INPUT);   //temperatura
  pinMode(7, OUTPUT);  //bomba
  pinMode(A0, INPUT);  //sensor en la tierra
  pinMode(13, INPUT);  //boton
  Serial.begin(9600);
}

void loop() {
  unsigned long millisActuales = millis();
  int humedadTierra = analogRead(sensorTierra);
  float temperatura = map(((analogRead(sensorTemperatura) - 20) * 3.04), 0, 1023, -40, 125);
  estadoBoton = digitalRead(boton);
  if (humedadTierra <= 1026)  //falta calibrar y ver que valores tomamos como secos o mojados
  {
    Serial.println("Seco, activando bomba");
    digitalWrite(bomba, HIGH);  //Se activa la bomba
    delay(5000);                //durante 5 segundos
    digitalWrite(bomba, LOW);
  } else {
    digitalWrite(bomba, LOW);  //No se activa la bomba;
  }
  * /

    //programacion riego manual
    if ((estadoBoton == HIGH) && (estadoBotonAnterior == LOW)) {
    if ((millisActuales - millisBoton) >= 5) {
      Serial.println("Prendido");
      digitalWrite(bomba, HIGH);
      delay(5000);
      Serial.println("Apagado");
      estadoBotonAnterior = estadoBoton;
    }
  }
  else {
    millisBoton = millis();
    estadoBotonAnterior = estadoBoton;
  }

  humedadTierra = constrain(humedadTierra, 0, 1026);    //rango depende lo que mida el que tenemos en clase
  humedadTierra = map(humedadTierra, 0, 1026, 0, 100);  //porcentajes
  Serial.print("La humedad actual de la tierra es: ");
  Serial.print(humedadTierra);
  Serial.println("%");  //Esto imprime el porcentaje, hay que programarlo en el OLED11C
  Serial.print("La temperatura es: ");
  Serial.print(temperatura);
  Serial.println("C");
}
