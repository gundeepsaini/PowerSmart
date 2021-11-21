

WebSocketsClient webSocket;

uint64_t heartbeatTimestamp = 0;
bool isConnected = false;




// Example payloads
        // {"deviceId": xxxx, "action": "setPowerState", value: "ON"} // https://developer.amazon.com/docs/device-apis/alexa-powercontroller.html
        // {"deviceId": xxxx, "action": "AdjustBrightness", value: 3} // https://developer.amazon.com/docs/device-apis/alexa-brightnesscontroller.html
        // {"deviceId": xxxx, "action": "setBrightness", value: 42} // https://developer.amazon.com/docs/device-apis/alexa-brightnesscontroller.html
        // {"deviceId": xxxx, "action": "SetColor", value: {"hue": 350.5,  "saturation": 0.7138, "brightness": 0.6501}} // https://developer.amazon.com/docs/device-apis/alexa-colorcontroller.html
        // {"deviceId": xxxx, "action": "DecreaseColorTemperature"} // https://developer.amazon.com/docs/device-apis/alexa-colortemperaturecontroller.html
        // {"deviceId": xxxx, "action": "IncreaseColorTemperature"} // https://developer.amazon.com/docs/device-apis/alexa-colortemperaturecontroller.html
        // {"deviceId": xxxx, "action": "SetColorTemperature", value: 2200} // https://developer.amazon.com/docs/device-apis/alexa-colortemperaturecontroller.html





void Alexa_Config()
{
  webSocket.begin("iot.sinric.com", 80, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setAuthorization("apikey", SECRET_Sinric_API_Key);
  webSocket.setReconnectInterval(5000);
}


void Alexa_Handle() 
{
  webSocket.loop();  
  if(isConnected) 
  {
    if((millis()/1000 - heartbeatTimestamp) > 300) 
      {
        heartbeatTimestamp = millis()/1000;
        webSocket.sendTXT("H");          
      }
  }   
}





void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) 
{
  switch(type) 
  {
    case WStype_DISCONNECTED:
	      isConnected = false;    	      
	      Serial.printf("Sinric disconnected\n");
	      break;

    case WStype_CONNECTED: 
	      isConnected = true;
	      Serial.printf("Sinric connected\n");	      		      	
	      break;

    case WStype_TEXT:
    	{
       Serial.printf("Sinric cmd:%s\n", payload);
       
       DynamicJsonDocument json(1024);
       deserializeJson(json, (char*) payload);              

       String deviceId = json ["deviceId"];
       int device = 0;       
       if(deviceId == SECRET_Sinric_DeviceID3a)
          device = 1;
       if(deviceId == SECRET_Sinric_DeviceID3b)
          device = 2;
       if(deviceId == SECRET_Sinric_DeviceID3c)
          device = 3;
      
       String action = json ["action"];
       String value = json ["value"];
       if(action != "setPowerState")
          device = 0;

       switch(device)
       {
        case 1:
                if(value == "ON") 
                    Relay1_State = 1;
                else
                    Relay1_State = 0;  
                break;
        case 2:
                if(value == "ON") 
                    Relay2_State = 1;
                else
                    Relay2_State = 0;  
                break;
        case 3:    
                if(value == "ON") 
                    Relay3_State = 1;
                else
                    Relay3_State = 0;  
                break;            
       }

       break;
      }
    
    case WStype_BIN:
      {
        Serial.printf("Get binary length: %u\n", length);
        break;
      }    
  }
}
