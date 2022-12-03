#include "WiFi.h"
#include "HTTPClient.h"

// Board: DOIT ESP32 DEVKIT V1
// FATAL ERROR OCCURRED? - Wrong boot mode? --> Press boot button for ~2sec while it says "connecting....."

const char* ssid = ""; // Name of Wifi
const char* password = ""; // Password of Wifi

String HOST_NAME = ""; // IP des Geräts mit der Datenbank im lokalen Netzwerk
String PATH_NAME   = "/wetter/insert_data.php";
String queryString = "?temp=23.2&humidity=54.3&co2=12.3";

void setup() {
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    HTTPClient http;

    http.begin(HOST_NAME + PATH_NAME + queryString);
    int httpCode = http.GET();

    if(httpCode > 0) {
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

void loop() {}