#include <DHT.h>

// --- Pin Definitions ---
#define POT_PIN   34
#define LDR_PIN   35
#define LED_PIN   4
#define DHT_PIN   32 
#define DHT_TYPE  DHT11

DHT dht(DHT_PIN, DHT_TYPE);

// --- PWM Config ---
#define PWM_FREQ  5000
#define PWM_RES   8

// --- LDR Threshold ---
#define LDR_DARK_THRESHOLD 3500

void setup() {
  Serial.begin(115200);

  // PWM setup (ESP32 correct usage)
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RES);

  dht.begin();
}

void loop() {

  // --- Potentiometer ---
  int potVal = analogRead(POT_PIN);
  int brightness = map(potVal, 0, 4095, 0, 255);

  // ensure LED is never completely dead when active
  if (brightness > 0 && brightness < 30) {
    brightness = 30;
  }

  // --- LDR ---
  int ldrVal = analogRead(LDR_PIN);

if (ldrVal > LDR_DARK_THRESHOLD) {
    ledcWrite(LED_PIN, brightness);
  } else {
    ledcWrite(LED_PIN, 0);
  }

  // --- DHT11 Read ---
  float tempC = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(tempC) || isnan(humidity)) {
    Serial.println("DHT11 read failed!");
    delay(1000);
    return;
  }

  // --- Serial Monitor ---
  Serial.print("Pot: "); Serial.print(potVal);
  Serial.print(" | Brightness: "); Serial.print(brightness);
  Serial.print(" | LDR: "); Serial.print(ldrVal);
  Serial.print(" | Temp: "); Serial.print(tempC);
  Serial.print(" °C | Humidity: "); Serial.print(humidity);
  Serial.println(" %");

  delay(1000);
}