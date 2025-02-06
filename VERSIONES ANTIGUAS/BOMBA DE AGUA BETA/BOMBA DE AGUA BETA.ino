#define dt 1000 //intervalo de tiempo entre mediciones
int SensorTierra = A0; //el sensor de la tierra esta conectado al pin analogico A0
int Suelo = 0; //inicializo
int boton = 13; //boton en el pin 13
int bomba = 7; //bomba en el pin 7
int estadoBoton; //almacena el estado del boton
void setup()
{
  pinMode(3, INPUT);//temperatura + humedad ambiente
  pinMode(7, OUTPUT);//bomba
  pinMode(A0, INPUT);//sensor en la tierra
  pinMode(13, INPUT); //boton de riego manual
  Serial.begin(9600);
}

void loop()
{
  int humedadTierra = analogRead(SensorTierra); //guarda la lectura del sensor de humedad en la variable
  Serial.println(humedadTierra); //mostrar en consola el valor del sensor
  if (humedadTierra <= 1026)//falta calibrar y ver que valores tomamos como secos o mojados
  	{
      	Serial.println("Seco, activando bomba");
    	digitalWrite(bomba, HIGH);//Se activa la bomba
    	delay(5000);//durante 5 segundos
    }
	  else 
      	{
          digitalWrite(bomba, LOW);//No se activa la bomba;
        }
  //programacion riego manual
  estadoBoton = digitalRead(boton); //guardamos en la variable el estado del boton
  if (estadoBoton==HIGH){
    Serial.println("Prendido"); //muestra en consola que esta prendido
    digitalWrite(bomba, HIGH);
  }
  else if (estadoBoton == LOW){
    digitalWrite(bomba, LOW);//no se presiona el boton
  }
 
    delay(dt);
    Serial.println(humedadTierra);
    int Suelo = analogRead(SensorTierra);
    Suelo = constrain(Suelo, 0, 1026); //rango depende lo que mida el que tenemos en clase
    Suelo = map(Suelo, 0, 1026, 0, 100); //se establecen los porcentajes 0=0% 1026=%100
  	Serial.println("La humedad actual de la tierra es: ");
    Serial.print(Suelo); //hay saltos de linea por la dimension de la pantalla
    Serial.println("%");//Esto imprime el porcentaje, hay que programarlo en el OLED11C
  	delay(dt);
}
