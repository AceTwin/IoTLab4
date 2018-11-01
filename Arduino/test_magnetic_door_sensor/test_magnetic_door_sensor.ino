int switchreed = D5; //pin for input of sensor

void setup() {
  // put your setup code here, to run once:
  pinMode(switchreed, INPUT_PULLUP); //give me fixed levels
  Serial.begin(115200);
}

void loop() { //test whether door is open or closed
  // put your main code here, to run repeatedly:
 
  if (digitalRead(switchreed)==LOW){ //if magnets are touching
    Serial.println("Your Door is Closed"); //print to console
  }
  else //if magnets are not touching
  {
    Serial.println("Your Door is Open");//print to console
  }
  delay(1); //Let's put a delay
}

//Ref: https://randomnerdtutorials.com/monitor-your-door-using-magnetic-reed-switch-and-arduino/
