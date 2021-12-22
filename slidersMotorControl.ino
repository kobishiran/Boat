/*************************************************************

  You can receive x and y coords for joystick movement within App.

  App project setup:
    Two Axis Joystick on V1 in MERGE output mode.
    MERGE mode means device will receive both x and y within 1 message
 *************************************************************/


// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLDiKtWUe9"
#define BLYNK_DEVICE_NAME "rc joystick"
#define BLYNK_AUTH_TOKEN "-ZIOaMfl77L8iO1rTpdNl07L4Ja_YfWM"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <Arduino.h>
#include <analogWrite.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "POCO X3 Pro";
//char pass[] = "123456789";

char ssid[] = "Avshalom";
char pass[] = "123456789";

//int motorApin1 = 14;
//int motorApin2 = 27;
//int motorBpin1 = 32;
//int motorBpin2 = 33;
//int speedPinA  = 26;
//int speedPinB  = 25;


int motorApin1 = 16; 
int motorApin2 = 4; 
int speedPinA = 17; 

int motorBpin1 = 0; 
int motorBpin2 = 2; 
int speedPinB = 15; 

int x = 0;
int y = 0;

int MAX_INPUT_VALUE = 1000000;

BLYNK_WRITE(V0) 
{
   int x = param.asInt();
   motorsControl(speedPinA,speedPinB,motorApin1,motorApin2,motorBpin1,motorBpin2,x,y);
}
BLYNK_WRITE(V1) 
{
   int y = param.asInt();
   motorsControl(speedPinA,speedPinB,motorApin1,motorApin2,motorBpin1,motorBpin2,x,y);
}
void setup()
{
  

  pinMode(motorApin1, OUTPUT);
  pinMode(motorApin2, OUTPUT);
  pinMode(motorBpin1, OUTPUT);
  pinMode(motorBpin2, OUTPUT);

  pinMode(speedPinA, OUTPUT); 
  pinMode(speedPinB, OUTPUT);

  


  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
}

void motorsControl(int speedPinA, int speedPinB, int motorApin1,int motorApin2,int motorBpin1, int motorBpin2,int x,int y)
{
 
  
  
  //Controlling speed (0 = off and 255 = max speed):
  x=map(x,-1000000,1000000,-255,255);
  y=map(y,-1000000,1000000,-255,255);
  
  analogWrite(speedPinA,abs(x)); //ENA pin
  analogWrite(speedPinB,abs(y)); //ENB pin

  if(abs(x)<100)
  {
    analogWrite(speedPinA,0); //ENA pin
  }
  if(abs(y)<100)
  {
    analogWrite(speedPinB,0); //ENB pin
  }

  //Controlling spin direction of motors:
  if(x>0)
  {
    digitalWrite(motorApin1, HIGH);
    digitalWrite(motorApin2, LOW);
  }
  else
  {
    digitalWrite(motorApin1, LOW);
    digitalWrite(motorApin2, HIGH);
  }

  if(y>0)
  {
    digitalWrite(motorBpin1, HIGH);
    digitalWrite(motorBpin2, LOW);
  }
  else
  {
    digitalWrite(motorBpin1, LOW);
    digitalWrite(motorBpin2, HIGH);
  }

  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
}
