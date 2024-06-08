#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

// Define the pin where the DHT sensor is connected
#define DHTPIN 4
// Define the type of DHT sensor (DHT11 or DHT22)
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define extreme condition thresholds
const float TEMP_THRESHOLD = 30.0; // Celsius
const float HUM_THRESHOLD = 80.0; // Percentage

// GSM configuration
#define TINY_GSM_MODEM_SIM800
#define GSM_TX_PIN 17
#define GSM_RX_PIN 16

const char apn[] = ""; // APN not needed for SMS
const char user[] = "";
const char pass[] = "";
const char smsRecipient[] = "+1234567890"; // Replace with your phone number

TinyGsm modem(SerialAT);
SoftwareSerial SerialAT(GSM_TX_PIN, GSM_RX_PIN); // RX, TX

void setup() {
  Serial.begin(115200);
  SerialAT.begin(9600);
  dht.begin();
  
  // Initialize the LCD
  lcd.begin(16, 2, LCD_5x8DOTS);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temp & Humidity");

  // Initialize GSM module
  Serial.println("Initializing modem...");
  modem.restart();

  // Check if the modem is connected properly
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem Info: ");
  Serial.println(modemInfo);

  // Set modem to SMS mode
  modem.sendAT(GF("+CMGF=1"));
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

  // Check for extreme conditions and send SMS alerts
  if (temperature > TEMP_THRESHOLD || humidity > HUM_THRESHOLD) {
    String message = "Warning! Extreme conditions detected:\n";
    if (temperature > TEMP_THRESHOLD) {
      message += "Temperature: " + String(temperature) + " C\n";
    }
    if (humidity > HUM_THRESHOLD) {
      message += "Humidity: " + String(humidity) + " %";
    }

    // Send SMS
    sendSMS(message);
  }
}

void sendSMS(String message) {
  Serial.println("Sending SMS...");
  modem.sendSMS(smsRecipient, message);
  Serial.println("SMS sent successfully");
}
