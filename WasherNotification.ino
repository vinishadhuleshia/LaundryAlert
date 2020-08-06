#include <ESP8266WiFi.h> 

char server[] = "mail.smtpcorp.com";
ADC_MODE(ADC_VCC);
#include <ESPMail.h>

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
WiFiClient client;

//CHANGE TO YOUR NETWORK DETAILS
const char* ssid = "WIFI";
const char* password = "PASSWORD";


bool washerStatus = false;
unsigned int lastVibration = 0;
unsigned int vibrationCount = 0;
unsigned int lastTimeSteady = 0;
bool washerDone = false;
bool lastState = false;

#define washer_pin D0


void setup(){
  washerDone = false;
  Serial.begin(9600);
  Serial.println();

  Serial.print("Connecting");

  //connects to the wifi at start
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  delay(10);
  Serial.print("Connecting");
  Serial.println("WiFi Connected");
  
}

void loop() {
  pinMode(washer_pin, INPUT);
  Serial.println("Measuring..");

  //if the washer is not done check if it is
  if(!washerDone)
  {
    if(hasStopped(washer_pin))
    {
      Serial.println("WASHER DONE");
      washerDone = true;
      sendEmail();
    }
  }

}

bool hasStopped(int pin)
{
  int value = digitalRead(pin);
  //calculates the time since the last vibration and the last time it wasn't vibrating
  int now = millis();
  int timeSinceLastVibration = now - lastVibration;
  int timeSinceLastSteady = now - lastTimeSteady;

  //if it has been at least 1 second and the vibration count is greater than 0, it will decrease it by one
  if (vibrationCount > 0 && timeSinceLastSteady > 1000) {
    vibrationCount--;
    lastTimeSteady = now;
    Serial.print(value);
    Serial.print(": ");
    Serial.println(vibrationCount);
  }
    // If the sensor went off see what 
    bool state = digitalRead(pin) == 0 ? false : true;
    if (lastState != state) {
      lastState = state;
    //Can only collect a vibration every 1/4 second
    if (timeSinceLastVibration > 250) {
      lastVibration = now;
      
      if (vibrationCount < 305) {
        vibrationCount++;
      }
    }
  }

  //washer was on but has been 5 minutes since its off
  if (washerStatus && vibrationCount == 0) {
    washerStatus = false;
    Serial.println("Washer is done!");
    return true;
  }

  //washer is running if the vibration count gets to at least 300
  if (!washerStatus && vibrationCount > 300) {
    washerStatus = true;
    Serial.println("Washer is running!");
  }
  return false;
}

byte sendEmail()
{
    byte thisByte = 0;
    byte respCode;
    //connect to the server and return if it doesnt connect
    if (client.connect(server, 2525) == 1) {
      Serial.println(F("connected"));
    } 
    else {
      Serial.println(F("connection failed"));
      return 0;
    }

    //checks for error after each command
    if (!eRcv()) return 0;
    client.println("EHLO www.example.com");
    if (!eRcv()) return 0;
    client.println("auth login");
    if (!eRcv()) return 0;

    //Login to sender email
    // CHANGE to your base64, ASCII encoded user
    client.println("d2FzaGVybm90aWZpY2F0aW9u"); // SMTP UserID
    if (!eRcv()) return 0;
    // CHANGE to your base64, ASCII encoded password
    client.println("dGI1MjNWRDAw");//  SMTP Passw
    
    if (!eRcv()) return 0;
    //CHANGE TO SENDER ADDRESS
    client.println(F("MAIL From: SENDER@gmail.com"));
    if (!eRcv()) return 0; 
    // CHANGE TO RECIPIENT ADDRESS
    client.println(F("RCPT To: RECIPIENT@gmail.com"));
    if (!eRcv()) return 0;
    client.println(F("DATA"));
    if (!eRcv()) return 0;
    
    Serial.println(F("Sending email"));
    //CHANGE TO RECIPIENT ADDRESS
    client.println(F("To: RECIPIENT@gmail.com"));
    //CHANGE TO SENDER ADDRESS
    client.println(F("From: SENDER@gmail.com"));
    client.println(F("Subject: Washer Notification\r\n"));

    //random message sent each time
    int messageNumber = random(1,5);
    if(messageNumber == 1)
    {
      client.println(F("Wet and soggy and leady to go!"));
    }
    else if(messageNumber == 2)
    {
      client.println(F("Move your clothes to the dryer before they get moldy!"));
    }
    else if(messageNumber ==3)
    {
      client.println(F("Put the clothes in the dryer already!"));
    }
    else
    {
      client.println(F("Get up and put your clothes in the dryer...jeez!"));
    }
    
    //necessary to indicate the end of the email
    client.println(F("."));
    if (!eRcv()) return 0;
    client.println(F("QUIT"));
    if (!eRcv()) return 0;
    client.stop();
    Serial.println(F("disconnected"));
    return 1;
  }
  byte eRcv()
  {
    byte respCode;
    byte thisByte;
    int loopCount = 0;
    while (!client.available())
    {
      delay(1);
      loopCount++;     
      
      // if nothing received for 10 seconds, timeout
      if (loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
      }
    }

  respCode = client.peek();
  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
  }

  if (respCode >= '4')
  {
    return 0;
  }
  return 1;
}
