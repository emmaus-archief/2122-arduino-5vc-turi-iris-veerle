
/*   Stoplicht Startcode
   met toestansdiagrammen
   Emmauscollege
   v20201114GEE
*/
const int START = 0; // toestand is kalibreren
const int SENSOR_LEZEN = 1; // toestand is sensor lezen
const int RIJDEN = 2; // toestand is rijden in de richtingvan de lijn
const int DRAAIEN = 3; // toestand is draaien
int toestand = START;
int knop = 12;
bool lijn = false;

void setup() {
  pinMode(knop, OUTPUT);
  digitalWrite(knop, LOW);
}

void loop() {
  // lees sensorwaarden
  /*knop7 = digitalRead(pin7Knop);
  knop8 = digitalRead(pin8Knop);*/

  // bepaal toestand
  if (toestand == START) {
    if (knop == HIGH) 
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
    if () {
      
    }
    if () {
      
    }
  }

  
  if (toestand == DRAAIEN) {
    if () { 
      
    }
  }

  // zet stoplichten conform toestand
  if (toestand == LIJN VOLGEN) {
    for (int i = 1; i <= 8; i = i + 1) {
      stoplicht(i, ROOD);
    }
  }
  if (toestand == REMMEN) {
    for (int i = 1; i <= 8; i = i + 1) {
      stoplicht(i, GROEN);
    }
  }
  if (toestand == DRAAIEN/ZOEKEN) {
    for (int i = 1; i <= 6; i = i + 1) {
      stoplicht(i, ORANJE);
    }
  }

  // vertraging om te zorgen dat berichten op de seriele monitor leesbaar blijven
  delay(100);
}
