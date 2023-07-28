#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SocketIoClient.h>
#include <SoftwareSerial.h>

ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;

#define LED_1_PIN D5
#define LED_2_PIN D6

const byte RX = D2;
const byte TX = D3;
SoftwareSerial mySerial(RX,TX);
String dataSend = "";
void sendDataToArduino( int LED_1, int LED_2)
{
  dataSend = "";
  dataSend = "{\"LED_1_PIN\":\""+ (String)LED_1 + "\"," +
              "\"LED_2_PIN\":\""+ (String)LED_2+ "\"}" ;
  Serial.println(dataSend); 
  mySerial.println(dataSend);
  mySerial.flush();
}


#define SWITCH_ID_1 "sw_01"
bool state_1 = false;
#define SWITCH_ID_2 "sw_02"
bool state_2 = false;
int a = 0;
int b = 0;

void setup()
{
    Serial.begin(9600);
    mySerial.begin(9600);

    WiFiMulti.addAP("Quan", "12345678");
    while (WiFiMulti.run() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }
    Serial.println("Wifi connected!");
    Serial.println();
    webSocket.on("esp-execute-state", eventToggle);
    webSocket.begin("group4iot.herokuapp.com", 80, "/socket.io/?transport=websocket");
}

void loop()
{
    webSocket.loop();
    delay(10);
}

void eventToggle(const char *payload, size_t length)
{
    Serial.println(payload);
    DynamicJsonBuffer Json;
    JsonObject &root = Json.parseObject(payload);
    if (!root.success())
    {
        Serial.println("fail");
    }
    else
    {
        String swId = root["id"];
        if (swId == SWITCH_ID_1)
        {
            state_1 = !state_1;
            emitData(SWITCH_ID_1, state_1);
        
           if(state_1 == true)
           {
             a = 1;
             sendDataToArduino(a, b);
           }
           else
           {
             a = 0;
             sendDataToArduino(a, b);
           }
        }
        else if (SWITCH_ID_2)
        {
            state_2 = !state_2;
            emitData(SWITCH_ID_2, state_2);
            if(state_2 == true)
           {
             b = 1;
             sendDataToArduino(a, b);
           }
           else
           {
             b = 0;
             sendDataToArduino(a, b);
           }
        }
    }
}

void emitData(String SWITCHID, bool STATE)
{
    StaticJsonBuffer<250> Json;
    JsonObject &root = Json.createObject(); 
    root["id"] = SWITCHID;
    if (SWITCHID == SWITCH_ID_1)
    {
        root["state"] = STATE;
    }
    else if (SWITCHID == SWITCH_ID_2)
    {
        root["state"] = STATE;
    }
    String tempString;
    root.printTo(Serial);
    Serial.println();
    root.printTo(tempString);
    int len = 0;
    while (tempString[len] != NULL)
    {
        len++;
    }
    char strEmit[len + 1];
    tempString.toCharArray(strEmit, len + 1);

    webSocket.emit("esp-update-state", strEmit);
}
