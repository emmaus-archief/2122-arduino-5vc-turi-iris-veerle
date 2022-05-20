
/*   Stoplicht Startcode
   met toestansdiagrammen
   Emmauscollege
   v20201114GEE
*/
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
const int DRAAIEN = 3; // toestand is draaien
int toestand = START;

#define PWMA   6           //Left Motor Speed pin (ENA)
#define AIN2   A0          //Motor-L forward (IN2).
#define AIN1   A1          //Motor-L backward (IN1)
#define PWMB   5           //Right Motor Speed pin (ENB)
#define BIN1   A2          //Motor-R forward (IN3)
#define BIN2   A3          //Motor-R backward (IN4)

bool lijn = true;
unsigned int position;
int proportional;
unsigned long lasttime = 0;
void setup() {
  Serial.begin(115200);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

}


void loop() {
  // lees sensorwaarden
  /*knop7 = digitalRead(pin7Knop);
    knop8 = digitalRead(pin8Knop);*/
  /* while (value != 0xEF) //wait button pressed
    {
     PCF8574Write(0x1F | PCF8574Read());
     value = PCF8574Read() | 0xE0;
     toestand = START;
     Serial.println("knop geklikt");
     value = 0;
    }

  */
  // bepaal toestand
  if (toestand == START) {
    if (millis() - lasttime > 5000) {
      lasttime = millis();
      Serial.println( "START klaar");
      toestand = RIJDEN;
    }
  }




  if (toestand == RIJDEN) {
    if (proportional < -500 || proportional > 500) {
      lasttime = millis();
      Serial.println( "klaar rijden");
      toestand = DRAAIEN;
    }
  }

  if (toestand == DRAAIEN) {
    if (millis() - lasttime > 1000  ) {
      lasttime = millis();
      Serial.println( "DRAAIEN klaar");
      toestand = RIJDEN;
    }
  }


  // zet Robot conform toestand
  if (toestand == START) {
    Serial.println("START");
    analogWrite(PWMA, 60);
    analogWrite(PWMB, 60);
    for (int i = 0; i < 100; i++)  // make the calibration take about 10 seconds
    {
      if (i < 25 || i >= 75)
      {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, HIGH);
      }
      else
      {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
        digitalWrite(BIN1, HIGH);
        digitalWrite(BIN2, LOW);
      }
      trs.calibrate();       // reads all sensors 100 times
      for (int i = 0; i < NUM_SENSORS; i++)
      {
        Serial.print(trs.calibratedMin[i]);
        Serial.print(' ');
      }
      Serial.println();
    }

  }


  /* if (toestand == SENSOR_LEZEN) {
     for (int i = 1; i <= 4; i = i + 1) {
       position = trs.readLine(sensorValues);
       int proportional = (int)position - 2000;
       Serial.println("sensor lezen begin");
       // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
       // 1000 means minimum reflectance, followed by the line position
       for (unsigned char i = 0; i < NUM_SENSORS; i++)
       {
         Serial.print(sensorValues[i]);
         Serial.print('\t');
       }
       Serial.println();
       Serial.print(position); // comment this line out if you are using raw values
       Serial.println();
       Serial.print(proportional);
       Serial.println();

     }
    }
  */


  if (toestand == RIJDEN) {
    Serial.println( "rijden");
    position = trs.readLine(sensorValues);
    proportional = (int)position - 2000;

    Serial.print("Het midden van de lijn zit op positie: ");
    Serial.println(proportional);

  
    analogWrite(PWMA, 51);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMB, 50);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);


  }


  if (toestand == DRAAIEN) {
    // Serial.println( "DRAAIEN");

    analogWrite(PWMA, 50);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMB, 50);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
  }



  // vertraging om te zorgen dat berichten op de seriele monitor leesbaar blijven
  delay(100);
}

void PCF8574Write(byte data)
{
  Wire.beginTransmission(Addr);
  Wire.write(data);
  Wire.endTransmission();
}

byte PCF8574Read()
{
  int data = -1;
  Wire.requestFrom(Addr, 1);
  if (Wire.available()) {
    data = Wire.read();
  }
  return data;
}
