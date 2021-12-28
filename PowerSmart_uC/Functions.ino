
void Config_Pins()
{
  pinMode(LEDPin1, OUTPUT);   digitalWrite(LEDPin1,Relay1_State);
  pinMode(LEDPin2, OUTPUT);   digitalWrite(LEDPin2,Relay2_State);
  pinMode(LEDPin3, OUTPUT);   digitalWrite(LEDPin3,Relay3_State);

  pinMode(RelayPin1, OUTPUT);   digitalWrite(RelayPin1,!Relay1_State);
  pinMode(RelayPin2, OUTPUT);   digitalWrite(RelayPin2,!Relay2_State);
  pinMode(RelayPin3, OUTPUT);   digitalWrite(RelayPin3,!Relay3_State);
  pinMode(RelayPin4, OUTPUT);   digitalWrite(RelayPin4,!Relay4_State);

  pinMode(TSOP_Pin, INPUT);
  pinMode(POT_Pin, INPUT);
}


void Read_Buttons()
{
  Button1.Update();
  Button2.Update();
  Button3.Update();

  if(Button1.clicks > 0)
  	{
      ButtonTime = millis();
  	  Button1_State = 1;
  	  if(En_Serial) Serial.println("Button1 Press");
  	}
  if(Button2.clicks > 0)
  	{
      ButtonTime = millis();
  	  Button2_State = 1;
  	  if(En_Serial) Serial.println("Button2 Press");
  	}
  if(Button3.clicks > 0)
  	{
      ButtonTime = millis();
  	  Button3_State = 1;
  	  if(En_Serial) Serial.println("Button3 Press");
  	}

  if(millis() - ButtonTime > ButtonTime_SP && (Button1_State||Button2_State||Button3_State))
  {
    Button1_State = 0;
    Button2_State = 0;
    Button3_State = 0;
    Status_Buffer = 0;

    if(En_Serial) 
    {
      Serial.print("Button Status:");
      Serial.print(Button1_State);
      Serial.print(Button2_State);
      Serial.println(Button3_State);
    }
  }
}

void Set_RelaysLEDs()
{
  digitalWrite(RelayPin1, !Relay1_State);
  digitalWrite(LEDPin1,    Relay1_State);

  digitalWrite(RelayPin2, !Relay2_State);
  digitalWrite(LEDPin2,    Relay2_State);

  digitalWrite(RelayPin3, !Relay3_State);
  digitalWrite(LEDPin3,    Relay3_State);
}
