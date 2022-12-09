#include "WiFi.h"
#include "HTTPClient.h"
#include "DHT.h"
#include "LiquidCrystal_I2C.h"

// Board: DOIT ESP32 DEVKIT V1
// FATAL ERROR OCCURRED? - Wrong boot mode? --> Press boot button for ~2sec while it says "connecting....."

#define DHTPIN 18
#define CO2PIN 19
#define DHTTYPE DHT22

LiquidCrystal_I2C lcd(0x27, 16, 2);  

const char* ssid = "JulianHotspot"; // Name of Wifi
const char* password = "09022004"; // Password of Wifi
String HOST = "http://192.168.219.177:80/wetter/insert_data.php"; // IP des Geräts mit der Datenbank im lokalen Netzwerk
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    delay(1000);
    pinMode(2, OUTPUT); // ONBOARD LED
    pinMode(CO2PIN, INPUT);

    lcd.init();
    // turn on LCD backlight                      
    lcd.backlight();

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.print("\nConnecting");
    digitalWrite(2, HIGH);

    while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
        delay(500);
    }

    digitalWrite(2, LOW);
    Serial.print("\nConnected to the WiFi");
    Serial.print(ssid);
    Serial.print(" network\n");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    dht.begin();
}

void loop() {
    delay(100);
    // Temp & Humidity
    float humidity = 0;
    float temperature = 0;
    float co2 = 0;
    int values = 0;

    for (int i = 0; i < 30; i++) {
      float h = dht.readHumidity();
      float t = dht.readTemperature();

      // CO2
      unsigned long pwmtime = pulseIn(CO2PIN, HIGH, 2000000) / 1000;
      float pulsepercent = pwmtime / 1004.0;
      int c = 5000 * pulsepercent; // 5000 = ppm range
      
      if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        continue;
      }

      if (isnan(c) || c == 0) {
        Serial.println("Failed to read CO2 data!");
        continue;
      }

      humidity += h;
      temperature += t;
      co2 += c;

      values++;

      lcd.clear();

      lcd.print("temp: "+ String(temperature / values,0)); 
      lcd.print(" hum: "+String(humidity / values,0)); 
      lcd.setCursor(0,1);
      lcd.print("CO2: "+String(co2 / values,0));

      delay(2000);
    }

    humidity /= values;
    temperature /= values;
    co2 /= values;

    if (humidity == 0 && temperature == 0 && co2 == 0) {
      return;
    }
    
    Serial.println("-------------------");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%  Temperature: ");
    Serial.print(temperature);
    Serial.print(" Grad C CO2 Konzentration: ");
    Serial.print(co2);
    Serial.print(" ppm");
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
    String queryString = "?temp=" + String(temperature) + "&humidity=" + String(humidity) + "&co2=" + String(co2);
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