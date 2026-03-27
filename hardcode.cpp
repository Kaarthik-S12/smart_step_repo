#include <Wire.h>
#include <MPU6050.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
MPU6050 mpu;
const char* ssid = "OnePlus Nord CE 3 Lite 5G";
const char* password = "n2mppcda";
const char* serverURL = "http://192.168.94.192:8080/alerts";


#define REST_THRESHOLD 0.00100
#define CHECK_INTERVAL 1000
#define WIFI_SCAN_INTERVAL 10000


#define TOTAL_REST_ADDR 0
#define TOTAL_UPTIME_ADDR 8
#define SYSTEM_START_ADDR 16


unsigned long systemStartTime;
unsigned long totalRestTime = 0;
unsigned long totalUptime = 0;
unsigned long lastCheckTime = 0;
unsigned long lastWiFiScanTime = 0;

unsigned long re=0;
float lastX = 0;
float lastY = 0;
float lastZ = 0;
bool isResting = false;
bool dataSent = false;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    return;
  }
  EEPROM.get(TOTAL_REST_ADDR, totalRestTime);
  EEPROM.get(TOTAL_UPTIME_ADDR, totalUptime);
  EEPROM.get(SYSTEM_START_ADDR, systemStartTime);

  systemStartTime = millis();
  EEPROM.put(SYSTEM_START_ADDR, systemStartTime);
  EEPROM.commit();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {
  unsigned long currentTime = millis();
  totalUptime = currentTime - systemStartTime;


  if (currentTime - lastCheckTime >= CHECK_INTERVAL) {
    lastCheckTime = currentTime;

    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);


     Serial.print("Raw accel: ");
    Serial.print(ax); Serial.print(", ");
    Serial.print(ay); Serial.print(", ");
    Serial.println(az);

    float x = ax / 16384.0;
    float y = ay / 16384.0;
    float z = az / 16384.0;


    float dx = x - lastX;
    float dy = y - lastY;
    float dz = z - lastZ;


    lastX = x;
    lastY = y;
    lastZ = z;


    double movement = sqrt(dx*dx + dy*dy + dz*dz);


    Serial.print("Movement calc: dx=");
    Serial.print(dx, 6);
    Serial.print(" dy=");
    Serial.print(dy, 6);
    Serial.print(" dz=");
    Serial.print(dz, 6);
    Serial.print(" -> movement=");
    Serial.println(movement, 6);
    if (movement <= REST_THRESHOLD ) {
      if (!isResting) {
        isResting = true;
        Serial.println("Rest started");
      } else {
        totalRestTime += CHECK_INTERVAL;
        EEPROM.put(TOTAL_REST_ADDR, totalRestTime);
        EEPROM.commit();
        int n = WiFi.scanNetworks();
        Serial.println(n);
        Serial.println("Rest ongoing. Total rest time (ms): " + String(totalRestTime));
      }
    } else {
      if (isResting) {
        isResting = false;
        Serial.println("Rest ended");
      }
    }
  }

  if (!dataSent && currentTime - lastWiFiScanTime >= WIFI_SCAN_INTERVAL) {
    lastWiFiScanTime = currentTime;
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
      if (WiFi.SSID(i) == ssid && WiFi.RSSI(i) >= -50) {
        Serial.println("Nearby Wi-Fi detected with strong signal, attempting connection...");
        WiFi.begin(ssid, password);
        unsigned long startAttempt = millis();
       while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 5000) {
          delay(500);
          Serial.print(".");
          if (isResting) {
             totalRestTime += CHECK_INTERVAL;
             EEPROM.put(TOTAL_REST_ADDR, totalRestTime);
             EEPROM.commit();
          }
        }
        if (WiFi.status() == WL_CONNECTED) {
          Serial.println("\nConnected to Wi-Fi");
          Serial.print("IP Address: ");
          Serial.println(WiFi.localIP());
          sendDataToServer();
          dataSent = true;
          Serial.println("Program terminating...");
            while (true) {
            delay(1000);
    }
        } else {
          Serial.println("\nConnection failed");
        }
        break;
      }
    }
  }
}

void sendDataToServer() {
  totalUptime = millis() - systemStartTime;

  HTTPClient http;
  WiFiClient client;

  String jsonData = "{\"restingHours\":" + String((totalRestTime  / 1000)*2) +
                    ",\"sensingHours\":" + String(totalUptime / 1000) + "}";
  Serial.println("Sending data: " + jsonData);

  http.begin(client, serverURL);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(jsonData);
  if (httpCode > 0) {
    Serial.println("Data sent successfully. HTTP code: " + String(httpCode));
    EEPROM.put(TOTAL_REST_ADDR, 0UL);
    EEPROM.put(TOTAL_UPTIME_ADDR, 0UL);
    EEPROM.put(SYSTEM_START_ADDR, 0UL);
    EEPROM.commit();
    Serial.println("EEPROM cleared after send.");
  } else {
    Serial.println("Failed to send data. HTTP error code: " + String(httpCode));
  }

  http.end();
}