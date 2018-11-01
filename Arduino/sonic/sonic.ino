//Implementing the sonic with MQTT

/*References:
 * Sonic: https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/

*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <PubSubClient.h> //allows connection to MQTT broker

//WiFi
const char* ssid = "ssid";       // WiFi name
const char* password = "password"; // WiFi password

int Status = 0; // This is the LED status we are going to send to the "distance" channel

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

    //Make a connection to WiFi
    delay(1000);
    Serial.begin(115200);    // to use tools->serial monitor
    WiFi.begin(ssid, password); //connect to WiFi

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
}  

void loop() 
{
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

}
