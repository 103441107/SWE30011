
#include "ArduinoJson.h"
#include <SoftwareSerial.h>
const byte RX = 2;
const byte TX = 3;
SoftwareSerial mySerial(RX, TX);
String dataEsp = "";
String dataSend = "";
#define pin_led1 4
#define pin_led2 5

void read_Esp8266()
{
  while (mySerial.available())
  {
    char inChar = (char)mySerial.read();

    if (inChar != '\n')
    {
      dataEsp += inChar;
    }
    else
    {
      Serial.println(dataEsp);
      const size_t capacity = JSON_OBJECT_SIZE(2) + 256;
      DynamicJsonDocument JSON(capacity);
      deserializeJson(JSON, dataEsp);

      String a = JSON["LED_1_PIN"]; 
      String b = JSON["LED_2_PIN"];
      int LED_1_PIN = a.toInt();
      int LED_2_PIN = b.toInt();

      digitalWrite(pin_led1, LED_1_PIN);
      digitalWrite(pin_led2, LED_2_PIN);
      Serial.print("LED_1_PIN: ");
      Serial.println(LED_1_PIN);
      Serial.print("LED_2_PIN: ");
      Serial.println(LED_2_PIN);
      JSON.clear();
      dataEsp = "";
      unsigned long time1 = millis();
      while (millis() <= time1 + 100)
        ;
      read_Status();
    }
  }
}

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(pin_led1, OUTPUT);
  pinMode(pin_led2, OUTPUT);
  digitalWrite(pin_led1, 0);
  digitalWrite(pin_led2, 0);
}

void loop()
{
  read_Esp8266();
}
