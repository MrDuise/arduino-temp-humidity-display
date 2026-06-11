#include <Arduino.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11
#define DHT_SENSOR_PIN   2
#define BUTTON_PIN       3

#define LCD_RS_PIN       7
#define LCD_EN_PIN       8
#define LCD_D4_PIN       9
#define LCD_D5_PIN       10
#define LCD_D6_PIN       11
#define LCD_D7_PIN       12
#define DEBOUNCE_DELAY_MS 50

static unsigned long lastDebounceTime = 0;
static float currentTemp     = 0.0f;
static float currentHumidity = 0.0f;
static int lastButtonState = HIGH;
static unsigned long measurementTimestamp = 0;

typedef enum {
    DISPLAY_MODE_TEMPERATURE = 0,
    DISPLAY_MODE_HUMIDITY    = 1      
} DisplayMode_t;

static DisplayMode_t displayMode = DISPLAY_MODE_TEMPERATURE;

static bool measure_environment(float *temperature, float *humidity);
static void updateDisplay(float temperature, float humidity);

DHT_Unified dht_sensor(DHT_SENSOR_PIN, DHTTYPE);
LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  lcd.begin(16, 2);
  dht_sensor.begin();
  lcd.setCursor(0, 0);
  lcd.print("Measuring...");
  Serial.println("OK: System ready");
}

void loop() {
  int currentButtonState = digitalRead(BUTTON_PIN);
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    if((millis() - lastDebounceTime) > DEBOUNCE_DELAY_MS) {
        lastDebounceTime = millis();
        displayMode = (displayMode == DISPLAY_MODE_TEMPERATURE)
                    ? DISPLAY_MODE_HUMIDITY
                    : DISPLAY_MODE_TEMPERATURE;
        updateDisplay(currentTemp, currentHumidity); 
    } 
  }
  lastButtonState = currentButtonState;

  if ((millis() - measurementTimestamp) >= 3000UL) {
    measurementTimestamp = millis();
    if (measure_environment(&currentTemp, &currentHumidity)) {
      updateDisplay(currentTemp, currentHumidity);
    }
  }
}

static void updateDisplay(float currentTemp, float currentHumidity) {
  lcd.setCursor(0, 1);
  lcd.print("                "); 
  lcd.setCursor(0, 1);

  if (displayMode == DISPLAY_MODE_TEMPERATURE) {
    float fahrenheit = (currentTemp * 9.0 / 5.0) + 32.0;
    lcd.setCursor(0, 0);
    lcd.print("Temperature:");
    lcd.setCursor(0, 1);
    lcd.print(fahrenheit);
    lcd.print(" F");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Humidity:");
    lcd.setCursor(0, 1);
    lcd.print(currentHumidity);
    lcd.print(" % RH");
  }
}

static bool measure_environment(float *temperature, float *humidity) {
  sensors_event_t tempEvent;
  sensors_event_t humidityEvent;

  dht_sensor.temperature().getEvent(&tempEvent);
  dht_sensor.humidity().getEvent(&humidityEvent);

  if (isnan(tempEvent.temperature) || isnan(humidityEvent.relative_humidity)) {
    Serial.println("WARN: DHT11 read failed");
    return false;
  }

  *temperature = tempEvent.temperature;
  *humidity    = humidityEvent.relative_humidity;
  return true;
}