De readme.md vind je in de hoofdmap van deze repo.

const int pinAfstandTrigM = A4; // afstandssensor midden
const int pinAfstandEchoM = A5; // afstandssensor midden
const int pinAfstandTrigR = A2; // afstandssensor rechts
const int pinAfstandEchoR = A3; // afstandssensor rechts
const int pinAfstandTrigL = A0; // afstandssensor links
const int pinAfstandEchoL = A1; // afstandssensor links
const int pinMotorSnelheidR = 11; // motor rechts
const int pinMotorSnelheidL = 10; // motor links


long afstandR = 0;
long afstandL = 0;
long afstandM = 0;
int snelheidR = 0;
int  snelheidL = 0;
String regelBoven = "";
String regelOnder = "";