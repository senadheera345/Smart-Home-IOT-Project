#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPin 2

DHT dht(DHTPin, DHTTYPE);

// ESP8266 connects to router
const char* ssid = "Dialog 4G 235";
const char* password = "thimiraj1";

// MQTT broker credentials 
const char* MQTT_username = NULL; 
const char* MQTT_password = NULL;

// MQTT broker IP address
const char* mqtt_server = "broker.hivemq.com";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long previousMillis = 0;

// connects ESP8266 to router
void setup_wifi() {
  delay(10);
  // connecting to a WiFi network
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
  // Loop until reconnected
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
    // Read temperature as Celsius 
    float temperatureC = dht.readTemperature();
    // Read temperature as Fahrenheit 
    float temperatureF = dht.readTemperature(true);

    // Check if any reads failed and exit early 
    if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

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
  }
}

float calculate_heat_index(float T, float RH) {
  return -42.379 + 2.04901523 * T + 10.14333127 * RH - 0.22475541 * T * RH - 0.00683783 * T * T - 0.05481717 * RH * RH + 0.00122874 * T * T * RH + 0.00085282 * T * RH * RH - 0.00000199 * T * T * RH * RH;
}


