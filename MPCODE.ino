#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <NewPing.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_TEMPLATE_ID "TMPL3TtpfP8fr"
#define BLYNK_TEMPLATE_NAME "Coal Mine Monitor"
#define BLYNK_AUTH_TOKEN "wy2x3OoRcfJ5TvM8-rHFkhrXXPvt2vRT"

#define DHTPIN 27
#define DHTTYPE DHT11
#define MAX_DISTANCE 200
DHT dht(DHTPIN, DHTTYPE);

char auth[] = "wy2x3OoRcfJ5TvM8-rHFkhrXXPvt2vRT";
char ssid[] = "SUNILBK";        
char pass[] = "sunilkiranbk";   

const int irSensorPin = 13;
const int buzzerPin = 14;
const int ledPin1 = 4;
const int ledPin2 = 2;
const int TRIGGER_PIN = 5;
const int ECHO_PIN = 18;
const int mq9Pin = 35;
const int ledPin3 = 15;
const int switchPin = 34;

LiquidCrystal_I2C lcd(0x27, 16, 2);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);


void setup() {
pinMode(irSensorPin, INPUT);
pinMode(buzzerPin, OUTPUT);
pinMode(ledPin1, OUTPUT);
pinMode(ledPin2, OUTPUT);
pinMode(TRIGGER_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);
pinMode(ledPin3, OUTPUT);
pinMode(switchPin, INPUT);

Serial.begin(115200);
Blynk.begin(auth, ssid, pass);
lcd.init();
lcd.backlight();
dht.begin();
}


void loop() {
int irSensorValue = digitalRead(irSensorPin);
float temp = dht.readTemperature();
int humidity = dht.readHumidity();
int mq9Value = analogRead(mq9Pin);
Blynk.run();

  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, mq9Value);


//IR SENSOR
if (irSensorValue == HIGH) {
lcd.setCursor(0, 0);
lcd.print("HELMET NOT WORN");
digitalWrite(buzzerPin, HIGH);
Blynk.virtualWrite(V4, 0);


} else {
digitalWrite(buzzerPin, LOW);
lcd.setCursor(0, 0);
lcd.print("               ");
Blynk.virtualWrite(V4, 1);
}

//DHT11 SENSOR
if (temp > 40) {
  Blynk.virtualWrite(V6, 1);
lcd.setCursor(0, 1);
lcd.print("High temp!!");
digitalWrite(buzzerPin, HIGH);
digitalWrite(ledPin1, HIGH);
} else {
  Blynk.virtualWrite(V6, 0);
lcd.setCursor(0, 1);
lcd.print("T:");
lcd.print(temp);
lcd.print("C H:");
lcd.print(humidity);
lcd.print("%  ");
digitalWrite(ledPin1, LOW);
}

//ULTRASONIC SENSOR
unsigned int distance = sonar.ping_cm();
if (distance < 30) {
  Blynk.virtualWrite(V3, distance);
lcd.setCursor(0, 1);
lcd.print("Obstacle in");
lcd.print(distance);
lcd.print("cm ");
digitalWrite(ledPin2, HIGH);
digitalWrite(buzzerPin, HIGH);
if (distance < 2) {
unsigned long startTime = millis();
while (sonar.ping_cm() < 2 && millis() - startTime < 2000) {
delay(1000);
}
if (millis() - startTime >= 2000) {
lcd.setCursor(0, 1);
lcd.print("COLLISION DETECTED");
digitalWrite(ledPin2, HIGH);
  Blynk.logEvent("collision_detected");
}
}
} else {
digitalWrite(ledPin2, LOW);
digitalWrite(buzzerPin, LOW);
}

//MQ9 GAS SENSOR
if (mq9Value > 2000) {
  Blynk.logEvent("gas_detected");
lcd.setCursor(0, 0);
lcd.print("GAS ALERT:");
lcd.print(mq9Value);
digitalWrite(buzzerPin, HIGH);
digitalWrite(ledPin3, HIGH);
} else {
digitalWrite(ledPin3, LOW);
}

// Switch
  if (digitalRead(switchPin) == HIGH)
  {
    Blynk.logEvent("panic_switch");
    Blynk.virtualWrite(V5, 1);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ALERT SENT!!");
    delay(2000);
    lcd.clear();
  }
  else {
   Blynk.virtualWrite(V5, 0);
  }
delay(1000);
}


