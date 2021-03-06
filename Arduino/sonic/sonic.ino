//Implementing the sonic with MQTT

/*References:
 * Sonic: https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/

*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <PubSubClient.h> //allows connection to MQTT broker

//WiFi
const char* ssid = "IOTCC"; //WiFi Name
const char* password = "IOTCC515"; //WiFi Password

//MQTT
const char* mqtt_server = "192.168.3.1"; //Raspberry Pi's (MQTT Broker) IP
const int mqttPort = 1883; //default port

//Not sure if useful or not
const char* Status = "0"; // This is the state we are going to set for the client to read via HTML

WiFiClient espClient;
PubSubClient client(espClient);

//variables for Sonic  
  
  // defines pins numbers
  const int trigPin = D5;
  const int echoPin = D2;
  // defines variables
  long duration;
  int distance;

//int Status = 0; // This is the LED status we are going to send to the "distance" channel

void callback(char* topic, byte* payload, unsigned int length) { //Watch MQTT messages
 
  Serial.print("Message arrived in topic: "); //Which channel
  Serial.println(topic);
 
  Serial.print("Message:"); //prints the message from the channel
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------"); //Seperates the messages
 
}


void setup() 
{    
  //Sonic
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  //Make a connection to WiFi
  Serial.begin(115200);    // to use tools->serial monitor
  WiFi.begin(ssid, password); //connect to WiFi

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500); //0.5 second wait
    Serial.print(".");
  }

  //Show connection details
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //MQTT Connection/Action
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);

  //MQTT Connection or Fail to Connect
  while (!client.connected()) {
   Serial.print("Attempting MQTT connection...");
   // Attempt to connect
   if (client.connect("ESPSonic")){ //If connected to MQTT Server
    client.subscribe("test/status");
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
}  
void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESPSonic")) {
  Serial.println("connected");
  client.subscribe("test/status");
 } else {
 Serial.print("failed, rc=");
 Serial.print(client.state());
 Serial.println(" try again in 2 seconds");
 // Wait 5 seconds before retrying
 delay(2000);
  }
 }
}
void loop() 
{
  // Check if dropped and reconnect
  if (!client.connected()) {
    reconnect();
  }

  // Send sonic pulse and read result.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  
  //Debug: Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  //converting int (the distance) to char -- Ref: https://www.instructables.com/id/Converting-integer-to-character/
  //char b[5];
  //String str=String(distance);
  //str.toCharArray(b,5);
  char *CurrentStatus = "0";
  if (distance >= 100)// && distance <= 150)
  {
    CurrentStatus = "3";

    //client.publish("test/distance", b);
  }
  if (distance >=50 && distance < 100)
  {
    CurrentStatus = "2";

    //client.publish("test/distance", b);
  }
  if (distance >= 30 && distance < 50)
  {
    CurrentStatus = "1";

    //client.publish("test/distance", b);
  }
  if (distance < 30)
  {
    CurrentStatus = "5";
    
    //client.publish("test/distance", b);
  }
  // If Status has changed, publish it.
  if (CurrentStatus != Status){
    client.publish("test/status", CurrentStatus);
    Status = CurrentStatus;
    Serial.println("Pushed update");
  }
  //code for MQTT

  client.loop();
  delay(100);
  
}
