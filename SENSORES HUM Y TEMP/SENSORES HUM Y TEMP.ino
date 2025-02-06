
#include <DHT.h> //biblioteca para sensores DHT (11, 22)
#include <DHT_U.h>




#include "SSD1306Ascii.h" //biblioteca para controlar pantallas oled con comunicacion I2C
#include "SSD1306AsciiAvrI2c.h"//biblioteca para controlar pantallas oled con comunicacion I2C

// 0X3C+SA0 - 0x3C o 0x3D
#define I2C_ADDRESS 0x3C //Dirección I2C del display OLED


#define RST_PIN -1 //se define pin de reset de la pantalla (se establece -1 por que no se usa)

SSD1306AsciiAvrI2c oled; //clase + objeto //oled permite manejar el hardware de la pantalla
//SACADO EJEMPLO PANTALLA

//SENSOR AMBEINTE
int Sensor = 3; //pin 3 para el sensor DHT11
int Temperatura;
int Humedad;

DHT dht(Sensor, DHT11);//se configura sensor DHT indicando el pin(sensor) y el tipo

void setup() {
 Serial.begin(9600);
 dht.begin(); //inicializa el sensor humedad y temperatura
 //SENSOR AMBEINTE

 //# son para direcctivas preprocesador, se ejecutan antes de compilar
 #if RST_PIN >= 0 
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN); //tipo de pantalla, direccion y resetpin
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS); //inicializa pantalla sin resetpin
#endif 
  
}



void loop() {
  //SENSOR AMBEINTE
  Temperatura = dht.readTemperature(); //lectura de valores
  Humedad = dht.readHumidity(); // lectura de valores

  Serial.print("Temperatura: ");
  Serial.print(Temperatura);
  Serial.println("°C");
  Serial.print("Humedad: ");
  Serial.print(Humedad);
  Serial.println("%");
  delay(2000);
//SENSOR AMBIENTE

//PANTALLA
  oled.setFont(Adafruit5x7); //se establece fuente de texto 5x7 pixeles por caracter

  uint32_t m = micros(); //variable m como entero de 32 bits, devuelve microsegundos
  oled.clear(); //limpia pantalla
  oled.print("Temperatura: ");
  oled.print(Temperatura);
  oled.print("°C");
  oled.println();
  oled.print("Humedad: ");
  oled.print(Humedad);
  oled.println("%");
  delay(2000); //muestra informacion por 2 segundos y luego limpia pantalla
//PANTALLA
}
  
 

  