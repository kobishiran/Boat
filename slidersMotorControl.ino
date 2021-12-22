/*************************************************************

  You can receive sliderLeft and sliderRight coords for joystick movement within App.

  App project setup:
    Two Axis Joystick on V1 in MERGE output mode.
    MERGE mode means device will receive both sliderLeft and sliderRight within 1 message
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

int sliderLeft = 0;
int sliderRight = 0;

int MAX_INPUT_VALUE = 1000000;

BLYNK_WRITE(V0) 
{
   int sliderLeft = param.asInt();
   motorsControl(speedPinA,speedPinB,motorApin1,motorApin2,motorBpin1,motorBpin2,sliderLeft,sliderRight);
}
BLYNK_WRITE(V1) 
{
   int sliderRight = param.asInt();
   motorsControl(speedPinA,speedPinB,motorApin1,motorApin2,motorBpin1,motorBpin2,sliderLeft,sliderRight);
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

void motorsControl(int speedPinA, int speedPinB, int motorApin1,int motorApin2,int motorBpin1, int motorBpin2,int sliderLeft,int sliderRight)
{
 
  
  
  //Controlling speed (0 = off and 255 = max speed):
  sliderLeft=map(sliderLeft,-1000000,1000000,-255,255);
  sliderRight=map(sliderRight,-1000000,1000000,-255,255);
  
  analogWrite(speedPinA,abs(sliderLeft)); //ENA pin
  analogWrite(speedPinB,abs(sliderRight)); //ENB pin

  if(abs(sliderLeft)<100)
  {
    analogWrite(speedPinA,0); //ENA pin
  }
  if(abs(sliderRight)<100)
  {
    analogWrite(speedPinB,0); //ENB pin
  }

  //Controlling spin direction of motors:
  if(sliderLeft>0)
  {
    digitalWrite(motorApin1, HIGH);
    digitalWrite(motorApin2, LOW);
  }
  else
  {
    digitalWrite(motorApin1, LOW);
    digitalWrite(motorApin2, HIGH);
  }

  if(sliderRight>0)
  {
    digitalWrite(motorBpin1, HIGH);
    digitalWrite(motorBpin2, LOW);
  }
  else
  {
    digitalWrite(motorBpin1, LOW);
    digitalWrite(motorBpin2, HIGH);
  }

  Serial.print(sliderLeft);
  Serial.print(",");
  Serial.println(sliderRight);
}
