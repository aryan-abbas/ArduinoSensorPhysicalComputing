#include "DHT.h"

#define LIGHT_SENSOR_PIN A3   // Grove light sensor
#define DHT1_PIN 2            // First DHT11 sensor
#define DHT2_PIN 3            // Second DHT11 sensor
#define DHTTYPE DHT11         // Sensor type

// DHT sensor objects
DHT dht1(DHT1_PIN, DHTTYPE);
DHT dht2(DHT2_PIN, DHTTYPE);

struct MessageValue {
  String message;
  String value;
};

void sendMessage(struct MessageValue message) {
  Serial.print(message.message);
  Serial.print("||");
  Serial.print(message.value);
  Serial.println();
  delay(30);
}

struct MessageValue getMessage(String inputtedStr) {
  struct MessageValue result;
  char charArr[50];
  inputtedStr.toCharArray(charArr, 50);
  char* ptr = strtok(charArr, "||");
  result.message = String(ptr);
  ptr = strtok(NULL, "||");
  result.value = (ptr == NULL) ? String("") : String(ptr);
  return result;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Light + DHT11 sensors");
  dht1.begin();
  dht2.begin();
  Serial.setTimeout(10);
}

void loop() {
  struct MessageValue receivedData;

  while (Serial.available() > 0) {
    String receivedString = Serial.readStringUntil('\0');
    receivedData = getMessage(receivedString);
  }

  if (receivedData.message.equals("power")) {
  }

  int lightValue = analogRead(LIGHT_SENSOR_PIN);

  // Read DHT11 #1
  float temp1 = dht1.readTemperature();
  float hum1 = dht1.readHumidity();

  // Read DHT11 #2
  float temp2 = dht2.readTemperature();
  float hum2 = dht2.readHumidity();

  // Send light value
  struct MessageValue light;
  light.message = "light";
  light.value = String(lightValue);
  sendMessage(light);

  // Send DHT11 #1 readings
  struct MessageValue tempMsg1;
  tempMsg1.message = "temp1";
  tempMsg1.value = isnan(temp1) ? "NaN" : String(temp1);
  sendMessage(tempMsg1);

  struct MessageValue humMsg1;
  humMsg1.message = "humidity1";
  humMsg1.value = isnan(hum1) ? "NaN" : String(hum1);
  sendMessage(humMsg1);

  // Send DHT11 #2 readings
  struct MessageValue tempMsg2;
  tempMsg2.message = "temp2";
  tempMsg2.value = isnan(temp2) ? "NaN" : String(temp2);
  sendMessage(tempMsg2);

  struct MessageValue humMsg2;
  humMsg2.message = "humidity2";
  humMsg2.value = isnan(hum2) ? "NaN" : String(hum2);
  sendMessage(humMsg2);

  delay(1000);
}
