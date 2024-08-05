#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUTTON_PIN 16
#define BUZZER_PIN 18
#define LED_PIN 19

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
bool lastButtonState = LOW;
bool buttonPressed = false;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == HIGH && !buttonPressed) {
      buttonPressed = true;
      Serial.println("Button pressed!");
      ringBell();
      displayMessage("Doorbell Rung!");
      digitalWrite(LED_PIN, HIGH);
    } else if (reading == LOW) {
      buttonPressed = false;
      digitalWrite(LED_PIN, LOW);
    }
  }

  lastButtonState = reading;
}

void ringBell() {
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_PIN, 1000, 200);
    delay(200);
    noTone(BUZZER_PIN);
    delay(100);
  }
}

void displayMessage(const char* msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println(msg);
  display.display();
}
