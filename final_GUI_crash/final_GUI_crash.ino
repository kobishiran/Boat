


/*************************************************************

  You can receive sliderLeft and sliderRight coords for joystick movement within App.

  App project setup:
    Two Axis Joystick on V1 in MERGE output mode.
    MERGE mode means device will receive both sliderLeft and sliderRight within 1 message
 *************************************************************/


// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLLfadNeTB"
#define BLYNK_DEVICE_NAME "SpaceShip"
#define BLYNK_AUTH_TOKEN "-ZIOaMfl77L8iO1rTpdNl07L4Ja_YfWM"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <HCSR04.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Servo.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.

//char ssid[] = "Redmi";
//char pass[] = "83hf7eg3"; 

//char ssid[] = "POCO X3 Pro";
//char pass[] = "123456789";

char ssid[] = "Avshalom";
char pass[] = "123456789";

// laptop server for area intel 
const uint16_t port = 8090;
const char * host = "192.168.43.117";

//channels properties
const int freq = 5000;
const int resolution = 8;

// wifi-connection status
WiFiClient client;
bool isLaptopConnected;

// dc motor pins
int motorApin1 = 16; 
int motorApin2 = 4; 
int speedPinA = 17; 
int speedACh=14;

int motorBpin1 = 0; 
int motorBpin2 = 2; 
int speedPinB = 15; 
int speedBCh=15;

int laserPin = 18;
int laserCh=10;
byte ultraSonicTrigPin = 14; 
int ultraSonicTrigCh = 9; 
byte* ultraSonicEchoPins = new byte[2] {12, 25};


// servo pins
int gateServoPin = 5;
int gateCh=12;
int laserServoPin = 19;
int laserServoCh=11;
int ultraSonicServoPin = 13;
int ultraSonicServoCh = 10;

int catapultServoPin = 26;
int catapultCh=13;
//servo definitions
Servo gateServo;
Servo laserServo;
Servo catapultServo;
Servo ultraSonicServo;

int sliderLeft = 0;
int sliderRight = 0;
int state = 0;

//left motor slider
BLYNK_WRITE(V0) 
{
   sliderLeft = param.asInt();
   switch(state)
   {
    //dc control
    case 0:
    
      motorsControl(speedACh,speedBCh,motorApin1,motorApin2,motorBpin1,motorBpin2,sliderLeft,sliderRight);
      break;
    //catapult control
    case 1:
    
      catapultServo.write(map(sliderLeft,-255,255,0,180));
      break;  
    
    //laser control
    case 2:
    
      laserServo.write(map(sliderLeft,-255,255,0,180));
      break;
   }
    Serial.print(sliderLeft);
    Serial.print(",");
    Serial.print(sliderRight);
    Serial.print(",");
    Serial.println(state);
}
//right motor slider
BLYNK_WRITE(V1) 
{
   sliderRight = param.asInt();
   switch(state)
   {
    //dc control
    case 0:
    
      motorsControl(speedACh,speedBCh,motorApin1,motorApin2,motorBpin1,motorBpin2,sliderLeft,sliderRight);
      break;
    //gate control
    case 1:
    
      gateServo.write(map(sliderRight,-255,255,0,180));
      break;
    //laser on/off
    case 2:
    
      ledcWrite(laserCh,map(sliderRight,-255,255,255,0));
      break;
    Serial.print(sliderLeft);
    Serial.print(",");
    Serial.print(sliderRight);
    Serial.print(",");
    Serial.println(state);
   }
}
//states slider
BLYNK_WRITE(V2) 
{
   state = param.asInt();
    Serial.print(sliderLeft);
    Serial.print(",");
    Serial.print(sliderRight);
    Serial.print(",");
    Serial.println(state);
}

// establish connection with server
bool establishLaptopConnection()
{
 
    if (!client.connect(host, port)) {
 
        Serial.println("Connection to host failed");
 
        return false;
    }
 
    Serial.println("Connected to server successful!");
 
    client.print("Hello from ESP32!");
    return true;
}

// scan environment and send over wifi
BLYNK_WRITE(V3)
{
  if (!isLaptopConnected){
    return;
  }
  for (int pos = 0; pos <= 180; pos += 1){
    ultraSonicServo.write(pos);
    delay(45);
    double* distances = HCSR04.measureDistanceCm();
    client.println(String(pos) + ", " + String(distances[0]) + ", " + String(distances[1]));
  }
  for (int pos = 180; pos >= 0; pos -= 1){
    ultraSonicServo.write(pos);
    delay(45);
    double* distances = HCSR04.measureDistanceCm();
    client.println(String(pos) + ", " + String(distances[0]) + ", " + String(distances[1]));
  }
  
}


void setup()
{
  
  // pin initialition
  pinMode(motorApin1, OUTPUT);
  pinMode(motorApin2, OUTPUT);
  pinMode(motorBpin1, OUTPUT);
  pinMode(motorBpin2, OUTPUT);

  pinMode(speedPinA, OUTPUT); 
  pinMode(speedPinB, OUTPUT);

  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin,HIGH);
  pinMode(ultraSonicTrigPin, OUTPUT);
  pinMode(ultraSonicEchoPins[0], INPUT);
  pinMode(ultraSonicEchoPins[1], INPUT);
  pinMode(gateServoPin, OUTPUT);
  pinMode(laserServoPin, OUTPUT);
  pinMode(catapultServoPin, OUTPUT);
  pinMode(ultraSonicServoPin, OUTPUT);

  gateServo.attach(gateServoPin);
  laserServo.attach(laserServoPin);
  catapultServo.attach(catapultServoPin);
  ultraSonicServo.attach(ultraSonicServoPin);

  ledcSetup(laserCh,freq,resolution);
  ledcSetup(speedACh,freq,resolution);
  ledcSetup(speedBCh,freq,resolution);
  ledcAttachPin(laserPin, laserCh);
  ledcAttachPin(speedPinA, speedACh);
  ledcAttachPin(speedPinB, speedBCh);

  

  // Debug console
  Serial.begin(115200);
  HCSR04.begin(ultraSonicTrigPin, ultraSonicEchoPins, byte(2));

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  isLaptopConnected = establishLaptopConnection(); 
}

void loop()
{
  Blynk.run();
  if (!isLaptopConnected){
    isLaptopConnected = establishLaptopConnection();
  }
}
 
void motorsControl(int speedACh, int speedBCh, int motorApin1,int motorApin2,int motorBpin1, int motorBpin2,int sliderLeft,int sliderRight)
{
 
  
       
  
  //Controlling speed (0 = off and 255 = max speed):
  Serial.print("speed: ");
  Serial.print(abs(sliderLeft));
  Serial.print(",");
  Serial.println(abs(sliderRight));
  ledcWrite(speedACh,abs(sliderLeft)); //ENA pin
  ledcWrite(speedBCh,abs(sliderRight)); //ENB pin

  if(abs(sliderLeft)<100)
  {
    ledcWrite(speedACh,0); //ENA pin
    Serial.println("A motor stops");
  }
  if(abs(sliderRight)<100)
  {
    ledcWrite(speedBCh,0); //ENB pin
    Serial.println("B motor stops");
  }

  //Controlling spin direction of motors:
  if(sliderLeft>0)
  {
    Serial.println("A motor dir 1");
    digitalWrite(motorApin1, HIGH);
    digitalWrite(motorApin2, LOW);
  }
  else
  {
    Serial.println("A motor dir 2");
    digitalWrite(motorApin1, LOW);
    digitalWrite(motorApin2, HIGH);
  }

  if(sliderRight>0)
  {
    Serial.println("B motor dir 1");
    digitalWrite(motorBpin1, HIGH);
    digitalWrite(motorBpin2, LOW);
  }
  else
  {
    Serial.println("B motor dir 2");
    digitalWrite(motorBpin1, LOW);
    digitalWrite(motorBpin2, HIGH);
  }

  
}
