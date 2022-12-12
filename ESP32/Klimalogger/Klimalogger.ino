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

int ledRotTemp = 34;
int ledRotHum = 33;
int ledRotCO2 = 27;

int ledGelbTemp = 35;
int ledGelbHum = 25;
int ledGelbCO2 = 14;

int ledGruenTemp = 32;
int ledGruenHum = 26;
int ledGruenCO2 = 12;

const char* ssid = "JulianHotspot"; // Name of Wifi
const char* password = "12345678neun"; // Password of Wifi
String HOST = "http://192.168.219.177:80/wetter/insert_data.php"; // IP des Geräts mit der Datenbank im lokalen Netzwerk
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(2, OUTPUT); // ONBOARD LED
    pinMode(CO2PIN, INPUT);

    pinMode(ledRotTemp,OUTPUT);
    pinMode(ledGelbTemp,OUTPUT);
    pinMode(ledGruenTemp,OUTPUT);
    pinMode(ledRotHum,OUTPUT);
    pinMode(ledGelbHum,OUTPUT);
    pinMode(ledGruenHum,OUTPUT);
    pinMode(ledRotCO2,OUTPUT);
    pinMode(ledGelbCO2,OUTPUT);
    pinMode(ledGruenCO2,OUTPUT);


    lcd.init();
    // turn on LCD backlight                      
    lcd.backlight();

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.print("\nConnecting");
    digitalWrite(2, HIGH);

    while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      lcd.setCursor(0, 0);
      lcd.print("WLAN Verbindung");
      lcd.setCursor(0, 1);
      lcd.print("wird hergestellt");
      delay(500);
      lcd.clear();
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

      lcd.setCursor(0,0);
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

    digitalWrite(ledRotTemp, LOW);
    digitalWrite(ledGelbTemp, LOW);
    digitalWrite(ledGruenTemp, LOW);
    digitalWrite(ledRotHum, LOW);
    digitalWrite(ledGelbHum, LOW);
    digitalWrite(ledGruenHum, LOW);
    digitalWrite(ledRotCO2, LOW);
    digitalWrite(ledGelbCO2, LOW);
    digitalWrite(ledGruenCO2, LOW);


//TEMP
    if(temperature >= 20 && temperature <= 23){
      digitalWrite(ledRotTemp, LOW);
      digitalWrite(ledGelbTemp, LOW);
      digitalWrite(ledGruenTemp, HIGH);
    }
    if(temperature >= 16 && temperature <= 19 || temperature >= 24 && temperature <= 26){
      digitalWrite(ledRotTemp, LOW);
      digitalWrite(ledGelbTemp, HIGH);
      digitalWrite(ledGruenTemp, LOW);
    }
    if(temperature < 16 || temperature > 26){
      digitalWrite(ledRotTemp, HIGH);
      digitalWrite(ledGelbTemp, LOW);
      digitalWrite(ledGruenTemp, LOW);
    }

//CO2
    if(co2 < 1000){
      digitalWrite(ledRotCO2, LOW);
      digitalWrite(ledGelbCO2, LOW);
      digitalWrite(ledGruenCO2, HIGH);
    }
    if(co2 >= 1000 && co2 <= 2000){
      digitalWrite(ledRotCO2, LOW);
      digitalWrite(ledGelbCO2, HIGH);
      digitalWrite(ledGruenCO2, LOW);
    }
    if(co2 > 2000){
      digitalWrite(ledRotCO2, HIGH);
      digitalWrite(ledGelbCO2, LOW);
      digitalWrite(ledGruenCO2, LOW);
    }

//HUMIDITY
    if( humidity >= 40 && humidity <= 60){
      digitalWrite(ledRotHum, LOW);
      digitalWrite(ledGelbHum, LOW);
      digitalWrite(ledGruenHum, HIGH);
    }
    if(humidity > 60  && humidity < 70 || humidity > 30 && humidity < 40){
      digitalWrite(ledRotHum, LOW);
      digitalWrite(ledGelbHum, HIGH);
      digitalWrite(ledGruenHum, LOW);
    }
    if(humidity <= 30 || humidity >= 70){
      digitalWrite(ledRotHum, HIGH);
      digitalWrite(ledGelbHum, LOW);
      digitalWrite(ledGruenHum, LOW);
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

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("WLAN Verbindung");
      lcd.setCursor(0, 1);
      lcd.print("wird hergestellt");

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
  
}