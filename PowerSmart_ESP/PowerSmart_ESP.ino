/*-----------------------------------------

            Power Smart Outlet Board

Date: Jan 2020 (v4)
Device: ESP8266 NodeMcU  <---> Atmega328
Block Diagram / Circuit Diagram: OneNote Workbook

Note:
  - Reset ESP after upload, disable IPv6, 
      reset adapter, allow port in firewall
  - send "OTA" on blynk terminal to enter dedicated mode
    or navigate to "ip/OTA" for OTA through web portal
  
To-Do:
  - Web server: WebPage detailing with on/off function  
  - Power Calc: Setup CT calc in Arduino


------------------------------------------- */


/* ------------- LIB ------------------------------ */
#include "Secrets.h"
#include <Wire.h> 
#include "src/LiquidCrystal/LiquidCrystal_I2C.h"
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
//#include <NTPClient.h>
//#include <TimeLib.h>
//#include <WebSocketsClient.h>
//#include <ArduinoJson.h> 



/* ------------- CONFIG VAR ------------------------------ */
unsigned long looptime_Fast = 0;    // in secs
unsigned long looptime_Mid1 = 2;    // in secs
unsigned long looptime_Mid2 = 5;   // in secs
unsigned long looptime_Slow       = 30;      // in secs
unsigned long looptime_VerySlow   = 05 *(60);      // in secs
unsigned long NTP_Update_Period   = 06 *(60*60);    // in secs
unsigned long utcOffsetInSeconds  = 01 *(60*60);    // in secs
unsigned long LCD_TMR_SP          = 30;             // in secs

#define ArduinoAddress 11


/* ------------- VAR ------------------------------ */
const char* OTA_Password     = SECRET_Device_OTA_PASS; 
const char* ssid             = SECRET_WIFI_SSID2;
const char* pass             = SECRET_WIFI_PASS2;
const char* DeviceHostName   = SECRET_Device_Name3;
const char* BlynkAuth        = SECRET_BLYNK_AUTH3;

unsigned long lastrun_fast, lastrun_Mid1;
unsigned long lastrun_Mid2, lastrun_slow, lastrun_Veryslow;
unsigned long lastrun_30mins,lastrun_OTA, LastNTP_Update;
bool OTA_Mode=0;
String webpage1="";
float Irms, Power;
bool Relay1_State,Relay2_State,Relay3_State;
bool Last_Relay1_State,Last_Relay2_State,Last_Relay3_State;
int Relay1_TMR=0,Relay2_TMR=120,Relay3_TMR=120;
unsigned long  Relay1_TMR_Start,Relay2_TMR_Start,Relay3_TMR_Start;
unsigned long LCD_TMR_Start=1;
int Relay1_TimeLeft,Relay2_TimeLeft,Relay3_TimeLeft;
byte Button_Status, CMD_Buffer, Last_CMD_Buffer=200;
byte I2C_Com_Err;


//(lcd_Addr,En,Rw,Rs,d4,d5,d6,d7,backlighPin,t_backlighPol)
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3,POSITIVE);
WidgetTerminal terminal(V0);
ESP8266WebServer server(80);


/* ------------------------------------------- */

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(115200);
  Serial.println(DeviceHostName);

  Wire.begin(12,14); 
  
  lcd.begin(16,2);
  lcd.backlight();
  lcd.clear();
  lcd.print(DeviceHostName);

  EEPROM.begin(512);
  
  wifi_station_set_hostname(DeviceHostName);
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, pass);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("Wifi Error! Rebooting in 30s...");
    delay(30 *1000);
    digitalWrite(LED_BUILTIN, HIGH);
    Restart_ESP();
  }
  
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  lcd.setCursor(0,1);
  lcd.print("Wifi");

  WebServer_Config();
  OTA_Config();
  Blynk_Config();
  Read_Memory();
  //Alexa_Config();
  Time_NTP_Config(); 
  
  lcd.setCursor(0,1);
  lcd.print("Ready!");  
  Serial.println("Ready");
  digitalWrite(LED_BUILTIN, HIGH);
}




void loop() 
{
  // Always
  Always_loop();

  // Fast Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_fast > looptime_Fast) || lastrun_fast ==0))
  {
    lastrun_fast = millis()/1000;
    Fast_Loop();
  }

  // Mid1 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid1 > looptime_Mid1) || lastrun_Mid1 ==0))
  {
    lastrun_Mid1 = millis()/1000;
    Mid1_Loop();
  }

  // Mid2 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid2 > looptime_Mid2) || lastrun_Mid2 ==0))
  {
    lastrun_Mid2 = millis()/1000;
    Mid2_Loop();
  }

  // Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_slow > looptime_Slow) || lastrun_slow ==0))
  {
    lastrun_slow = millis()/1000;
    Slow_Loop();
  }

    // Very Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Veryslow > looptime_VerySlow) || lastrun_Veryslow ==0))
  {
    lastrun_Veryslow = millis()/1000;
    VerySlow_Loop();
  }
}



void Restart_ESP()
{
  Serial.println("Restarting ESP");
  ESP.restart();
  delay(1000);
  while(1);
}
