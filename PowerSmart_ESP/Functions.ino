
void Relay_StateChange_Events()
{	

	if( Last_Relay1_State != Relay1_State ||
	   	Last_Relay2_State != Relay2_State ||
	   	Last_Relay3_State != Relay3_State)
	{
		//Serial.println("State Change Detected");
    
    	Write_Memory(0);		
		unsigned long now = millis()/(1000*60);
		LCD_TMR_Start = millis()/1000;

		if(Last_Relay1_State != Relay1_State)
		 {
			if(Relay1_State && Relay1_TMR !=0)
			{
				Relay1_TMR_Start = now +1;
				Relay1_TimeLeft = Relay1_TMR;
			}
			if(!Relay1_State)
			{
				Relay1_TMR_Start = 0;
				Relay1_TimeLeft	 = 0;
			}
		 } 

		if(Last_Relay2_State != Relay2_State)
		 {
			if(Relay2_State && Relay2_TMR !=0)
			{
				Relay2_TMR_Start = now +1;
				Relay2_TimeLeft = Relay2_TMR;
			}
			if(!Relay2_State)
			{
				Relay2_TMR_Start = 0;
				Relay2_TimeLeft	 = 0;
			}
		 }

		if(Last_Relay3_State != Relay3_State)
		 {
			if(Relay3_State && Relay3_TMR !=0)
			{
				Relay3_TMR_Start = now +1;
				Relay3_TimeLeft = Relay3_TMR;
			}
			if(!Relay3_State)
			{
				Relay3_TMR_Start = 0;
				Relay3_TimeLeft	 = 0;
			}
		 }

		LCD_Update();
		Blynk_ForceUpdate();		
	}
}

void Relay_TMR_PowerOff()
{	
	unsigned long now = millis()/(1000*60);

	if(Relay1_TMR_Start > 0)
	{
		if(now - Relay1_TMR_Start +1 >= Relay1_TMR)
		{
			Relay1_TMR_Start = 0;
			Relay1_State 	 = 0;
		}

		Relay1_TimeLeft = Relay1_TMR + Relay1_TMR_Start -1 - now;
	}

	/*
	Serial.println();
	Serial.println(millis()/(60*1000));	//4	5	6	7
	Serial.println(Relay2_TMR_Start); 	//5	5	5	5
	Serial.println(Relay2_TMR);			//2	2	2	2
	*/

	if(Relay2_TMR_Start > 0)
	{
		if(now - Relay2_TMR_Start +1 >= Relay2_TMR)
		{
			Relay2_TMR_Start = 0;
			Relay2_State 	 = 0;
			//Serial.print("Off");
		}

		Relay2_TimeLeft = Relay2_TMR + Relay2_TMR_Start -1 - now;
	}
	//Serial.println(Relay2_TimeLeft);	//2	1	0 -6


	if(Relay3_TMR_Start > 0)
	{
		if(now - Relay3_TMR_Start +1 >= Relay3_TMR)
		{
			Relay3_TMR_Start = 0;
			Relay3_State 	 = 0;
		}

		Relay3_TimeLeft = Relay3_TMR + Relay3_TMR_Start -1 - now;
	}
}



void Calc_Energy()
{	
	if(Relay1_State || Relay2_State || Relay3_State)
	{
		Irms 	= float(random(0,50)) / 100.00;
		Power 	= 220.00 * Irms * 0.90;  
	}
	else
	{
		Irms 	= float(random(0,5)) / 1000.00;
		Power 	= 220.00 * Irms * 0.90;  
	}
}





void Read_Memory()
{
  int address = 100;
  byte value;

  value = EEPROM.read(address);
  Button_Status = value;
  Button_CMD();
  
  value = EEPROM.read(address+1);
  if(Relay1_State)
  	Relay1_TMR = int(value);
  value = EEPROM.read(address+2);
  if(Relay2_State)
  	Relay2_TMR = int(value);
  value = EEPROM.read(address+3);
  if(Relay3_State)
  	Relay3_TMR = int(value);
  
  //Serial.println(Relay1_TMR);
  //Serial.println(Relay2_TMR);
  //Serial.println(Relay3_TMR);
}

void Write_Memory(int howMany)
{
  int address = 100;
  byte value;

  value = CMD_Buffer;
  EEPROM.write(address, value);
  if(howMany > 0)
  {
      if(Relay1_State)
      	value = byte(Relay1_TimeLeft);
      else
      	value = 0;
      EEPROM.write(address+1, value);
      //Serial.println(value);

      if(Relay2_State)
      	value = byte(Relay2_TimeLeft);
      else
      	value = 0;
      EEPROM.write(address+2, value);
      //Serial.println(value);

      if(Relay3_State)
      	value = byte(Relay3_TimeLeft);
      else
      	value = 0;
      EEPROM.write(address+3, value);
      //Serial.println(value);
  }
  EEPROM.commit();
}
