#include <ESP8266WiFi.h>
#include <PubSubClient.h> //allows to connect to MQTT broker

//WiFi
const char* ssid = "phoenix"; //WiFi Name
const char* password = "ThisistheTSNPhoenix"; //WiFi Password
const char* mqtt_server = "hackiot"; //Raspberry Pi's (MQTT Broker) IP

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0; i<length; i++){
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);
  }
  Serial.println();
}

void reconnect(){
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Client")) {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("test");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);

// WiFi.begin(ssid, password);
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());
}

void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
}

//References:
//https://www.learnrobotics.org/blog/how-to-configure-node-red-and-wemos-d1-mini-using-mqtt/
//https://www.baldengineer.com/mqtt-tutorial.html
