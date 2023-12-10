#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include "Kalman.h"
#include "MPU6050_6Axis_MotionApps20.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MPU6050 gyro;

int16_t AccX, AccY, AccZ, GyroX, GyroY, GyroZ;

double accelScale = 2.0 / 32768.0;
double gyroScale = 250.0 / 32768.0;

uint8_t fifoBuffer[45];

uint32_t timer;

extern const unsigned char logo[];

void setup() {

  Serial.begin(9600);

  startOLED();
  introductionOLED();

  gyro.initialize();
  gyro.dmpInitialize();
  gyro.setDMPEnabled(true);

  gyro.CalibrateGyro(10);
  gyro.CalibrateAccel(10);
}

void loop() {

  if (millis() - timer >= 11) {  // таймер на 11 мс (на всякий случай)
    if (gyro.dmpGetCurrentFIFOPacket(fifoBuffer)) {

      Quaternion q;
      VectorFloat gravity;
      float zyx[3];

      gyro.dmpGetQuaternion(&q, fifoBuffer);
      gyro.dmpGetGravity(&gravity, &q);
      gyro.dmpGetYawPitchRoll(zyx, &q, &gravity);

      Serial.print(zyx[0] * 180 / M_PI); // вокруг оси Z
      Serial.print(',');
      Serial.print(zyx[1] * 180 / M_PI); // вокруг оси Y
      Serial.print(',');
      Serial.print(zyx[2] * 180 / M_PI); // вокруг оси X
      Serial.println();

      display.clearDisplay();

      aimOLED();

      double pitch = getPitch(zyx[1]);

      double roll = getRoll(zyx[0]);

      int16_t yaw = getYaw(zyx[2]);
      
      compassOLED(yaw);

      display.drawLine(0, 32 + roll / 1.40625 + pitch / 1.8, 128, 32 - roll / 1.40625 + pitch / 1.8, WHITE);

      display.display();

      delay(100);
  
      timer = millis();
    }
  }
}

double getPitch(float y) {
  // Угол тангажа
  // pitch = atan2(acc_x, sqrt(acc_y^2 + acc_z^2))

  return y * 180 / M_PI;
}

double getRoll(float x) {
  // Угол крена
  // roll = atan2(-acc_y, acc_z)

  return x * 180 / M_PI;
}

double  getYaw(float z) {
  // Угол рыскания
  // yaw = gyro_z*dt

  return z * 180 / M_PI;
}

void compassOLED(int16_t yaw) {
  yaw = yaw * -1;
  if (yaw < 0) {
    yaw = 360 + yaw;
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(57, 6);
  display.print(yaw);
}

void startOLED() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
}

void introductionOLED() {
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

void aimOLED() {
  display.clearDisplay();
  display.drawLine(40, 32, 56, 32, WHITE);
  display.drawLine(72, 32, 88, 32, WHITE);
  display.drawLine(56, 32, 56, 36, WHITE);
  display.drawLine(72, 32, 72, 36, WHITE);
  display.drawLine(56, 36, 72, 36, WHITE);
}
