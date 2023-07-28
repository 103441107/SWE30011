#include <SoftwareSerial.h> 
#include "Servo.h" 
#include <SPI.h>  
#include <MFRC522.h>
#define SS_PIN 10  
#define RST_PIN 9
#define PICC_0 0xD4
#define PICC_1 0x87
#define PICC_2 0x75
#define PICC_3 0xA7

MFRC522 rfid(SS_PIN, RST_PIN); 
 
Servo myservo;
int blueTx=2;   
int blueRx=3;   
SoftwareSerial mySerial(blueTx, blueRx); 
String myString=""; 


void setup() {
  myservo.attach(6);  
  myservo.write(0);    
  mySerial.begin(9600); 
SPI.begin();
rfid.PCD_Init();
}


void RFID_S() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

 
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

   if (rfid.uid.uidByte[0] != PICC_0 || 
      rfid.uid.uidByte[1] != PICC_1 || 
      rfid.uid.uidByte[2] != PICC_2 || 
      rfid.uid.uidByte[3] != PICC_3 ) {

    Serial.println(F("This is a confirmed Card."));   
    Serial.println(F("Motor On!!"));   

 
    myservo.write(90);
    delay(3000);
    myservo.write(0);
  }
   
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

}

void Blue_motor() {
   while(mySerial.available())  
  {
    char myChar = (char)mySerial.read();  
    myString+=myChar;   
    delay(5);    }
  
  if(!myString.equals(""))  
  {
    Serial.println("input value: "+myString);
 
      if(myString=="on") 
      {
        myservo.write(90); 
        delay(3000);
        myservo.write(0);
     
    myString="";  
  }
}
}
void loop() {

  Blue_motor();
  RFID_S(); 
}

