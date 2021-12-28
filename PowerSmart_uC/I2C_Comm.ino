


void I2C_Config()
 {
    Wire.begin(ArduinoAddress);            // Start with Slave Address                
    Wire.onRequest(Status_RequestEvent);   // Status send function
    Wire.onReceive(CMD_ReceiveEvent);      // Command read function
 }


void Status_RequestEvent() 
{
  Wire.write(Status_Buffer);
  Button1_State = 0;
  Button2_State = 0;
  Button3_State = 0;
  Status_Buffer = 0;
}


void CMD_ReceiveEvent(int ByteCount) 
{
  if(Wire.available()>0)
    CMD_Buffer = Wire.read();
}


void Set_StatusBuffer()
{
  if(Button1_State)        Status_Buffer = Status_Buffer | 0b00000001;
  if(Button2_State)        Status_Buffer = Status_Buffer | 0b00000010;  
  if(Button3_State)        Status_Buffer = Status_Buffer | 0b00000100;   

  if(En_Serial && Status_Buffer >0) 
    {
      Serial.print("Status_Buffer");
      Serial.println(Status_Buffer); 
    }
}

void Execute_CMD()
{
  if(CMD_Buffer & 0b00000001)    Relay1_State = true;
  else                           Relay1_State = false;

  if(CMD_Buffer & 0b00000010)    Relay2_State = true;
  else                           Relay2_State = false;

  if(CMD_Buffer & 0b00000100)    Relay3_State = true;
  else                           Relay3_State = false;

  if(En_Serial && CMD_Buffer >0) 
    {
      Serial.print("CMD_Buffer");
      Serial.println(CMD_Buffer); 
    }
}
