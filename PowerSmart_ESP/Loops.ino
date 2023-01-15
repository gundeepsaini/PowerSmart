

void Always_loop()
{
  	OTA_Check();
}


void Fast_Loop()
{
   //Blynk.run();
   server.handleClient();
    //Alexa_Handle();
    
   READ_I2C_Data();
	Button_CMD();
	Prep_I2C_Data();
	Relay_StateChange_Events();
	Send_I2C_Cmd();
	MQTT_loop();  
}



void Mid1_Loop()
{
	Relay_TMR_PowerOff();
}



void Mid2_Loop()
{
	//Blynk_ForceUpdate();
	LCD_BacklightControl();
	LCD_Update();	
	Calc_Energy();  	
}



void Slow_Loop()
{
  //Blynk_Graph_DataPush();
   MQTT_heartbeat();

   if(!Time_NTP_isValid())
      Time_NTP_Update(); 
}


void VerySlow_Loop()
{   
 	Time_NTP_Update();
}