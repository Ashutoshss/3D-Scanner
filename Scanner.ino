#include "Adafruit_VL53L0X.h"
#include <SPI.h>
#include <SD.h>

File root;  
String fileName = "PointData.txt";

//Variable Initialization
//float x = 0.0, y = 0.0, z = 0.0;
volatile bool SystemState = true;


//Defining Pins
const int stepPin1 = 2; 
const int  dirPin1 = 3;
const int stepPin2 = 4; 
const int  dirPin2 = 5;

//Uncomment and set up if you want to use custom pins for the SPI communication
//#define REASSIGN_PINS
//int sck = -1;
//int miso = -1;
//int mosi = -1;
int cs = 4;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);
  while (! Serial) {delay(1);}
  if (!SD.begin(cs)) {Serial.println("initialization failed!");while (1);}
  if (!lox.begin()) {Serial.println(F("Failed to boot VL53L0X"));while(1);}

  pinMode(stepPin1,OUTPUT);
  pinMode(dirPin1,OUTPUT);
  pinMode(stepPin2,OUTPUT);
  pinMode(dirPin2,OUTPUT);
  
  pinMode(2,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2),Warning,CHANGE);
}

void loop() {
  if (!SystemState) {
    Serial.print("Stop the actuator safely");
    while (true) {delay(100); }}  // Stop the actuator safely
    
  VL53L0X_RangingMeasurementData_t measure;
  
  //Makes 200 pulses for making one full cycle rotation
  for(int i = 0; i < 200; i++){
    lox.getRangingMeasurement(&measure, false); // pass in 'true' to get debug data printout!
    if (measure.RangeStatus != 4) { // phase failures have incorrect data
      WriteSD(measure.RangeMilliMeter);
    } else {
      WriteSD(0.0);       //If their is any empty space between the object
    }
    digitalWrite(stepPin1,HIGH);
    delay(10);
    digitalWrite(stepPin1,LOW);
    delay(10);
  }
  //for the Vertical Actuator
  for(int j = 0; j < 200; j++){
    digitalWrite(stepPin2,HIGH);
    delay(5);
    digitalWrite(stepPin2,LOW);
    delay(5);
  }
}

void Warning(){
  SystemState = false;
}

void WriteSD(float SDx){
  root = SD.open(fileName,FILE_WRITE);
  if(root){root.println(SDx);root.close();
    } else {Serial.println("Error opening file for writing.");}
}

/*
 Created 6 Mar. 2023
 Modified 13 Jan. 2024
 by Ashutosh Singh
 */
