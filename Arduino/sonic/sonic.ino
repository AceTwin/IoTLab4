//Combine test_ap and test_sonic

/* Server
*  We write the server first because we need its IP address
* Ref:https://arduino.stackexchange.com/questions/18176/peer-to-peer-communication
* https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/server-examples.html
* https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h> //Set up the access point

const char* ssid = "ssid";       // ssid of server (Access Point (AP))
const char* password = "password";        // password of server (Access Point (AP))
//WiFiServer server(80);            //Service Port
ESP8266WebServer server(80);            //Service Port
void handleState();              // function prototypes for HTTP handlers
void handleNotFound();

int ledPin = 2; // GPIO2 of Server ESP8266
int Status = 0; // This is the state we are going to set for the client to read via HTML

//variables for Sonic  
  
  // defines pins numbers
  const int trigPin = D5;
  const int echoPin = D2;
  // defines variables
  long duration;
  int distance;


void setup() 
{    
    //Sonic
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input

    //Server
    delay(1000);
    Serial.begin(115200);    // to use tools->serial monitor
    WiFi.begin(ssid, password);

      // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500); //5 second wait
    Serial.print(".");
  }

    pinMode(ledPin, OUTPUT);   // set GPIO 2 as an output

//    WiFi.mode(WIFI_AP_STA);  // Set WiFi to AP and station mode

    // Connect to the WiFi network
    Serial.println();
    Serial.print("Connecting to: "); Serial.println(ssid);
//    WiFi.softAP(ssid, password);

    // Display the server address
    Serial.print("Connected, My address: ");
    Serial.print("http://");
//    Serial.print(WiFi.softAPIP());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
    Serial.println("/");

    // Tell the server to begin listening for incoming connections
    server.on("/state", handleState);               // Call the 'handleRoot' function when a client requests URI "/"
    server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
    server.begin();
    Serial.println("Server listening for incoming connections");
}  

void loop() 
{
   // Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
//** Sonic Code **//
    // Clears the trigPin
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
    // Prints the distance on the Serial Monitor
    //Serial.print("Distance: ");
    //Serial.println(distance);
    
    if (distance >= 100)
    {
      Status = 3;
    }
    else if (distance >=50)
    {
      Status = 2;
    }
    else if (distance >= 30)
    {
      Status = 1;
    }
    else if (distance < 30)
    {
      Status = 5;
    }

  server.handleClient();  
}

void handleState() {
  
  server.send(200, "text/plain", String(Status));   // Send HTTP status 200 (Ok) and send some text to the browser/client
  Serial.println(String(Status) + " sent to client.");
}

void handleNotFound(){
  Serial.println("Something went wrong... - 404 delivered");
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
