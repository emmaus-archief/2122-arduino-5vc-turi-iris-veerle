
/*   Stoplicht Startcode
   met toestansdiagrammen
   Emmauscollege
   v20201114GEE
*/
#include "TRSensors.h"

#define NUM_SENSORS 5 // alle 5 de ir, sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
TRSensors trs =   TRSensors();
unsigned int sensorValues[NUM_SENSORS];

const int START = 0; // toestand is kalibreren
const int SENSOR_LEZEN = 1; // toestand is sensor lezen
const int RIJDEN = 2; // toestand is rijden in de richtingvan de lijn
const int DRAAIEN = 3; // toestand is draaien
int toestand = START;

#define PWMA   6           //Left Motor Speed pin (ENA)
#define AIN2   A0          //Motor-L forward (IN2).
#define AIN1   A1          //Motor-L backward (IN1)
#define PWMB   5           //Right Motor Speed pin (ENB)
#define BIN1   A2          //Motor-R forward (IN3)
#define BIN2   A3          //Motor-R backward (IN4)

int knop = 12;
int knopStatus = LOW;

bool lijn = false;

void setup() {
  Serial.begin(115200);

  pinMode(PWMA,OUTPUT);                     
  pinMode(AIN2,OUTPUT);      
  pinMode(AIN1,OUTPUT);
  pinMode(PWMB,OUTPUT);       
  pinMode(AIN1,OUTPUT);     
  pinMode(AIN2,OUTPUT);  
  
  pinMode(knop, INPUT);
  digitalWrite(knop, knopStatus);
}

void loop() {
  // lees sensorwaarden
  /*knop7 = digitalRead(pin7Knop);
  knop8 = digitalRead(pin8Knop);*/

  // bepaal toestand
  if (toestand == START) {
    knopStatus = digitalRead(knop);
    if (knopStatus == HIGH) {
     toestand = SENSOR_LEZEN;
    }
  }

  
  if (toestand == SENSOR_LEZEN) {
    if (lijn = true) {
     toestand = RIJDEN;
    }
    
    if (lijn = false) {
      toestand = DRAAIEN;
    }
  }

  
  if (toestand == RIJDEN) {
    
  }

  
  if (toestand == DRAAIEN) {
    
  }

  // zet stoplichten conform toestand
  if (toestand == START) {
    for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
     {
      trs.calibrate();       // reads all sensors 10 times
     }
    Serial.println("calibrate done");
    // print the calibration minimum values measured when emitters were on
    for (int i = 0; i < NUM_SENSORS; i++)
     {
      Serial.print(trs.calibratedMin[i]);
      Serial.print(' ');
     }
    Serial.println();
    
    // print the calibration maximum values measured when emitters were on
    for (int i = 0; i < NUM_SENSORS; i++)
     {
      Serial.print(trs.calibratedMax[i]);
      Serial.print(' ');
     }
    Serial.println();
  }

  
  if (toestand == SENSOR_LEZEN) {
    for (int i = 1; i <= 8; i = i + 1) {
      
    }
  }

  if (toestand == RIJDEN) {
    for (int i = 1; i <= 8; i = i + 1) {
      analogWrite(PWMA,50);
      digitalWrite(AIN1,LOW);
      digitalWrite(AIN2,HIGH);
      analogWrite(PWMB,50);
      digitalWrite(BIN1,LOW); 
      digitalWrite(BIN2,HIGH);
    }
  }

  
  if (toestand == DRAAIEN) {
    for (int i = 1; i <= 6; i = i + 1) {
      analogWrite(PWMA,50);
      digitalWrite(AIN1,LOW);
      digitalWrite(AIN2,HIGH);
      analogWrite(PWMB,50);
      digitalWrite(BIN1,HIGH); 
      digitalWrite(BIN2,LOW);
    }
  }

  // vertraging om te zorgen dat berichten op de seriele monitor leesbaar blijven
  delay(100);
}
