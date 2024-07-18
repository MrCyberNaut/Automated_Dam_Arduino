#include <Arduino.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char DEVICE_LOGIN_NAME[] = "21bdb08f-d8fb-4784-abb4-d85a05fa67eb";
const char SSID[] = "lele bhikari";
const char PASS[] = "";
const char DEVICE_KEY[] = "B2WUQU9S3TFRZ5EEWBYY";

CloudTemperatureSensor temperature;
CloudRelativeHumidity humidity;
CloudTemperature waterLevel;
CloudVolume volume;

void initProperties() {
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(temperature, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(humidity, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(waterLevel, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(volume, READ, ON_CHANGE, NULL);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

String URL = "http://api.openweathermap.org/data/2.5/weather?";
String ApiKey = "337b08c71b0f0ab278a297dfd21d8706";
String lat = "13.352657352678051";
String lon = "74.79279020882488";

void setup() {
  Serial.begin(9600);
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Failed to connect after waiting.");
  }

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  waterLevel = 75;
}

void loop() {
  // Maintain IoT Cloud connection
  ArduinoCloud.update();

  // Check for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Set HTTP Request Final URL
    http.begin(URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKey);

    if (Serial.available() > 0) {
      String data = Serial.readStringUntil('\n');

      // Check if the received data contains both values
      int waterPos = data.indexOf("WaterLevel:");
      int volumePos = data.indexOf("Volume:");

      if (waterPos != -1 && volumePos != -1) {
        String waterStr = data.substring(waterPos + 11, volumePos);
        String volumeStr = data.substring(volumePos + 7);

        int waterLevelValue = waterStr.toInt();
        float volumeValue = volumeStr.toFloat();

        Serial.print("Received Water Level: ");
        Serial.println(waterLevelValue);
        Serial.print("Received Volume: ");
        Serial.println(volumeValue);

        // Update the cloud properties
        waterLevel = waterLevelValue;
        volume = volumeValue;
      }
    }

    int httpCode = http.GET();
    if (httpCode > 0) {
      String JSON_Data = http.getString();
      Serial.println(JSON_Data);

      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);
      JsonObject obj = doc.as<JsonObject>();

      // Update global variables
      temperature = obj["main"]["temp"].as<float>();
      humidity = obj["main"]["humidity"].as<float>();

      // Print on Serial Monitor
      Serial.print("Weather Description: ");
      Serial.println(obj["weather"][0]["description"].as<const char*>());
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");
      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");
    } else {
      Serial.println("Error!");
    }

    http.end();

    Serial.println("Values will be printed every 30 seconds");
    Serial.print("Water Level: ");
    Serial.println(waterLevel);
    Serial.print("Volume: ");
    Serial.println(volume);
     Serial.println("Message Received: ");
    Serial.println(Serial2.readString());
  }

  // Wait for 30 seconds before the next iteration
  delay(30000);
}
