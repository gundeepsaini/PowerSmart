/*-----------------------------------------

            Power Smart Outlet Board

Date: Jan 2020 (v4)
Device: ESP8266 NodeMcU  <---> Atmega328
Block Diagram / Circuit Diagram: OneNote Workbook
Programmed with external editor: Sublime Text

CHIP: Atmega328 UNO
Arduino I2C Address : 11

Pins:
  Buttons          - Pins 7,11,12
  LED              - Pins 8,9,10
  Relay            - Pins 15,5,6,4
  Current Sensor   - Pin A0 (14)  
  TSOP             - Pin A2 (16)
  Pot              - Pin A3 (17)  
  

------------------------------------------- */


// Var User Configurable-----------------
bool En_Serial=0;

unsigned long looptime_Fast = 0;    // in secs
unsigned long looptime_Mid1 = 2;    // in secs
unsigned long looptime_Mid2 = 10;   // in secs
unsigned long looptime_Slow = 30;   // in secs

#define LEDPin1 8
#define LEDPin2 9
#define LEDPin3 10
#define ButtonPin1 7
#define ButtonPin2 11
#define ButtonPin3 12
#define RelayPin1 15
#define RelayPin2 5
#define RelayPin3 4   // Interchanged with Relay 4
#define RelayPin4 6   // Interchanged with Relay 3    
#define Current_Pin 14
#define TSOP_Pin 16
#define POT_Pin 17
#define ArduinoAddress 11

// Var Sys------------------
unsigned long lastrun_fast=-100,lastrun_Mid1=-100;  // value to ensure run on power up
unsigned long lastrun_Mid2=-100,lastrun_slow=-100;
byte Status_Buffer, CMD_Buffer;
byte Relay1_State,Relay2_State,Relay3_State,Relay4_State; 
byte Button1_State,Button2_State,Button3_State;
long ButtonTime;
int ButtonTime_SP = 500; // in ms

// Lib-----------------------
#include <Wire.h> 
#include <avr/wdt.h>

#include "src/ClickButton/ClickButton.h"


// Instance-----------------
ClickButton Button1(ButtonPin1, LOW, CLICKBTN_PULLUP);
ClickButton Button2(ButtonPin2, LOW, CLICKBTN_PULLUP);
ClickButton Button3(ButtonPin3, LOW, CLICKBTN_PULLUP);

//#include "EmonLib.h"                   
//EnergyMonitor EMON;


void setup() 
{
  wdt_disable();
  wdt_reset();
  wdt_enable(WDTO_8S);

  Serial.begin(9600);
  Serial.println("Starting...");

  Config_Pins();
  I2C_Config();

  Serial.print("Initialized");
}

void loop() 
{
  wdt_reset();
  Read_Buttons();
  Set_StatusBuffer();
  Execute_CMD();
  Set_RelaysLEDs();
}
