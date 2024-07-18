#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <esp_wifi.h>

// IoT Cloud Config
const char DEVICE_LOGIN_NAME[] = "21bdb08f-d8fb-4784-abb4-d85a05fa67eb";
const char SSID[] = "lele bhikari"; 
const char PASS[] = "";
const char DEVICE_KEY[] = "B2WUQU9S3TFRZ5EEWBYY";

unsigned long lastUpdateTime = 0;

CloudTemperatureSensor temperature; // Declare the properties
CloudRelativeHumidity humidity;
CloudVolume volume;

void initProperties() {
    ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
    ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
    ArduinoCloud.addProperty(temperature, READ, ON_CHANGE, NULL);
ArduinoCloud.addProperty(humidity, READ, ON_CHANGE, NULL);



  ArduinoCloud.addProperty(volume, READ, ON_CHANGE, NULL);

}
WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

// Your original variables and configurations

//const char* ssid = "lele bhikari";
//const char* password = "";
String URL = "http://api.openweathermap.org/data/2.5/weather?";
String ApiKey = "337b08c71b0f0ab278a297dfd21d8706";
String lat = "13.352657352678051";
String lon = "74.79279020882488";

void setup() {
    Serial.begin(115200);
    initProperties();

    // Connect to Arduino IoT Cloud
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);

   


    // Setup LCD with backlight and initialize
    

    // Connect to WiFi
    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (WiFi.status() == WL_CONNECT_FAILED) {
            Serial.println("Connection Failed! Please check credentials.");
            break;
        }
        if (WiFi.status() == WL_NO_SSID_AVAIL) {
            Serial.println("SSID not available.");
            break;
        }
    }
    delay(5000);  // Wait for 5 seconds

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected.");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Failed to connect after waiting.");
    }

    setDebugMessageLevel(2);
    ArduinoCloud.printDebugInfo();
}




void loop() {
    // Maintain IoT Cloud connection
    ArduinoCloud.update();

    // Check for WiFi connection
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        // Set HTTP Request Final URL
        http.begin(URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKey);

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

////////
            temperature = temperature + 0.001;
            humidity = humidity + 0.001;
////////
            // Print on LCD
            

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
    }

   
    
    // Wait for 30 seconds before next iteration
    delay(60000);
}
