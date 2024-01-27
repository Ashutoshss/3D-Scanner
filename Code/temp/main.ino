/*
 3D Scanner 

 This program to make the point cloud mesh of the object using distance sensor and some stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino Mega.

 Created 6 Mar. 2023
 Modified 13 Jan. 2024
 by Ashutosh Singh

 */
#include <Stepper.h>
#include <SD.h>

//INterupt pins
const int UpStop =2;
const int DownStop =3;
const int restart = 11;

volatile bool programRunning = true;

const int rotationMotorStepPin = 2;
const int rotationMotorDirPin = 3;

const int actuatorMotorStepPin = 4;
const int actuatorMotorDirPin = 5;


const int chipSelect = 10;  // Define the SD card pins


File dataFile;    // Create a file on the SD card

const int stepsPerRevolutionRotation = 200;
const int stepsPerRevolutionActuator = 200;

//creating an object of stepper
Stepper rotationMotor(stepsPerRevolutionRotation, rotationMotorStepPin, rotationMotorDirPin);
Stepper actuatorMotor(stepsPerRevolutionActuator, actuatorMotorStepPin, actuatorMotorDirPin);


void setup() {
  Serial.begin(9600);
  
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

}


void loop() {
  if(programRunning){
    // Rotate the platform by one step
    rotatePlatform(1);
  
    // Measure distance
    float distance = measureDistance();
  
    // Print the distance to the serial monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  
    // Check if a full revolution is completed
    if (rotationMotor.currentPosition() == stepsPerRevolutionRotation) {
      // Move the actuator forward (adjust the number of steps as needed)
      moveActuator(100);
      
      // Reset the rotation motor position to start a new revolution
      rotationMotor.setCurrentPosition(0);
    }
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


void logDistance(float distance) {
  // Open the file for writing
  dataFile = SD.open("distance_log.txt", FILE_WRITE);

  // If the file is available, write to it
  if (dataFile) {
    dataFile.print("Distance: ");
    dataFile.print(distance);
    dataFile.println(" cm");

    dataFile.close();
  } else {
    Serial.println("Error opening file for writing.");
  }
}
