#include <DHT.h>
#include <LiquidCrystal.h>

// Define pin assignments
#define DHTPIN 12              // GPIO pin 12 connected to the DHT11 sensor Data pin D4
const int fanPin = 0;         // GPIO pin 0 connected to the Fan D3
const int heaterPin = 1;      // GPIO pin 1 connected to the Heater D2
const int soilMoisturePin = A0;  // Analog pin A0 connected to the Soil Moisture Sensor
const int trigPin = 2;        // GPIO pin 2 connected to the ultrasonic sensor trigger pin
const int echoPin = 3;        // GPIO pin 3 connected to the ultrasonic sensor echo pin
const int motorPin = 4;       // GPIO pin 4 connected to the motor

// Define DHT type
#define DHTTYPE DHT11

// Initialize DHT sensor object
DHT dht(DHTPIN, DHTTYPE);

// Initialize LCD object
const int rs = 5, en = 6, d4 = 7, d5 = 8, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define moisture levels
enum MoistureLevel {
  Saturated,
  Wet,
  Moist,
  Dry
};

// Define threshold values
const float highWaterLevel = 4.00; // High water level threshold
const float lowWaterLevel = 30.00; // Low water level threshold

void setup() {
  Serial.begin(9600);         // Initialize serial communication
  dht.begin();                // Initialize DHT sensor

  pinMode(fanPin, OUTPUT);    // Set fan pin as output
  pinMode(heaterPin, OUTPUT); // Set heater pin as output
  pinMode(trigPin, OUTPUT);   // Set ultrasonic sensor trigger pin as output
  pinMode(echoPin, INPUT);    // Set ultrasonic sensor echo pin as input
  pinMode(motorPin, OUTPUT);  // Set motor pin as output

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Soil Moisture:");
}

void loop() {
  delay(2000);  // Wait for 2 seconds between measurements

  // Read temperature and humidity from DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Read soil moisture level
  MoistureLevel soilMoistureLevel = readSoilMoisture();

  // Read water level
  float waterLevel = readWaterLevel();

  // Check if readings from DHT sensor are valid
  if (isnan(humidity) || isnan(temperature)) {
    lcd.clear();
    lcd.print("Failed to read DHT");
    Serial.println("Failed to read DHT");
    return;
  }

  // Control temperature and humidity based on readings
  temperature_Humidity_Control(humidity, temperature);

  // Control soil moisture based on readings
  soilMoisture_Control(soilMoistureLevel);

  // Control motor based on water level
  waterLevel_Control(waterLevel);

  // Print temperature and humidity readings to LCD and serial monitor
  lcd.clear();
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
  lcd.setCursor(12, 0);
  lcd.print(temperature);
  lcd.setCursor(10, 1);
  lcd.print(humidity);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Print soil moisture level to LCD and serial monitor
  delay(5000); // Delay to allow reading to be visible before clearing
  lcd.clear();
  lcd.print("Soil Moisture: ");
  lcd.setCursor(0,1);
  lcd.print(getSoilMoistureState(soilMoistureLevel));
  Serial.print("Soil Moisture: ");
  Serial.println(getSoilMoistureState(soilMoistureLevel));

  // Print water level to LCD and serial monitor
  lcd.clear();
  lcd.print("Water Level: ");
  lcd.setCursor(0,1);
  lcd.print(waterLevel);
  Serial.print("Water Level: ");
  Serial.println(waterLevel);
}

// Control temperature and humidity based on readings
void temperature_Humidity_Control(float H, float T) {
  // Temperature control
  /*
  if (T >= 24) {
    digitalWrite(fanPin, HIGH); // Turn on the fan 
  } else {
    digitalWrite(fanPin, LOW); // Turn off the fan
  }
  */

  /*// Humidity control
  if (H > 10) { // Adjust the humidity threshold as needed
    digitalWrite(heaterPin, HIGH); // Turn on the heater
  } else {
    digitalWrite(heaterPin, LOW); // Turn off the heater
  }
  */
}

// Read soil moisture level
MoistureLevel readSoilMoisture() {
  int sensorValue = analogRead(soilMoisturePin); // Read analog value from soil moisture sensor
  int mappedValue = map(sensorValue, 1024, 250, 0, 100); // Map analog value to percentage

  // Determine moisture level based on mapped value
  if (mappedValue >= 76) {
    return Saturated;
  } else if (mappedValue >= 51) {
    return Wet;
  } else if (mappedValue >= 26) {
    return Moist;
  } else {
    return Dry;
  }
}

// Control soil moisture based on moisture level reading
void soilMoisture_Control(MoistureLevel moistureLevel) {
  // You can add your soil moisture control logic here based on the moisture level reading
  // For example, you can activate an irrigation system or display a warning message.
  if (moistureLevel == Dry) {
    // Turn on the water pump
  } else if (moistureLevel == Moist) {
    // Turn off the water pump
  }
}

// Function to get soil moisture state
String getSoilMoistureState(MoistureLevel moistureLevel) {
  switch (moistureLevel) {
    case Saturated:
      return "Saturated";
    case Wet:
      return "Wet";
    case Moist:
      return "Moist";
    case Dry:
      return "Dry";
    default:
      return "";
  }
}

// Read water level
float readWaterLevel() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance = (duration * 0.0343) / 2;

  return distance;
}

// Control motor based on water level
void waterLevel_Control(float distance) {
  // Adjust threshold value as needed
  if (distance > lowWaterLevel) {
    // If water level is low, turn on the motor
    digitalWrite(motorPin, HIGH); // Assuming HIGH is the direction to turn on the motor
    Serial.println("Low Water");
  } 
  // Check if the water level is high
  else if (distance <= highWaterLevel) {
    // If water level is high, turn off the motor
    digitalWrite(motorPin, LOW);
    Serial.println("High Water");
  } 
}