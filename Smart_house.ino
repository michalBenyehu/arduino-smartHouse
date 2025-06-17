#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

//pins definition
#define TRIG_PIN 10
#define ECHO_PIN 9
#define DHT_PIN 8
#define RED_LED_PIN 7
#define GREEN_LED_PIN 6
#define YELLOW_LED_PIN 5
#define FAN_PIN 11

#define DHTTYPE DHT11

//devices configuration
DHT dht(DHT_PIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

float humidityStartValue = 0;
float tempretureStartValue = 0;


void startRedLight()
{    
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);  //red led on
}
void startYellowLight()
{
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, HIGH);  //yellow led on
}
void startGreenLight()
{
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);  //green led on
}
void startDistanceSensorPulse()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
}
float measureDistance()
{
    long duration;
    float distance;
    // שולחים פולס של 10 מיקרו-שניות
    startDistanceSensorPulse();
   // מודדים את הזמן שהפין Echo היה HIGH
    duration = pulseIn(ECHO_PIN, HIGH);
      // חישוב המרחק בס"מ
    distance = duration * 0.0343 / 2;
    return distance;
}




void setup() {
  Serial.begin(9600);
  //initialize DHT
  dht.begin();
  //initialize leds
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  //initialize ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  //FAN initialization
  pinMode(FAN_PIN, OUTPUT);
  //initialize lcd
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("HELLO");   //"שלום עולם!");
  lcd.setCursor(0, 1);  // שורה שנייה
  lcd.print("LCD + Arduino");

  humidityStartValue = dht.readHumidity();
  tempretureStartValue = dht.readTemperature();
}

void loop() {
  float distance;
  //handle temp&Hum
  float currHumidity = dht.readHumidity();
  float currTempreture = dht.readTemperature();
  float diffHumidity = currHumidity - humidityStartValue;
  float diffTemperature = currTempreture - tempretureStartValue;

  // Serial.print("\nhumidity: ");
  // Serial.print(currHumidity);
  // Serial.print("\ttempreture: ");
  // Serial.print(currTempreture);

  if (diffTemperature>0) {
    lcd.setCursor(0, 0);
    lcd.print("HUMIDITY: ");
    lcd.setCursor(10, 0);
    lcd.print(currHumidity);  //"שלום עולם!");
    lcd.setCursor(0, 1);      // שורה שנייה
    lcd.print("TEMPRETURE: ");
    lcd.setCursor(13, 1);
    lcd.print(currTempreture);

    digitalWrite(FAN_PIN,HIGH);
  }
  else
  {
    digitalWrite(FAN_PIN, LOW);
  }
  Serial.print("diff tempreture: ");
  Serial.print(diffTemperature);
  Serial.println("/n");
  

  distance = measureDistance();
  if (distance > 0 and distance < 50) {
      startRedLight();
  } else if (distance > 50 and distance < 100) {
      startGreenLight();
  } else {
      startYellowLight();
  }

  //Serial.println(" cm");

  delay(50);
}
