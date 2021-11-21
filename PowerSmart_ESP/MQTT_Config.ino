

// https://pubsubclient.knolleary.net/api


/******************** LIB **************************************/
#define MQTT_MAX_PACKET_SIZE 1024  // < ----- Change in lib: This is because the defaul maxium length is 128b. So just go to PubSubClient.h and change #define MQTT_MAX_PACKET_SIZE 128 to #define MQTT_MAX_PACKET_SIZE 1024
#include <PubSubClient.h>
#include <ArduinoJson.h>


/********************* Var *************************************/
WiFiClient espClient;
PubSubClient client(espClient);

long lastReconnectAttempt = 0;

/******************** Config **************************************/
const char* mqtt_server     = SECRET_MQTT_Server;
const char* mqtt_user       = SECRET_MQTT_User;
const char* mqtt_password   = SECRET_MQTT_Pass;

// Light
#define MQTT_TOPIC_STATE       "HA/Devices/PowerSmart/state" 
#define MQTT_TOPIC_CMD1        "HA/Devices/PowerSmart/cmd1"
#define MQTT_TOPIC_CMD2        "HA/Devices/PowerSmart/cmd2"
#define MQTT_TOPIC_CMD3        "HA/Devices/PowerSmart/cmd3"  

// Will Topic - Availability
#define MQTT_TOPIC_WILL        "HA/Devices/PowerSmart/status"     
#define MQTT_OFFLINE           "Offline"
#define MQTT_ONLINE            "Active"

#define MQTT_ON                "ON"
#define MQTT_OFF               "OFF"



/**************** External Functions ************************************/

void MQTT_Config()
{
	client.setServer(mqtt_server, SECRET_MQTT_Port);
 	client.setCallback(MQTT_MessageRecd_callback);  
}


void MQTT_loop()
{
	if (!client.connected())
    	MQTT_reconnect();              
  
  client.loop();
}


void MQTT_publish()
{   
    MQTT_heartbeat();
}


/**************** Internal Functions ************************************/


void MQTT_reconnect() 
{
  if (millis()/1000 - lastReconnectAttempt > 30 || lastReconnectAttempt == 0) 
  {
      Serial.println("MQTT reconnecting");
      
      //boolean connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage], [cleanSession])
      if (client.connect(DeviceHostName, mqtt_user, mqtt_password, MQTT_TOPIC_WILL, 1, true, MQTT_OFFLINE)) 
      {
        //MQTT_publish_config();  
        Serial.println("MQTT connected");
        client.publish(MQTT_TOPIC_WILL, MQTT_ONLINE, true);
        client.subscribe(MQTT_TOPIC_CMD1);
        client.subscribe(MQTT_TOPIC_CMD2);
        client.subscribe(MQTT_TOPIC_CMD3);
      }
      lastReconnectAttempt = millis()/1000;
  }
}


void MQTT_MessageRecd_callback(char* p_topic, byte* p_payload, unsigned int p_length) 
{
  String payload;
  for (uint8_t i = 0; i < p_length; i++) 
    { payload.concat((char)p_payload[i]); }

  if (String(MQTT_TOPIC_CMD1).equals(p_topic)) 
  {
    int cmd = payload.toInt();
    Serial.print("MQTT CMD1:");
    Serial.println(cmd);
    
    if(cmd == 1) 
        Relay1_State = 1;
    else
        Relay1_State = 0;  

    MQTT_heartbeat();
  }

  if (String(MQTT_TOPIC_CMD2).equals(p_topic)) 
  {
    int cmd = payload.toInt();
    Serial.print("MQTT CMD2:");
    Serial.println(cmd);
    
    if(cmd == 1) 
        Relay2_State = 1;
    else
        Relay2_State = 0;  

    MQTT_heartbeat();
  }
  
  if (String(MQTT_TOPIC_CMD3).equals(p_topic)) 
  {
    int cmd = payload.toInt();
    Serial.print("MQTT CMD3:");
    Serial.println(cmd);
    
    if(cmd == 1) 
        Relay3_State = 1;
    else
        Relay3_State = 0;  

    MQTT_heartbeat();
  }  

}


void MQTT_heartbeat()
{  
  int msg = 0;

  if(Relay1_State == 1) 
        msg = msg + 100;
  else
        msg = msg + 0;
  
  if(Relay2_State == 1) 
        msg = msg + 10;
  else
        msg = msg + 0;      

  if(Relay3_State == 1) 
        msg = msg + 1;
  else
        msg = msg + 0;

  char data[5];
  itoa(msg, data, 5);

  client.publish(MQTT_TOPIC_STATE, data, true);
}
