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

  Serial.print("Temp: ");
  Serial.print(tempEvent.temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidityEvent.relative_humidity);
  Serial.println(" %");

  delay(2000); // Temporary blocking delay for sensor reads
}