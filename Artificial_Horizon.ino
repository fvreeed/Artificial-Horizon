#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

extern const unsigned char logo[];

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  
  introduction();
}

void loop() {
  display.clearDisplay();
  display.drawLine(48, 32, 80, 32, WHITE);

  display.display();
}

void introduction() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(9, 22);
  display.print("Artificial Horizon");

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(18, 32);
  display.print("Moscow Aviation");

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(35, 42);
  display.print("Institute");

  display.display();

  delay(3000);

  display.clearDisplay();
  display.drawBitmap(0, 0, logo, 128, 64, WHITE);

  display.display();

  delay(3000);
}