#include "TRSensors.h"

#define NUM_SENSORS 5 // alle 5 de ir 
// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
TRSensors trs =   TRSensors();
unsigned int sensorValues[NUM_SENSORS];


void setup()
{
  Serial.begin(115200);
  Serial.println("TRSensor example");
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
  delay(500);
}


void loop()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  // trs.AnalogRead(sensorValues); 
  // trs.readCalibrated(sensorValues);
  unsigned int position = trs.readLine(sensorValues);
  
  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }

  Serial.println(position); // comment this line out if you are using raw values
  
  delay(250);
}
