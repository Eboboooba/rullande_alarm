/*

fil: rullande_alarm
författare: Ebba malm

*/

// Inkludera bibliotek för skärm (U8glib), RTC (Real-Time Clock) och I2C-kommunikation
#include <U8glib.h>
#include <RTClib.h>
#include <Wire.h>

// Definiera pinnar för motorer och sensorer
const int vensterhjul = 2;   // Vänsterhjul (motor)
const int hogerhjul = 3;     // Högerhjul (motor)

const int trigPin1 = 4;      // Triggerpin för avståndssensor 1
const int echoPin1 = 5;      // Echopin för avståndssensor 1
const int trigPin2 = 6;      // Triggerpin för avståndssensor 2
const int echoPin2 = 7;      // Echopin för avståndssensor 2
const int trigPin3 = 8;      // Triggerpin för avståndssensor 3
const int echoPin3 = 9;      // Echopin för avståndssensor 3

const int buzzer = A0;        // Pin för buzzern (larm)

const int buttonSetHour = 10;  // Knapp för att sätta timme
const int buttonSetMinute = 11;  // Knapp för att sätta minut
const int buttonToggleAlarm = 12; // Knapp för att aktivera/deaktivera alarm
const int buttonStopAlarm = 13;   // Knapp för att stoppa alarmet

// Deklarera variabler för tid och alarm
int hour;    // Aktuell timme
int minut;   // Aktuell minut
int second;  // Aktuell sekund

int ahour = 0;    // Alarmets timme
int aminut = 0;   // Alarmets minut
bool alarmSet = false;  // Om alarmet är satt eller inte

// Objekt för RTC (Real-Time Clock) och OLED-skärm
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

void setup() {
  // Starta seriekopplingen, I2C och RTC
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();

  // Sätt alla pinnar för sensorer och motorer som in- eller utgångar
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(hogerhjul, OUTPUT);
  pinMode(vensterhjul, OUTPUT);
  
  // Knapp-pinnar för att sätta alarmtid och aktivera/deaktivera alarm
  pinMode(buttonSetHour, INPUT);
  pinMode(buttonSetMinute, INPUT);
  pinMode(buttonToggleAlarm, INPUT);
  pinMode(buttonStopAlarm, INPUT_PULLUP);

  // Ställ in RTC-tid till den aktuella tidpunkten för kompileringsdatumet
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  u8g.setFont(u8g_font_unifont);  // Sätt teckensnittet för OLED-skärmen
}

void loop() {
  // Läs aktuell tid och uppdatera alarmet och skärmen
  
  getTime();
  setAlarm();
  checkAlarm();
  oledWrite(12, 12, getTime(), 12, 30, String(hour));
}

void fram() {
  // Kör båda motorerna framåt
  digitalWrite(hogerhjul, HIGH);
  digitalWrite(vensterhjul, HIGH);
  delay(50); // Kort paus
}

void venster() {
  // Kör höger motor och stoppa vänster motor för att svänga vänster
  digitalWrite(hogerhjul, HIGH);
  digitalWrite(vensterhjul, LOW);
  delay(500); // Håll svängen i 500 ms
}

void hoger() {
  // Kör vänster motor och stoppa höger motor för att svänga höger
  digitalWrite(hogerhjul, LOW);
  digitalWrite(vensterhjul, HIGH);
  delay(50);  // Kort paus
}

// Funktion för att mäta avstånd med hjälp av en ultraljudssensor
float distans(int pin, int echo) {
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);

  // Mät avståndet baserat på tid för ljudets resa fram och tillbaka
  float duration = pulseIn(echo, HIGH);
  float distance = (duration * .0343) / 2;
  return distance; // Returnera det beräknade avståndet
}

// Funktion för att få aktuell tid från RTC
String getTime() {
  DateTime now = rtc.now();

  hour = now.hour();   // Aktuell timme
  minut = now.minute(); // Aktuell minut
  second = now.second(); // Aktuell sekund

  // Formatera tid till en sträng
  String newTime = String(now.hour());
  if (now.minute() < 10) {
    newTime = newTime + ":" + "0" + String(now.minute());
  } else {
    newTime = newTime + ":" + String(now.minute());
  }

  if (now.second() < 10) {
    newTime = newTime + ":" + "0" + String(now.second());
  } else {
    newTime = newTime + ":" + String(now.second());
  }

  return newTime;
}

// Funktion för att aktivera buzzern (alarm)
void alarm() {
  tone(buzzer, 1000); // Spela upp en ton på buzzern
}

// Funktion för att skriva information till OLED-skärmen
void oledWrite(int x, int y, String text1, int x2, int y2, String text2) {
  u8g.firstPage();
  do {
    u8g.drawStr(x, y, text1.c_str());   // Visa tid på skärmen
    u8g.drawStr(x2, y2, (String(ahour) + ":" + String(aminut) + ":" + String("00")).c_str()); // Visa alarmtid
    u8g.drawStr(x2, y2+15, String(alarmSet).c_str()); // Visa om alarmet är aktiverat eller inte
  } while (u8g.nextPage());
}

// Funktion för att sätta alarmtiden via knappar
void setAlarm() {
  if (digitalRead(buttonSetHour) == HIGH) {
    ahour = (ahour + 1) % 24;  // Öka timmen med en och återställ till 0 vid 24
    delay(300);
  }
  if (digitalRead(buttonSetMinute) == HIGH) {
    aminut = (aminut + 1) % 60; // Öka minuten med en och återställ till 0 vid 60
    delay(300);
  }
  if (digitalRead(buttonToggleAlarm) == HIGH) {
    alarmSet = !alarmSet; // Växla mellan att sätta på eller stänga av alarmet
    delay(300);
  }
}

// Funktion för att kontrollera om alarmtiden har inträffat
void checkAlarm() {
  if (alarmSet && hour == ahour && minut == aminut) {
    alarm();  // Starta buzzern om alarmtiden har inträffat

    while (digitalRead(buttonStopAlarm) == LOW) { // Kör tills stoppknappen trycks
      fram();  // Rör sig framåt

      // Kontrollera om något är närmare än 20 cm från någon sensor
      if (distans(trigPin1, echoPin1) < 20 || distans(trigPin2, echoPin2) < 20 || distans(trigPin3, echoPin3) < 20) {
        venster();  // Sväng vänster om ett objekt är för nära
      }

      delay(100); // Kort paus för att minska responstiden
    }

    // noTone(buzzer);  // Stäng av buzzern när stoppknappen trycks
    alarmSet = false; // Återställ alarmet
    stopp();  // Stoppa bilen
  }
}

// Funktion för att stoppa alla motorer (bilen står stilla)
void stopp() {
  digitalWrite(hogerhjul, LOW);
  digitalWrite(vensterhjul, LOW);
}
