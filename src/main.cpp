#include <Arduino.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11
#define DHT_SENSOR_PIN   2

#define LCD_RS_PIN       7
#define LCD_EN_PIN       8
#define LCD_D4_PIN       9
#define LCD_D5_PIN       10
#define LCD_D6_PIN       11
#define LCD_D7_PIN       12

DHT_Unified dht_sensor(DHT_SENSOR_PIN, DHTTYPE);
LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  dht_sensor.begin();
  lcd.setCursor(0, 0);
  lcd.print("Measuring...");
  Serial.println("OK: System ready");
}

void loop() {
  sensors_event_t tempEvent;
  sensors_event_t humidityEvent;

  dht_sensor.temperature().getEvent(&tempEvent);
  dht_sensor.humidity().getEvent(&humidityEvent);

  float currentTemp = tempEvent.temperature;
  float fahrenheit = (currentTemp * 9.0 / 5.0) + 32.0;

  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print("                "); // Clear line
  lcd.setCursor(0, 1);
  lcd.print(fahrenheit);
  lcd.print(" F");

  delay(2000);
}