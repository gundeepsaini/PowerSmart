

void LCD_Update()
{
	// ---- ---- ---- ----
	// ---- ---- ---- ----

	lcd.clear();
	
	lcd.setCursor(0,0);
  	lcd.print("I:");
  	lcd.print((Irms));   
  	lcd.print("A");

	lcd.setCursor(10,0);
  	lcd.print("P:");
  	lcd.print(int(Power));
  	lcd.print("W");

  	lcd.setCursor(0,1);
  	lcd.print("mins:");
	
	if(Relay1_TimeLeft >0)
	{
		lcd.setCursor(5,1);
  		lcd.print(Relay1_TimeLeft);
	}
  	if(Relay2_TimeLeft >0)
	{
  		lcd.setCursor(9,1);
  		lcd.print(Relay2_TimeLeft);
	}
  	if(Relay3_TimeLeft >0)
	{
  		lcd.setCursor(13,1);
  		lcd.print(Relay3_TimeLeft);
	}
}



void LCD_BacklightControl()
{
	if(LCD_TMR_Start > 0)
	{
		lcd.backlight();
		if(millis()/1000 - LCD_TMR_Start > LCD_TMR_SP)
			{
				lcd.noBacklight();
				LCD_TMR_Start = 0;
			}
	}
}
