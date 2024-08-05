#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TM1637Display.h>

#define TRIG_PIN 3
#define ECHO_PIN 2
#define CLK_PIN 5
#define DIO_PIN 4

LiquidCrystal_I2C lcd(0x27, 16, 2);
TM1637Display display(CLK_PIN, DIO_PIN);

const float SOUND_SPEED = 340.0 / 10000.0;

float measureDistance() {
  long duration;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * SOUND_SPEED / 2;
  return distance;
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lcd.init();
  lcd.backlight();

  display.setBrightness(0x0f);

  lcd.setCursor(0, 0);
  lcd.print("Distance:     cm");

  delay(2000);
}

void loop() {
  float distance = measureDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  lcd.setCursor(10, 0);
  lcd.print("    ");
  lcd.setCursor(10, 0);
  lcd.print(distance, 1);

  if (distance < 1000) {
    display.showNumberDec((int)distance);
  } else {
    display.showNumberDecEx(9999, 0b01000000);
  }

  delay(1000);
}
