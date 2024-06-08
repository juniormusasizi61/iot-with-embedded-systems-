https://github.com/mus-junior/iot-with-embedded-systems-.git#include "DHT.h"

// Define DHT sensor type and pin
#define DHTTYPE DHT22   // DHT 22 (AM2302), DHT11
#define DHTPIN  4       // Digital pin connected to the DHT sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float temperatureF = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature) || isnan(temperatureF)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float heatIndex = dht.computeHeatIndex(temperatureF, humidity);
  // Compute heat index in Celsius (isFahrenheit = false)
  float heatIndexC = dht.computeHeatIndex(temperature, humidity, false);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C ");
  Serial.print(temperatureF);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(heatIndex);
  Serial.print(" *F ");
  Serial.print(heatIndexC);
  Serial.println(" *C");
}
