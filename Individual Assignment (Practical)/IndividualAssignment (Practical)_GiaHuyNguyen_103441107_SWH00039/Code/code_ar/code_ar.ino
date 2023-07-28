#include <LiquidCrystal.h>// Import library for LCD Display
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4; // Define pin for LCD with 4bit mode
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // Initialize LCD with define pins
int sensorA = 3; // Initialize Pin 3 for IR sensor A
int sensorB = 2; // initialize pin 2 for IR sensor B
unsigned long t1 = 0; // Define Time1 Variable
unsigned long t2 = 0; // Define Time2 Variable
float speed;  // Define Speed variable
int i = 0;
void setup()
{
  lcd.begin(16, 2);// Start 16X2 LCD display
  lcd.setCursor(2, 0); // Set the cursor position for LCD to print
  lcd.print("Welcome to");
  lcd.setCursor(0, 1); // Set the cursor position for LCD to print
  lcd.print(" Swinburne Hanoi");
  delay(1000);
  lcd.clear();
  pinMode(sensorA, INPUT); // Define IR SensorA pin as Input
  pinMode(sensorB, INPUT); // Define IR SensorB pin as Input
  lcd.setCursor(0, 0);
  lcd.print("Speed Measurement");

}

void loop()
{

  while (digitalRead(sensorA)); // Read IR SensorA
  while (digitalRead(sensorA) == 0);
  t1 = millis(); // Record Time1
  while (digitalRead(sensorB)); // Read IR SensorA
  t2 = millis(); // Record Time2
  speed = t2 - t1;
  speed = speed / 1000; //convert millisecond to second
  speed = (5.0 / speed); //v=d/t
  speed = speed * 3600; //multiply by seconds per hr
  speed = speed / 1000; //division by meters per Km
  for (int i = 5; i > 0; i)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Object speed = ");
    lcd.setCursor(3, 1);
    lcd.print(speed);
    lcd.print(" Km/hr   ");
    delay(500);
    lcd.setCursor(3, 1);
    lcd.print("            ");
    delay(500);
  }
}
