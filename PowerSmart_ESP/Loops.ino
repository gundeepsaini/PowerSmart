

void Always_loop()
{
  	OTA_Check();
}


void Fast_Loop()
{
    Blynk.run();
    server.handleClient();
    Alexa_Handle();
    
    READ_I2C_Data();
	Button_CMD();
	Prep_I2C_Data();
	Relay_StateChange_Events();
	Send_I2C_Cmd();
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
  
}


void VerySlow_Loop()
{
   //Blynk_Graph_DataPush();
}