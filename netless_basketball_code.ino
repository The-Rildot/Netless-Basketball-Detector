/*
Author: Rony Tsirkel
Date: 12/1/2023
Purpose: Detects when an object moves in front of the sensors. Once an object is detected make a sound, flash an LED, and increment a counter.
*/

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
hd44780_I2Cexp lcd; // correct LCD setup

int LED = 7;
int buzzer = 6;
int PIR = 8;
int pirstate = 0;
int echo = 10;
int ping = 9;
int pirV = 0;
int count = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ping, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(PIR, INPUT);

  lcd.begin(16, 2);

  turnOn();
}

//Prints a starting message that informs the user that the device is on
void turnOn() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Device ON");

  delay(2000);
}

void loop() {
  long duration, inches;

  lcd.setCursor(0, 0);
  lcd.print("Number of Balls:");
  lcd.setCursor(0, 1);
  lcd.print(count);

  digitalWrite(ping, LOW);
  delayMicroseconds(2);
  digitalWrite(ping, HIGH);
  delayMicroseconds(10);
  digitalWrite(ping, LOW);

  duration = pulseIn(echo, HIGH);
  inches = MicrosecondsToInches(duration);

  pirV = digitalRead(PIR);
  if (pirV == HIGH) {
    if (pirstate == LOW) {
      if (inches <= 18) {//Average baskeball hoop diameter is 18 inches
        //Ball has been detected

        digitalWrite(LED, HIGH);
        count++;
        tone(buzzer, 2000, 200);
        delay(500);
        tone(buzzer, 2000, 200);
        delay(500);
        tone(buzzer, 2000, 200);

        pirstate = HIGH;
      }
    }
  } else {
    if (pirstate == HIGH) {
      //No ball in sight
      digitalWrite(LED, LOW);
      noTone(buzzer);

      pirstate = LOW;
    }
  }
}

long MicrosecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}
