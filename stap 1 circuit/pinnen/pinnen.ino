#include "TRSensors.h"
#include <Wire.h>

#define NUM_SENSORS 5 // alle 5 de ir, sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
TRSensors trs =   TRSensors();
unsigned int sensorValues[NUM_SENSORS];

#define Addr  0x20
#define beep_on  PCF8574Write(0xDF & PCF8574Read())
#define beep_off PCF8574Write(0x20 | PCF8574Read())
byte value;
void PCF8574Write(byte data);
byte PCF8574Read();

const int START = 1; // toestand is kalibreren
const int RIJDEN = 2; // toestand is rijden in de richtingvan de lijn
const int DRAAIENL = 3; // toestand is draaien linksom
const int DRAAIENR = 4; // toestand is draaien rechtsom
int toestand = START;

#define PWMA   6           //Left Motor Speed pin (ENA)
#define AIN2   A0          //Motor-L forward (IN2).
#define AIN1   A1          //Motor-L backward (IN1)
#define PWMB   5           //Right Motor Speed pin (ENB)
#define BIN1   A2          //Motor-R forward (IN3)
#define BIN2   A3          //Motor-R backward (IN4)

unsigned int position;
int proportional;
unsigned long lasttime = 0;
int snelheid;


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
