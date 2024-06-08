#include <WiFi.h>
#include <ESP32_MailClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Email credentials
#define SMTP_HOST "smtp.your-email.com"
#define SMTP_PORT 587
#define AUTHOR_EMAIL "your_email@domain.com"
#define AUTHOR_PASSWORD "your_email_password"
#define RECIPIENT_EMAIL "recipient_email@domain.com"

// Define the pin where the DHT sensor is connected
#define DHTPIN 4
// Define the type of DHT sensor (DHT11 or DHT22)
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust columns and rows here if needed

// Define extreme condition thresholds
const float TEMP_THRESHOLD = 30.0; // Celsius
const float HUM_THRESHOLD = 80.0; // Percentage

// Email configuration
SMTPData smtpData;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Initialize the LCD
  lcd.begin(16, 2, LCD_5x8DOTS);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temp & Humidity");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Configure SMTP server
  smtpData.setLogin(SMTP_HOST, SMTP_PORT, AUTHOR_EMAIL, AUTHOR_PASSWORD);
  smtpData.setSender("ESP32 Alert", AUTHOR_EMAIL);
  smtpData.setPriority("High");
  smtpData.setSubject("Temperature/Humidity Alert");
  smtpData.addRecipient(RECIPIENT_EMAIL);
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

  // Check for extreme conditions and send email alerts
  if (temperature > TEMP_THRESHOLD || humidity > HUM_THRESHOLD) {
    String message = "Warning! Extreme conditions detected:\n";
    if (temperature > TEMP_THRESHOLD) {
      message += "Temperature: " + String(temperature) + " C\n";
    }
    if (humidity > HUM_THRESHOLD) {
      message += "Humidity: " + String(humidity) + " %";
    }

    smtpData.setMessage(message.c_str(), true);
    
    if (!MailClient.sendMail(smtpData)) {
      Serial.println("Error sending email: " + MailClient.smtpErrorReason());
    }
    
    smtpData.empty();
  }
}
