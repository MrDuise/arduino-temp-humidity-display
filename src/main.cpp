#include <Arduino.h>
#include <LiquidCrystal.h>

// Pin definitions
#define LCD_RS_PIN       7
#define LCD_EN_PIN       8
#define LCD_D4_PIN       9
#define LCD_D5_PIN       10
#define LCD_D6_PIN       11
#define LCD_D7_PIN       12

LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Measuring...");
  Serial.println("OK: System ready");
}

void loop() {
  // Static placeholder for initial setup
}