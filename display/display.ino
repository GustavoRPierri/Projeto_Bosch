#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//'Adafruit BusIO' biblioteca para baixar

String L[4];
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(128, 64);

void setup() {
  Serial.begin(115200);
  Wire.begin(14,15);
  L[0] = "Carro";
  L[1] = "Rua"; 
  L[2] = "Semafaro";
  L[3] = "Peneu";

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println(L[0]);
  display.println(L[1]);
  display.println(L[2]);
  display.println(L[3]);
  display.println(L[4]);
  display.display(); 
}

void loop() {
  
}
