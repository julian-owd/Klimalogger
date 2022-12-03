#include "WiFi.h"
#include "HTTPClient.h"
#include "DHT.h"

// Board: DOIT ESP32 DEVKIT V1
// FATAL ERROR OCCURRED? - Wrong boot mode? --> Press boot button for ~2sec while it says "connecting....."

#define DHTPIN 21
#define DHTTYPE DHT22

const char* ssid = "FrettBox"; // Name of Wifi
const char* password = "42327947420018450426"; // Password of Wifi
String HOST = "http://192.168.178.44:80/wetter/insert_data.php"; // IP des Geräts mit der Datenbank im lokalen Netzwerk
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    delay(1000);
    pinMode(2, OUTPUT);

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.print("\nConnecting");
    digitalWrite(2, HIGH);

    while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
        delay(500);
    }

    digitalWrite(2, LOW);
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    dht.begin();
}

void loop() {
    delay(100);
    float humidity = 0;
    float temperature = 0;
    int values = 0;

    for (int i = 0; i < 30; i++) {
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
      if (isnan(h) || isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        continue;
      }

      humidity += h;
      temperature += t;
      values++;
      delay(2000);
    }

    humidity /= values;
    temperature /= values;

    if (humidity == 0 && temperature == 0) {
      return;
    }

    Serial.println("-------------------");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%  Temperature: ");
    Serial.print(temperature);
    Serial.print(" Grad C");
    Serial.println("");
    Serial.println("-------------------");

    while (WiFi.status() != WL_CONNECTED) {
      WiFi.reconnect();
      Serial.println("Connection to WiFi lost");
      digitalWrite(2, HIGH);
      delay(2000);
    }

    digitalWrite(2, LOW);

    HTTPClient http;
    String queryString = "?temp=" + String(temperature) + "&humidity=" + String(humidity) + "&co2=0";
    http.begin(HOST + queryString);
    int httpCode = http.GET();

    if (httpCode > 0) {
      // file found at server
      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      } else {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
}