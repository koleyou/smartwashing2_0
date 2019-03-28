/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//int aPin = 35;
int dPin = 12;
int sensorValue = 0;
//float sensorAverage = 0.0;
//float threshold  = 3000;

//int numSamples = 10;

int startTime;
int offTime;
int doneTimer;
bool notificationNotSent = false;
int count=0;
bool triggered=false;
int offValue =100;
int spinValue = 200;
int crazyValue = 300;
short pastStates [] = {0,0,0};
short currentState = 0;
short currentCondition = 0;
short lastCondition = 0;
short statesSum=0;
short cycleCount = 0;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "b77bff51e7c24e68bf323dcef5827478";
//char ipAddress[] = "192.168.43.235";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "BYUI_Visitor";
char pass[] = "";

//WidgetLCD lcd(V7);
WidgetLED offLED(V4);
WidgetLED spinLED(V5);
WidgetLED crazyLED(V6);
WidgetLED doneLED(V8);
void notifications(){
  if(currentCondition != lastCondition){
    //lcd.clear();  
  }
  
  switch(currentCondition){
    case 0:
      offLED.on();
      spinLED.off();
      crazyLED.off();
      //lcd.print(0,0,"OFF");
      break;
    case 1:
      offLED.off();
      spinLED.on();
      crazyLED.off();
      //lcd.print(0,0,"SPIN");
      break;
    case 2:
      offLED.off();
      spinLED.off();
      crazyLED.on();
      //lcd.print(0,0,"CRAZY");
      Blynk.notify("Going Crazy");
      break;
  }
  
  
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(dPin,INPUT);
  Blynk.begin(auth, ssid, pass);
  //timer.setInterval(1000L, myTimerEvent)
  offLED.on();
  spinLED.off();
  crazyLED.off();
  doneLED.off();
  offTime = millis();
}
BLYNK_CONNECTED(){
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
}
BLYNK_WRITE(V1){
  offValue = param.asInt();
  offValue = offValue * 1000;
}
BLYNK_WRITE(V2){
  spinValue = param.asInt();
  spinValue = spinValue * 1000;
}
BLYNK_WRITE(V3){
  doneTimer = param.asInt();
  doneTimer *= 1000;
}
void loop()
{
  
   startTime = millis();
  while (startTime + 1000 > millis()){
    sensorValue = digitalRead(dPin);
    if(sensorValue && !triggered){
      triggered = true;
    }
    else if(sensorValue && triggered){
      count++;
      triggered = false;
      
    }
  }
  //sensorValue = digitalRead(dPin);
  //Serial.println(count);
  lastCondition = currentCondition;
  if(count >= 0 && count < offValue){
    //Serial.println("OFF");
    currentCondition = 0;
    if(offTime + doneTimer < millis() && notificationNotSent){
      doneLED.on();
      Blynk.notify("Washer is done");
      notificationNotSent = false;
    }
    
  }
  else if (count >= offValue && count < spinValue){
    //Serial.println("Spin Cycle");
    currentCondition = 1;
    offTime = millis();
    doneLED.off();
    notificationNotSent = true;
  }
  else if (count >= spinValue ){
    //Blynk.notify("The washer is going crazy");
    currentCondition = 2;
    offTime = millis();
    doneLED.off();
    notificationNotSent = true;
  }
  Blynk.virtualWrite(V0, count);
  notifications();
  
  Blynk.run();
  //Serial.println(currentCondition);
  count = 0;
}
