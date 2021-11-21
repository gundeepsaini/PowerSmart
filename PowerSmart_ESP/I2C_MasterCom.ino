


void READ_I2C_Data()
{ 
  byte DataBytes 	= 1;
  I2C_Com_Err 	= 1;
  Button_Status = 0;

  byte ACK = Wire.requestFrom(ArduinoAddress, DataBytes);
  while(Wire.available()) 
  { 
    I2C_Com_Err = 0;
    Button_Status = Wire.read();
  }
}


void Button_CMD()
{
  if(Button_Status & 0b00000001)    Relay1_State = !Relay1_State;
  if(Button_Status & 0b00000010)    Relay2_State = !Relay2_State;
  if(Button_Status & 0b00000100)    Relay3_State = !Relay3_State;
}

void Prep_I2C_Data()
{
  CMD_Buffer = 0;
  
  if(Relay1_State)        CMD_Buffer = CMD_Buffer | 0b00000001;
  if(Relay2_State)        CMD_Buffer = CMD_Buffer | 0b00000010;  
  if(Relay3_State)        CMD_Buffer = CMD_Buffer | 0b00000100;  
}

void Send_I2C_Cmd()
{
	if(CMD_Buffer != Last_CMD_Buffer)
	{
	  	Wire.beginTransmission(ArduinoAddress);
	  	Wire.write(CMD_Buffer);
	  	Wire.endTransmission();

	  	Last_CMD_Buffer = CMD_Buffer;
	  	Last_Relay1_State = Relay1_State;
		  Last_Relay2_State = Relay2_State;
  		Last_Relay3_State = Relay3_State;
	}
}
