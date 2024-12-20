#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Servo.h>

#define DHTTYPE DHT11
#define DHTPin 2

DHT dht(DHTPin, DHTTYPE);

// ESP8266 connects to router
const char* ssid = "Yusri";
const char* password = "Yusri173";

// MQTT broker credentials 
const char* MQTT_username = NULL; 
const char* MQTT_password = NULL;

//MQTT broker IP address
const char* mqtt_server = "broker.hivemq.com";

// Initializes the espClient.
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long previousMillis = 0;

// Define servo motor parameters
#define SERVO_PIN D3 // Change to the pin connected to the servo
#define SERVO_CLOSED_ANGLE 0
#define SERVO_OPEN_ANGLE 180
#define SERVO_STEP 30 // Angle step for each stage

// Define Heat Index classification thresholds
#define NORMAL_THRESHOLD 80
#define CAUTION_THRESHOLD 90
#define EXTREME_CAUTION_THRESHOLD 105
#define DANGER_THRESHOLD 130
#define EXTREME_DANGER_THRESHOLD 999 // Set to a value above any possible Heat Index

Servo servoMotor;

// connects ESP8266 to router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("NodeMCUClient", MQTT_username, MQTT_password)) {
      Serial.println("connected");  
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  dht.begin();
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // Attach servo motor to pin
  servoMotor.attach(SERVO_PIN);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("NodeMCUClient", MQTT_username, MQTT_password);

  unsigned long currentMillis = millis();
  // Publishes new temperature and humidity every 10 seconds
  if (currentMillis - previousMillis > 10000) {
    previousMillis = currentMillis;

    float humidity = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float temperatureC = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float temperatureF = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    // Publish temperature and humidity
    client.publish("iotfrontier/temperatureF", String(temperatureF).c_str());
    client.publish("iotfrontier/temperature", String(temperatureC).c_str());

    client.publish("iotfrontier/humidity", String(humidity).c_str());
    // Calculate Heat Index
    float heatIndex = calculate_heat_index(temperatureF, humidity);
    
    // Publish Heat Index value
    client.publish("iotfrontier/heat_index", String(heatIndex).c_str());
    
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" ºC");
    Serial.print(temperatureF);
    Serial.println(" ºF");
    Serial.print("Heat Index: ");
    Serial.println(heatIndex);

    //180 degrees is closed 
    //0 degrees is fully open 
    // Control servo motor based on Heat Index classification
    if (heatIndex < NORMAL_THRESHOLD) {
        servoMotor.write(180);//closed
        delay(1000);

    } else if (heatIndex >= CAUTION_THRESHOLD) {
        servoMotor.write(180);//closed 
        delay(1000);
        servoMotor.write(0);// fully opened
        delay(1000);
    } else if (heatIndex >= NORMAL_THRESHOLD) {
        servoMotor.write(180);// closed
        delay(1000);
        servoMotor.write(90); //partially opened
        delay(1000);
    } else {
        // If heat index is below normal thresholds, do nothing or handle as needed
    }

  }
}

float calculate_heat_index(float T, float RH) {
  return -42.379 + 2.04901523 * T + 10.14333127 * RH - 0.22475541 * T * RH - 0.00683783 * T * T - 0.05481717 * RH * RH + 0.00122874 * T * T * RH + 0.00085282 * T * RH * RH - 0.00000199 * T * T * RH * RH;
}

void moveServo(int angle) {
  servoMotor.write(angle);
  delay(500); 
}