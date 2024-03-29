/*
 3D Scanner 

 This program to make the point cloud mesh of the object using distance sensor and some stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino Mega.
 The range readings are in units of mm. 
 
 Created 6 Mar. 2023
 Modified 13 Jan. 2024
 by Ashutosh Singh
 */

#include <Stepper.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <SD.h>

//Interupt pins
const int UpStop =2;
const int DownStop =3;
const int restart = 11;

volatile bool programRunning = true;
const float ditance_betwetween_the_center_and_tof = 100;

const int rotationMotorStepPin = 2;
const int rotationMotorDirPin = 3;

const int actuatorMotorStepPin = 4;
const int actuatorMotorDirPin = 5;

//Variable 
static int currentRotationStep = 0;
float distance = 0.0;
float x = 0.0;
float y = 0.0;
float z = 0.0;


const int chipSelect = 10;  // Define the SD card pins


File dataFile;    // Create a file on the SD card
VL53L0X sensor;

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
#define HIGH_ACCURACY

const int stepsPerRevolutionRotation = 200;
const int stepsPerRevolutionActuator = 200;

//creating an object of stepper
Stepper rotationMotor(stepsPerRevolutionRotation, rotationMotorStepPin, rotationMotorDirPin);
Stepper actuatorMotor(stepsPerRevolutionActuator, actuatorMotorStepPin, actuatorMotorDirPin);


void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  pinMode(rotationMotorStepPin, OUTPUT);
  pinMode(rotationMotorDirPin, OUTPUT);
  pinMode(actuatorMotorStepPin, OUTPUT);
  pinMode(actuatorMotorDirPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(UpStop),controller,RISING);
  attachInterrupt(digitalPinToInterrupt(DownStop),reset,RISING);
  attachInterrupt(digitalPinToInterrupt(restart),reset,RISING);
  
  // Initialize SD card
  if (SD.begin(chipSelect)) {
    Serial.println("SD card initialized.");
  } else {
    Serial.println("Error initializing SD card.");
    return;
  }

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
#endif
}


void loop() {
  if(programRunning){
    // Rotate the platform by one step
    rotatePlatform(1);
  
    // Measure distance
    float x = measureDistance();

    calculation part here and save in sd card
    
//    // Print the distance to the serial monitor
//    Serial.print("Distance: ");
//    Serial.print(distance);
//    Serial.println(" cm");
  
    // Check if a full revolution is completed
    if (currentRotationStep == stepsPerRevolutionRotation) {
      // Move the actuator forward (adjust the number of steps as needed)
      moveActuator(100);
      
      // Reset the rotation motor position to start a new revolution
      currentRotationStep = 0;
    }
    currentRotationStep+=1;
  }
}


void rotatePlatform(int steps) {
  // Rotate the stepper motor
  rotationMotor.step(steps);
}

void moveActuator(int steps) {
  // Move the actuator motor
  actuatorMotor.step(steps);
}

float measureDistance() {
  distance = sensor.readRangeSingleMillimeters();
  if (sensor.timeoutOccurred()) { 
    Serial.print(" TIMEOUT");
    programRunning =false;
    }
  
  return distance;
}

void controller(){
  programRunning =false;
}
void reset(){
  while(digitalRead(DownStop) != HIGH){
    moveActuator(1);
  }
}


void logDistance(float x, float y, float z) {
  // Open the file for writing
  dataFile = SD.open("distance_log.txt", FILE_WRITE);

  // If the file is available, write to it
  if (dataFile) {
    dataFile.print(x);
    dataFile.print(", ");
    dataFile.print(y);
    dataFile.print(", ");
    dataFile.print(z);
    dataFile.println();
    
    dataFile.close();
    
  } else {
    Serial.println("Error opening file for writing.");
  }
}
