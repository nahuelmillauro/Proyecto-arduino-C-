
#include "SSD1306Ascii.h" //biblioteca para controlar pantalla oled
#include "SSD1306AsciiAvrI2c.h"

// 0X3C+SA0 - 0x3C 0 0x3D
#define I2C_ADDRESS 0x3C //direccion de pantalla I2C


#define RST_PIN -1 //reset pin

SSD1306AsciiAvrI2c oled; //clase + objeto, manejamos hardware con oled
//------------------------------------------------------------------------------
void setup() {

//# son directivas prepocesador que se ejecutan antes de compilar
#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN); //tipo de pantalla, direccion, pinreset
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS); //tipo de pantalla, direccion
#endif // RST_PIN >= 0
  

  oled.setFont(Adafruit5x7); //se establece fuente de texto 5x7 pixeles por caracter

  uint32_t m = micros(); //variable m como entero de 32 bits, devuelve microsegundos
  oled.clear(); //limpia pantalla
  oled.println("Que ondaaaa");
  oled.println("A long line may be truncated");
  oled.println();
  oled.set2X();
  oled.println("2X demo");
  oled.set1X();
  oled.print("\nmicros: ");
  oled.print(micros() - m);
}
//------------------------------------------------------------------------------
void loop() {}
