// Designing Creativity Support Tools A1
//Haley Greenyer 

// Include NewPing and CapacitiveSensor Libraries
#include "NewPing.h"
#include <CapacitiveSensor.h>

//define the sensor pins
CapacitiveSensor   button1 = CapacitiveSensor(4, 2);
CapacitiveSensor   button2 = CapacitiveSensor(4, 3);

// smoothing variables
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

//initialize capacitive touch sensing variables
int total1val = 5000; 
int total2val = 5000; //adjustable threshold
int state1 = HIGH;    //state of the 'buttons'
int previous1 = LOW;
int state2 = HIGH;
int previous2 = LOW;

String out1 = "H"; //what will be sent to the Serial Monitor
String out2 = "H";

long time1 = 0; //last time the output pin was toggled
long time2 = 0;
long debounce = 200;


#define TRIGGER_PIN 9
#define ECHO_PIN 11

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 10  

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float duration;
int distance;

void setup() 
{
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  
  // initialize all the readings to 0 (smoothing)
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() 
{
  long start = millis();
  long total1 =  button1.capacitiveSensor(1000);
  long total2 =  button2.capacitiveSensor(1000);

  //button 1 pressed?
  if (total1 > total1val && millis() - time1 > debounce) {
    if (state1 == HIGH){
      state1 = LOW;
      previous1 = LOW; //to track the state of the button
      out1 = "L";  //to send to the Serial Monitor
    }
    else {
      state1 = HIGH;
      previous1 = HIGH;
      out1 = "H";
    }
  }

  //button 2 pressed?
  if (total2 > total2val && millis() - time2 > debounce) {
    if (state2 == HIGH){
      state2 = LOW;
      previous2 = LOW;
      out2 = "L";
    }
    else {
      state2 = HIGH;
      previous2 = HIGH;
      out2 = "H";
    }
  }

 //LED indication of button state
  digitalWrite(10, state1);
  digitalWrite(13, state2);
  
 //Ultrasonic sensor reading
  distance = sonar.ping_cm();

  //smoothing (average readings)
  total = total-readings[readIndex];
  if (distance < 15 && distance > 0){
    readings[readIndex] = distance;
  }
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if(readIndex >= numReadings){
    readIndex=0;
  }

  //to send to Serial Monitor 
  average = total/numReadings;

  //output to Serial Monitor
  String distSend = String(average);
  Serial.println(distSend + ' ' + out1 + ' ' + out2);
  delay(200);
}
