

//---------------------------------------------

void Blynk_Config()
{
  Blynk.config(BlynkAuth);
  while (Blynk.connect(1000) == false); 
  lcd.print(" Blynk"); 
}


void Blynk_Graph_DataPush()
{

}

//---------------------------------------------


void Blynk_ForceUpdate()
{
  Blynk.virtualWrite(V4, Relay1_State);
  Blynk.virtualWrite(V5, Relay2_State);
  Blynk.virtualWrite(V6, Relay3_State);

  Blynk.virtualWrite(V7, Relay1_TMR);
  Blynk.virtualWrite(V8, Relay2_TMR);
  Blynk.virtualWrite(V9, Relay3_TMR);

  Blynk.virtualWrite(V10, Relay1_TimeLeft);
  Blynk.virtualWrite(V11, Relay2_TimeLeft);
  Blynk.virtualWrite(V12, Relay3_TimeLeft);
}


//---------------------------------------------

BLYNK_WRITE(V0)
{

}


BLYNK_READ(V1)
{
  Blynk.virtualWrite(V1, Irms);
}

BLYNK_READ(V2)
{
  Blynk.virtualWrite(V2, Power);
}

BLYNK_READ(V3)
{
  Blynk.virtualWrite(V3, I2C_Com_Err);
}

BLYNK_WRITE(V4)
{
  boolean State = param.asInt();
  Relay1_State = State;
}

BLYNK_WRITE(V5)
{
  boolean State = param.asInt();
  Relay2_State = State;
}

BLYNK_WRITE(V6)
{
  boolean State = param.asInt();
  Relay3_State = State;
}

BLYNK_WRITE(V7)
{
  int TMR = param.asInt();
  Relay1_TMR = TMR;
}

BLYNK_WRITE(V8)
{
  int TMR = param.asInt();
  Relay2_TMR = TMR;
}

BLYNK_WRITE(V9)
{
  int TMR = param.asInt();
  Relay3_TMR = TMR;
}

BLYNK_READ(V10)
{
  Blynk.virtualWrite(V10, Relay1_TimeLeft);
}

BLYNK_READ(V11)
{
  Blynk.virtualWrite(V11, Relay2_TimeLeft);
}

BLYNK_READ(V12)
{
  Blynk.virtualWrite(V12, Relay3_TimeLeft);
}