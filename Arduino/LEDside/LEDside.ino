// the setup function runs once when you press reset or power the board

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESP8266HTTPClient.h>
#include <PubSubClient.h> //allows to connect to MQTT broker

bool statefive = 0;
int state;

//Setup WiFi stuff
const char* ssid = "xxx"; //Enter Wifi ssid inbetween quotes
const char* password = "xxx"; //Enter Wifi password inbetween quotes

//MQTT
const char* mqtt_server = "x.x.x.x"; //Raspberry Pi's (MQTT Broker) IP
const int mqttPort = 1883; //default port
const char* magnet_status; //Variable to hold whether door is closed or open?
const char* status; //LED status

WiFiClient espClient;
PubSubClient client(espClient);
//Initialize light connections
int red = D5; //Red
int yellow = D3; //Yellow
int green = D2; //Green

void callback(char* topic, byte* payload, unsigned int length) { //Watch MQTT messages

  if (topic == "magnet/1")
  {
    Serial.print("Message arrived in topic: "); //Which channel
    Serial.println(topic);
    
    Serial.print("Message:"); //prints the message from the channel
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
      magnet_status = ((char*)payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------"); //Seperates the messages
  }
  
  if (topic == "status")
  {
    Serial.print("Message arrived in topic: "); //Which channel
    Serial.println(topic);
    
    Serial.print("Message:"); //prints the message from the channel
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
      status = ((char*)payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------"); //Seperates the messages
  }
  
}

void setup() {
  //Wifi stuff
  Serial.begin(115200);
  WiFi.begin(ssid, password);
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
  
  // initialize digital pins as output
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  //Turn off all LEDs initially
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);

  //MQTT Connection/Action
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);

  //MQTT Connection or Fail to Connect
  while (!client.connected()) {
   Serial.print("Attempting MQTT connection...");
   // Attempt to connect
   if (client.connect("ESP8266Client")){ //If connected to MQTT Server
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

   //Subscribe to the magnet channel 
   client.subscribe("magnet/1"); //listen for messages in the magnet channel
   client.subscribe("status"); //listen for messages in the status channel
}

// the loop function runs over and over again forever
void loop() {

  if (magnet_status == "closed")
  {
//    state=0;
    Serial.print("closed");
    dostuff("0"); //Make sure the stoplight is off
  }
  if (magnet_status == "open")
  {
    Serial.print("open");
    dostuff(status);
    //look for distance
   // dostuff(0);
  }
  if (!client.connected()) {
   // client.reconnect();
   Serial.println("Disconnected...");
  }
  client.loop();
}

void dostuff(const char* state){
  //state is red
  if (state == "1")
  {
  digitalWrite(red, LOW);    
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);  
  digitalWrite(red, HIGH);   // turn the LED on (HIGH is the voltage level)
  }                       
  
  //state is yellow
  if (state == "2")
  {
  digitalWrite(red, LOW);    
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);  
  digitalWrite(yellow, HIGH);
  }
   
  //state is green
  if (state == "3")
  {
  digitalWrite(red, LOW);    
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);  
  digitalWrite(green,HIGH);
  }

  //state is off
  if (state == "0")
  {
  //// turn the LEDs off by making the voltage LOW
  digitalWrite(red, LOW);    
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);
  }

  if (state == "4") //Traffic Light Cycle
  {
      //Red
      digitalWrite(red, HIGH);     // red on
      delay(1000);                //wait
      digitalWrite(red, LOW);     //red off  

      //Yellow
      digitalWrite(yellow, HIGH); //yellow on
      delay(1000);     
      digitalWrite(yellow, LOW);  //yellow off

      //Green
      digitalWrite(green,HIGH);   //green on
      delay(1000);  
      digitalWrite(green, LOW);
  }
    if (state =="5") //Traffic Light Backup
  {
      if (statefive==1){
          digitalWrite(red, HIGH);    
          digitalWrite(yellow, HIGH);
          digitalWrite(green, HIGH);
          statefive=0;
          Serial.println("ON");
        } else if (statefive==0)
        {
          digitalWrite(red, LOW);    
          digitalWrite(yellow, LOW);
          digitalWrite(green, LOW);
          statefive=1;
          Serial.println("OFF");
        }delay(200);
      }
  
  

  Serial.print("Works");
}


//Reference: https://www.arduino.cc/en/Tutorial-0007/BlinkingLED
//Wifi Reference: http://cyaninfinite.com/tutorials/getting-started-with-the-wemos-d1-esp8266-wifi-board/#Running-a-Simple-Web-Server
//Read HTTP:  https://www.esp8266.com/viewtopic.php?f=32&t=6463
//https://techtutorialsx.com/2016/07/17/esp8266-http-get-requests/
