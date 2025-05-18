#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Runtime counters
volatile unsigned long revCount = 0;
unsigned long totalRevs = 0;
unsigned int rpm = 0;
float rph = 0;

// Timing
unsigned long lastUpdate = 0;
unsigned long startTime = 0;
unsigned long lastPulseTime = 0;
unsigned long lastPulseReceived = 0;
unsigned long lastEEPROMWrite = 0;

// Constants
const unsigned long debounceDelay = 10;     // ms
const unsigned long pulseTimeout = 22000;   // 22 seconds sensor fail
const unsigned long eepromInterval = 30000; // Save every 30 seconds

// LCD update memory
unsigned int prevRpm = 99999;
unsigned long prevRph = 999999;

void setup() {
  pinMode(2, INPUT_PULLUP);  // IR sensor input
  pinMode(4, INPUT_PULLUP);  // Reset button input

  attachInterrupt(digitalPinToInterrupt(2), countRevolution, FALLING);

  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  EEPROM.get(0, totalRevs);  // Load saved total revolutions
  startTime = millis();
}

void loop() {
  // Check for serial reset command
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();  // Remove whitespace/newlines

    if (input.equalsIgnoreCase("reset") || input.equalsIgnoreCase("r")) {
      resetCounters();
      Serial.println("**** Reset via Serial ****");
    }
  }

  unsigned long now = millis();

  // Manual reset button pressed (active LOW)
  if (digitalRead(4) == LOW) {
    resetCounters();
    Serial.println("**** Reset via Button ****");
    delay(500);  // Simple debounce delay for button
  }

  if (now - lastUpdate >= 1000) {
    noInterrupts();
    unsigned long count = revCount;
    revCount = 0;
    interrupts();

    if (count == 0) {
      if (now - lastPulseReceived > pulseTimeout) {
        rpm = 0;
        rph = 0;
        totalRevs = 0;
      } else {
        rpm = 0;
      }
    } else {
      rpm = count * 60;
      totalRevs += count;
      lastPulseReceived = now;

      float elapsedHours = (now - startTime) / 3600000.0;
      rph = (elapsedHours > 0) ? (totalRevs / elapsedHours) : 0;
    }

    // EEPROM save every 30 seconds
    if (now - lastEEPROMWrite > eepromInterval) {
      EEPROM.put(0, totalRevs);
      lastEEPROMWrite = now;
    }

    // Update LCD only if values changed
    if (rpm != prevRpm || (unsigned long)rph != prevRph) {
      lcd.setCursor(0, 0);
      lcd.print("   Tachometer     ");  // Clear rest of line

      lcd.setCursor(0, 1);
      lcd.print("RPM:");
      lcd.print(rpm);
      lcd.print(" ");

      int rpmDigits = (rpm < 10) ? 1 : (rpm < 100) ? 2 : (rpm < 1000) ? 3 : 4;
      int padding = 7 - (4 + rpmDigits);  // 4 = length of "RPM:"

      for (int i = 0; i < padding; i++) lcd.print(" ");

      lcd.print("RPH:");
      lcd.print((unsigned long)rph);
      lcd.print("    ");

      prevRpm = rpm;
      prevRph = (unsigned long)rph;
    }

    // Serial output for monitoring
    Serial.print("RPM: ");
    Serial.print(rpm);
    Serial.print(" | RPH: ");
    Serial.print((unsigned long)rph);
    Serial.print(" | Total: ");
    Serial.println(totalRevs);

    lastUpdate = now;
  }
}

void countRevolution() {
  unsigned long currentTime = millis();
  if (currentTime - lastPulseTime > debounceDelay) {
    revCount++;
    lastPulseTime = currentTime;
  }
}

void resetCounters() {
  totalRevs = 0;
  rpm = 0;
  rph = 0;
  startTime = millis();
  EEPROM.put(0, totalRevs);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Tachometer     ");
  lcd.setCursor(0, 1);
  lcd.print("RPM:0    RPH:0 ");

  prevRpm = 0;
  prevRph = 0;
}
