// Arduino CO2 Sensor - MH-Z19 Beispiel und Sketch
// https://iotspace.dev/arduino-co2-sensor-mh-z19-beispiel-und-sketch

int DataPin = 19;


void setup() {
 pinMode(DataPin, INPUT);
 Serial.begin(115200);
}

void loop() {  
 unsigned long pwmtime = pulseIn(DataPin, HIGH, 2000000) / 1000;
 float pulsepercent = pwmtime / 1004.0;
 int PPM = 5000 * pulsepercent; // 5000 = ppm range
 Serial.print("CO2 Konzentration in der Luft in PPM: ");
 Serial.println(PPM);
 delay(5000);
}