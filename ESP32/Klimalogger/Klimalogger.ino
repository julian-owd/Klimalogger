#include "WiFi.h"
#include "HTTPClient.h"
#include "DHT.h"
#include "LiquidCrystal_I2C.h" 

#define DHTPIN 18
#define CO2PIN 19
#define DHTTYPE DHT22
#define HOST "http://192.168.178.44:80/wetter/data/insert_data.php" // ip of webserver
#define ROOMID 1
#define ACCESSKEY "m2nZGW1S"

// WiFi Settings
struct WiFiSettings {
  char ssid[50] = "FrettBox";
  char password[64] = "42327947420018450426";
};

struct LEDs {
  int green;
  int yellow;
  int red;
};

// LED pins
struct LEDs tempLEDs = {32, 4, 2};
struct LEDs humLEDs = {26, 25, 33};
struct LEDs co2LEDs = {12, 14, 27};

LiquidCrystal_I2C lcd(0x27, 16, 2); 
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // define pins
  pinMode(CO2PIN, INPUT);
  pinMode(tempLEDs.red,OUTPUT);
  pinMode(tempLEDs.yellow,OUTPUT);
  pinMode(tempLEDs.green,OUTPUT);
  pinMode(humLEDs.red,OUTPUT);
  pinMode(humLEDs.yellow,OUTPUT);
  pinMode(humLEDs.green,OUTPUT);
  pinMode(co2LEDs.red,OUTPUT);
  pinMode(co2LEDs.yellow,OUTPUT);
  pinMode(co2LEDs.green,OUTPUT);

  lcd.init();
  lcd.backlight();

  // connect to wifi
  WiFiSettings wiFiSettings;
  WiFi.begin(wiFiSettings.ssid, wiFiSettings.password);
  Serial.print("\nConnecting..\n");

  if (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(0, 1);
    lcd.print("WiFi: Connecting");
    delay(2000);
    lcd.clear();
  }

  dht.begin();

  // led test
  digitalWrite(tempLEDs.yellow, HIGH);
  digitalWrite(humLEDs.yellow, HIGH);
  digitalWrite(co2LEDs.yellow, HIGH);
  digitalWrite(tempLEDs.green, HIGH);
  digitalWrite(humLEDs.green, HIGH);
  digitalWrite(co2LEDs.green, HIGH);
  digitalWrite(tempLEDs.red, HIGH);
  digitalWrite(humLEDs.red, HIGH);
  digitalWrite(co2LEDs.red, HIGH);
}

void loop() {
  delay(100);
  
  // sum sensor values
  float humidity = 0;
  float temperature = 0;
  float co2 = 0;

  int values = 0; // amount of tracked values to calculate the average of each
  int displayStatus = 0; // status of display to switch content

  for (int i = 0; i < 12; i++) { // 60/12 = 5; new value each 5 seconds
    // Temp & Humidity
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // CO2
    unsigned long pwmtime = pulseIn(CO2PIN, HIGH, 2000000) / 1000;
    float pulsepercent = pwmtime / 1004.0;
    int c = 5000 * pulsepercent; // 5000 = ppm range
      
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fehler");
      lcd.setCursor(0, 1);
      lcd.print("DHT-Sensor");        
      continue;
    }

    if (isnan(c) || c == 0) {
      Serial.println("Failed to read CO2 data!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fehler");
      lcd.setCursor(0, 1);
      lcd.print("CO2-Sensor");       
      continue;
    }

    humidity += h;
    temperature += t;
    co2 += c;

    values++;

    lcd.clear();

    // update display, bottom line is different if wifi is not connected
    if (WiFi.isConnected()) {
      if (displayStatus == 0) {
        lcd.setCursor(0, 0);
        lcd.print("Temperatur: "+ String(temperature / values,1));
        lcd.setCursor(0, 1);
        lcd.print("Luftfeuchte: "+String(humidity / values,0));
        displayStatus = 1;
      } else if (displayStatus == 1) {
        lcd.setCursor(0, 0);
        lcd.print("Luftfeuchte: "+String(humidity / values,0));
        lcd.setCursor(0, 1);
        lcd.print("CO2-Gehalt: "+String(co2 / values,0));
        displayStatus = 2;          
      } else {
        lcd.setCursor(0, 0);
        lcd.print("CO2-Gehalt: "+String(co2 / values,0));
        lcd.setCursor(0, 1);
        lcd.print("Temperatur: "+ String(temperature / values,1));
        displayStatus = 0;
      }
    } else {
      if (displayStatus == 0) {
        lcd.setCursor(0, 0);
        lcd.print("Temperatur: "+ String(temperature / values,1));
        lcd.setCursor(0, 1);
        lcd.print("WiFi: Connecting");
        displayStatus = 1;
      } else if (displayStatus == 1) {
        lcd.setCursor(0, 0);
        lcd.print("Luftfeuchte: "+String(humidity / values,0));
        lcd.setCursor(0, 1);
        lcd.print("WiFi: Connecting");
        displayStatus = 2;          
      } else {
        lcd.setCursor(0, 0);
        lcd.print("CO2-Gehalt: "+String(co2 / values,0));
        lcd.setCursor(0, 1);
        lcd.print("WiFi: Connecting");
        displayStatus = 0;
      }
    }
    
    delay(5000);
  }

  humidity /= values;
  temperature /= values;
  co2 /= values;

  if (humidity == 0 && temperature == 0 && co2 == 0) {
    return;
  }

  // switching leds
  if (temperature >= 20 && temperature <= 23) {
    digitalWrite(tempLEDs.red, LOW);
    digitalWrite(tempLEDs.yellow, LOW);
    digitalWrite(tempLEDs.green, HIGH);
  }
  if (temperature >= 16 && temperature <= 19 || temperature >= 24 && temperature <= 26) {
    digitalWrite(tempLEDs.red, LOW);
    digitalWrite(tempLEDs.yellow, HIGH);
    digitalWrite(tempLEDs.green, LOW);
  }
  if (temperature < 16 || temperature > 26) {
    digitalWrite(tempLEDs.red, HIGH);
    digitalWrite(tempLEDs.yellow, LOW);
    digitalWrite(tempLEDs.green, LOW);
  }

  if (co2 < 1000) {
    digitalWrite(co2LEDs.red, LOW);
    digitalWrite(co2LEDs.yellow, LOW);
    digitalWrite(co2LEDs.green, HIGH);
  }
  if (co2 >= 1000 && co2 <= 2000) {
    digitalWrite(co2LEDs.red, LOW);
    digitalWrite(co2LEDs.yellow, HIGH);
    digitalWrite(co2LEDs.green, LOW);
  }
  if (co2 > 2000) {
    digitalWrite(co2LEDs.red, HIGH);
    digitalWrite(co2LEDs.yellow, LOW);
    digitalWrite(co2LEDs.green, LOW);
  }

  if (humidity >= 40 && humidity <= 60) {
    digitalWrite(humLEDs.red, LOW);
    digitalWrite(humLEDs.yellow, LOW);
    digitalWrite(humLEDs.green, HIGH);
  }
  if (humidity > 60  && humidity < 70 || humidity > 30 && humidity < 40) {
    digitalWrite(humLEDs.red, LOW);
    digitalWrite(humLEDs.yellow, HIGH);
    digitalWrite(humLEDs.green, LOW);
  }
  if (humidity <= 30 || humidity >= 70) {
    digitalWrite(humLEDs.red, HIGH);
    digitalWrite(humLEDs.yellow, LOW);
    digitalWrite(humLEDs.green, LOW);
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

  // try to reconnect to wifi, if connection got lost
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
    Serial.println("Connection to WiFi lost");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("WiFi: Connecting");
    delay(2000);
  }

  // send data to webserver
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String queryString = "?r=" + String(ROOMID) + "&t=" + String(temperature) + "&h=" + String(humidity) + "&c=" + String(co2) + "&accessKey=" + String(ACCESSKEY);
    http.begin(HOST + queryString);
    int httpCode = http.GET();

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      } else {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Web-Verbindung");
        lcd.setCursor(0, 1);
        lcd.print("fehlgeschlagen");
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Web-Verbindung");
      lcd.setCursor(0, 1);
      lcd.print("fehlgeschlagen");
    }
    http.end();
    delay(1000);
  }
}