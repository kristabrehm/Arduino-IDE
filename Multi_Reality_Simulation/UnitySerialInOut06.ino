/*
This code is designed to send and receive serial data for the Fall 2023 Digital Physical Systems Class in the School of Arts, Media and Engineering
at Arizona State University. 

This Arduino code communicates with "DataInOut01.cs" in the Unity Game Engine.

The purpose of this is to enable the physical interfaces (steering wheel, brake pedal, accelerator, ligh switches, etc.) for a vehicle simulator project. 

Our vehicle is exploring and interacting with the features of another planet.

Code created by: Dr. Byron Lahey, with contributions from (list students here). The code draws on common reference examples within the Arduino IDE
and was augmented with code snippets generate by ChatGPT 3.5. 
*/


// Libraries start  
#include <HCSR04.h>
#include <Encoder.h>
#include <Servo.h>
// Libraries end    

// USB Serial names start 
#define USBSERIAL Serial
#define USBSERIAL1 SerialUSB1
// USB Serial names end 


// Servo congiguration start
Servo displayServo01;
const int servoPin = 41;
// Servo congiguration end



// Distance Sensor (acclerator pedal) configuration start   
UltraSonicDistanceSensor acceleratorSensor(0, 1);  // Initialize sensor that uses digital pins 0 and 1.
// Distance Sensor (acclerator pedal) configuration end     


// Steering Encoder configuration start   
Encoder steeringEncoder(2, 3); // Initialize encoder with pins 2 and 3 as inputs
long oldSteerEncoderPosition  = -999; // set this variable to an initial state that won't potentially match the starting position
// Steering Encoder configuration end     


// Periscope Encoder configuration start  
Encoder periscopeEncoder(40, 41); // Initialize encoder with pins 40 and 41 as inputs - update this when sensor is connected
long oldPeriscopeEncoderPosition  = -999; // set this variable to an initial state that won't potentially match the starting position
// Periscope Encoder configuration end    


// Robot Arm IMU configuratation start  

// Robot Arm IMU configuratation end    


// Sensor pins start  
const int headlightSwitchPin = 5;
const int brakeSensorPin = A14;
// Sensor pins end    


// Sensor data value variables start 
float steeringVal = 0.0;
float acceleratorPedalVal = 0.0;
bool headlightSwitchState = 1;
float brakeSensorVal = 0.0;
int periscopeVal = 0;
float imuX = 0.0;
float imuY = 0.0;
float imuZ = 0.0;
// Sensor data value variables start 

// Actuator pins start 
const int brakeLedPin = 40;
// Actuator pins start 


void setup() {
  USBSERIAL.begin(115200);
  USBSERIAL1.begin(9600);
  //USBSERIAL1.println("blue");

  pinMode(brakeSensorPin, INPUT);
  pinMode(headlightSwitchPin, INPUT_PULLUP);
  pinMode(brakeLedPin, OUTPUT);
  digitalWrite(brakeLedPin, HIGH);

  displayServo01.attach(servoPin);
}

void loop() {
  readSensors();
  sendSensorData();
  readIncomingData();
  delay(20);
}

void readSensors() {
  // read steering encoder
   long newSteerEncoderPosition = steeringEncoder.read();
  if (newSteerEncoderPosition != oldSteerEncoderPosition) {
    oldSteerEncoderPosition = newSteerEncoderPosition;
   steeringVal = map(newSteerEncoderPosition, -160, 160, -1000, 1000);
   steeringVal = steeringVal * 0.001;
  }
  // end steering encoder

  // read periscope encoder
   long newPeriscopeEncoderPosition = periscopeEncoder.read();
  if (newSteerEncoderPosition != oldSteerEncoderPosition) {
    oldSteerEncoderPosition = newSteerEncoderPosition;
    periscopeVal = map(newPeriscopeEncoderPosition, -200, 200, -360, 360);
  }
  // end periscope encoder


  // read robot arm IMU
  // end robot arm IMU


  // read brake sensor start
  brakeSensorVal = analogRead(brakeSensorPin);
  brakeSensorVal = map(brakeSensorVal, 0, 1023, 0, 1000);
  brakeSensorVal = brakeSensorVal * 0.001;
  // read brake sensor end

  // read headlightSwitch start
  headlightSwitchState = digitalRead(headlightSwitchPin);
  // read headlightSwitch end

  // read acceleratorPedal start
  acceleratorPedalVal = acceleratorSensor.measureDistanceCm();
  acceleratorPedalVal = map(acceleratorPedalVal, 3, 13, 1000, 0);
  acceleratorPedalVal = acceleratorPedalVal * 0.001;
  // read acceleratorPedal end

}

void sendSensorData() {
  USBSERIAL.print(steeringVal);
  USBSERIAL.print(" ");
  USBSERIAL.print(acceleratorPedalVal);
  USBSERIAL.print(" ");
  USBSERIAL.print(brakeSensorVal);
  USBSERIAL.print(" ");
  USBSERIAL.print(headlightSwitchState);
  USBSERIAL.print(" ");
  USBSERIAL.print(imuX);
  USBSERIAL.print(" ");
  USBSERIAL.print(imuY);
  USBSERIAL.print(" ");
  USBSERIAL.println(imuZ);
  USBSERIAL.print(" ");
  USBSERIAL.println(periscopeVal);
}

void readIncomingData() {
  if (USBSERIAL.available() > 0) {
    // Read the incoming data as a string
    String data = USBSERIAL.readStringUntil('\n'); // Assuming '\n' is the end of your message
    
    // Split the data into three parts using the ',' delimiter
    int commaIndex1 = data.indexOf(',');
    int commaIndex2 = data.indexOf(',', commaIndex1 + 1);
    int commaIndex3 = data.indexOf(',', commaIndex2 +1);
    
    if (commaIndex1 != -1 && commaIndex2 != -1) {
      // Extract the three parts as strings
      String trackedY1Str = data.substring(0, commaIndex1);
      String trackedY2Str = data.substring(commaIndex1 + 1, commaIndex2);
      String trackedY3Str = data.substring(commaIndex2 + 1, commaIndex3);
      String distanceBetweenStr = data.substring(commaIndex3 +1);
      
      // Convert the string values to integers
      /*    For this example, the variables are all initialized here.
                It might be more practical to initialize them as global varibles (outside of any functions)
                so that they are accessible to other functions that use them. 
      */
      float trackedY1 = trackedY1Str.toFloat();
      int trackedY2 = trackedY2Str.toInt();
      int trackedY3 = trackedY3Str.toInt();
      float distanceBetween = distanceBetweenStr.toFloat();

      //int ledPWM = abs(trackedY1 % 128);
      //analogWrite(ledPin, ledPWM);
      
      // Now you can work with the parsed values
     USBSERIAL1.print("Received trackedY1: ");
     USBSERIAL1.println(trackedY1);
     USBSERIAL1.print("Received trackedY2: ");
     USBSERIAL1.println(trackedY2);
     USBSERIAL1.print("Received trackedY3: ");
     USBSERIAL1.println(trackedY3);
     USBSERIAL1.print("Received distanceBetween: ");
     USBSERIAL1.println(distanceBetween);
    }
  }
}


