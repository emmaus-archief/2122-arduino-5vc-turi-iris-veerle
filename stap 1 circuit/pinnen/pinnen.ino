#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "TRSensors.h"
#include <Wire.h>

#define PWMA   6           //Left Motor Speed pin (ENA)
#define AIN2   A0          //Motor-L forward (IN2).
#define AIN1   A1          //Motor-L backward (IN1)
#define PWMB   5           //Right Motor Speed pin (ENB)
#define BIN1   A2          //Motor-R forward (IN3)
#define BIN2   A3          //Motor-R backward (IN4)
#define PIN 7
#define NUM_SENSORS 5
#define OLED_RESET 9
#define OLED_SA0   8
#define Addr 0x20

#define beep_on  PCF8574Write(0xDF & PCF8574Read())
#define beep_off PCF8574Write(0x20 | PCF8574Read())
TRSensors trs =   TRSensors();
unsigned int sensorValues[NUM_SENSORS];
unsigned int last_proportional = 0;
unsigned int position;
long integral = 0;
uint16_t i, j;
byte value;
unsigned long lasttime = 0;
Adafruit_NeoPixel RGB = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);

void PCF8574Write(byte data);
byte PCF8574Read();
uint32_t Wheel(byte WheelPos);


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
