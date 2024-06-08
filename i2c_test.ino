#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Define the pin where the DHT sensor is connected
#define DHTPIN 4
// Define the type of DHT sensor (DHT11 or DHT22)
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Initialize the LCD
  lcd.begin(16, 2, LCD_5x8DOTS);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temp & Humidity");
}

void loop() {
  // Wait a few seconds between measurements
  delay(2000);

  // Read humidity and temperature
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the results to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  // Display the results on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");
  
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print(" %");
}
