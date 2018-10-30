#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h> //allows to connect to MQTT broker

//WiFi
const char* ssid = "..."; //WiFi Name
const char* password = "..."; //WiFi Password
const char* mqtt_server = "x.x.x.x"; //Raspberry Pi's (MQTT Broker) IP
const int mqttPort = 1883; //default port

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: "); //Which channel
  Serial.println(topic);
 
  Serial.print("Message:"); //prints the message from the channel
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------"); //Seperates the messages
 
}

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200); //console

 WiFi.begin(ssid, password); //Connect to WiFi
 Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500); //5 second wait
    Serial.print(".");
  }

 //Show connection details
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

 client.setServer(mqtt_server, mqttPort);
 client.setCallback(callback);

 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Client")){ //If connected to MQTT Server
  Serial.println("connected");
 } 
 else { //if not connected to MQTT Server
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }

 client.publish("esp/test", "Hello from me"); //publish to the esp/test channel
 client.subscribe("esp/test"); //listen for messages in the esp/test channel
}

void loop()
{
 client.loop(); //Keep going through code.
}

//References:
//Best one - https://techtutorialsx.com/2017/04/09/esp8266-connecting-to-mqtt-broker/
//https://www.learnrobotics.org/blog/how-to-configure-node-red-and-wemos-d1-mini-using-mqtt/
//https://www.baldengineer.com/mqtt-tutorial.html
