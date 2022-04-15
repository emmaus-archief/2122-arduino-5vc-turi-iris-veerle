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

Adafruit_SSD1306 display(OLED_RESET, OLED_SA0);

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
  Serial.begin(115200);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println("WaveShare");
  display.setCursor(10,25);
  display.println("AlphaBot2");
  display.setTextSize(1);
  display.setCursor(10,55);
  display.println("Press to calibrate");
  display.display();
  
  while(value != 0xEF)  //wait button pressed
  {
    PCF8574Write(0x1F | PCF8574Read());
    value = PCF8574Read() | 0xE0;
  }
  
  pinMode(PWMA,OUTPUT);                     
  pinMode(AIN2,OUTPUT);      
  pinMode(AIN1,OUTPUT);
  pinMode(PWMB,OUTPUT);       
  pinMode(AIN1,OUTPUT);     
  pinMode(AIN2,OUTPUT);  
  analogWrite(PWMA,0);
  analogWrite(PWMB,0);
  digitalWrite(AIN2,HIGH);
  digitalWrite(AIN1,LOW);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);  
  RGB.begin();
  RGB.setPixelColor(0,0x00FF00 );
  RGB.setPixelColor(1,0x00FF00 );
  RGB.setPixelColor(2,0x00FF00 );
  RGB.setPixelColor(3,0x00FF00);
  RGB.show(); 
  delay(500);
  analogWrite(PWMA,80);
  analogWrite(PWMB,80);
  for (int i = 0; i < 100; i++)  // make the calibration take about 10 seconds
  {
    if(i<25 || i >= 75)
    {
      digitalWrite(AIN1,HIGH);
      digitalWrite(AIN2,LOW);
      digitalWrite(BIN1,LOW); 
      digitalWrite(BIN2,HIGH);  
    }
    else
    {
      digitalWrite(AIN1,LOW);
      digitalWrite(AIN2,HIGH);
      digitalWrite(BIN1,HIGH); 
      digitalWrite(BIN2,LOW);  
    }
    trs.calibrate();       // reads all sensors 100 times
  }
  analogWrite(PWMA,0);
  analogWrite(PWMB,0);
  digitalWrite(AIN2,LOW);
  digitalWrite(AIN1,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,LOW);  
  RGB.setPixelColor(0,0x0000FF );
  RGB.setPixelColor(1,0x0000FF );
  RGB.setPixelColor(2,0x0000FF );
  RGB.setPixelColor(3,0x0000FF);
  RGB.show(); // Initialize all pixels to 'off'
  
  value = 0;
  while(value != 0xEF)  //wait button pressed
  {
    PCF8574Write(0x1F | PCF8574Read());
    value = PCF8574Read() | 0xE0;
    position = trs.readLine(sensorValues)/200;
    display.clearDisplay();
    display.setCursor(0,25);
    display.println("Calibration Done !!!");
    display.setCursor(0,55);
    for (int i = 0; i < 21; i++)
    {
      display.print('_');
    }
    display.setCursor(position*6,55);
    display.print("**");
    display.display();
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println("AlphaBot2");
  display.setTextSize(3);
  display.setCursor(40,30);
  display.println("Go!");
  display.display();
  
  delay(500);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);  
}

void loop() {
  // put your main code here, to run repeatedly:
  // Get the position of the line.  Note that we *must* provide
  // the "sensors" argument to read_line() here, even though we
  // are not interested in the individual sensor readings.
  //trs.AnalogRead(sensorValues); 
  position = trs.readLine(sensorValues);
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
   Serial.print(sensorValues[i]);
   Serial.print('\t');
  }
  Serial.println();
  Serial.println(position); // comment this line out if you are using raw values

  // The "proportional" term should be 0 when we are on the line.
  int proportional = (int)position - 2000;
  
  // Compute the derivative (change) and integral (sum) of the position.
  int derivative = proportional - last_proportional;
  integral += proportional;

  // Remember the last position.
  last_proportional = proportional;
    
  // Compute the difference between the two motor power settings,
    // m1 - m2.  If this is a positive number the robot will turn
    // to the right.  If it is a negative number, the robot will
    // turn to the left, and the magnitude of the number determines
    // the sharpness of the turn.
    int power_difference = proportional/20 + integral/10000 + derivative*10;  

  // Compute the actual motor settings.  We never set either motor
  // to a negative value.
  const int maximum =110;

  if (power_difference > maximum)
    power_difference = maximum;
  if (power_difference < - maximum)
    power_difference = - maximum;
    Serial.println(power_difference);
    
  if (power_difference < 0)
  {
     analogWrite(PWMA,maximum + power_difference);
     analogWrite(PWMB,maximum);
   }
   else
   {
      analogWrite(PWMA,maximum);
      analogWrite(PWMB,maximum - power_difference);
   }      
  
  if (sensorValues[1] > 900 && sensorValues[2] > 900 && sensorValues[3] > 900)
  {
    // There is no line .Must Stop.
    analogWrite(PWMA,0);
    analogWrite(PWMB,0);
  }

//  PCF8574Write(0xC0 | PCF8574Read());   //set Pin High
//  value = PCF8574Read() | 0x3F;         //read Pin
//  while(value != 0xFF)
//  {
//    PCF8574Write(0xC0 | PCF8574Read());   //set Pin High
//    value = PCF8574Read() | 0x3F;         //read Pin
//    analogWrite(PWMA,0);
//    analogWrite(PWMB,0);
//    beep_on;
//  }
//  beep_off;
  
  if(millis() - lasttime >200){  
    lasttime = millis();   
    for(i=0; i< RGB.numPixels(); i++) {
      RGB.setPixelColor(i, Wheel(((i * 256 / RGB.numPixels()) + j) & 255));
    }
    RGB.show();
    if(j++ > 256*4) j= 0;
  }

  //delay(250);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return RGB.Color(WheelPos * 50, 255 - WheelPos * 50, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return RGB.Color(255 - WheelPos * 50, 0, WheelPos * 50);
  } else {
   WheelPos -= 170;
   return RGB.Color(0, WheelPos * 50, 255 - WheelPos * 50);
  }
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
  if(Wire.available()) {
    data = Wire.read();
  }
  return data;
}
