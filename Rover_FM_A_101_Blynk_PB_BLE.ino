/**************************************************************
 * This sketch is for an Arduino 101 BLE rover using Blynk Pushbuttons and Adafruit Motor Shield V2
 * Code was copied from both Adafruit and Blynk librarie examples
 * Full documentation on building this rover yourself on Hackster.IO and electronhacks.com
 * Sketch is released under MIT license
 * 
 * The base code for this sketch comes from Arduino_101_BLE which can be found in the Arduino IDE 
 File>Examples>Blynk>Boards_Bluetooth  The Blynk Library http://www.blynk.cc/getting-started/
 must be loaded to access this example. Thank-you to Blynk for your amazing app!
 
 This sketch was inspired by Intel Arduino 101 BLE Blynk Tank posted on Electron Hacks-Thanks! https://goo.gl/gvvyVh

 The code below was developed to operate a four motor Rover-Robot with an Arduino 101 and 
 Adafruit Motor-Servo Controller Shield using the Blynk iPhone app using the Blynk "Pushbuttons" and Blynk
 "Long Slider" over a BLE Bluetooth connection. This code should work for a 2 motor Rover. You will have to 
 designate the motors appropriately in the Define Motor Port Section ~line 43. 

 Note: This code generates an error "Blynk.begin(auth, blePeripheral);"
                                                                    ^
 during compliation. It doesn't seem to be an issue.
 
 Use this code at your own risk! 
                                   
 **************************************************************/
//This is the original Blynk sketch using a slider v(0) to set the speed and 6 buttons
//to set the direction V(1-6)


#define BLYNK_USE_DIRECT_CONNECT
#define BLYNK_PRINT Serial
#include <BlynkSimpleCurieBLE.h>
#include <CurieBLE.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "43df130281184e12a763e9ccc4bef15e";

BLEPeripheral  blePeripheral;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4.
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);


//######### SETUP ######################################
void setup() {
  Serial.begin(9600);
  delay(1000);

  // The name your bluetooth service will show up as, customize this if you have multiple devices
  blePeripheral.setLocalName("JLM_Rover");
  blePeripheral.setDeviceName("JLM_Rover");
  blePeripheral.setAppearance(384);

  Blynk.begin(auth, blePeripheral);

  blePeripheral.begin();
  Serial.println("Waiting for connections...");
    
  Serial.println("Adafruit Motorshield v2 - DC Motor");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  motor1->setSpeed(255);
  motor2->setSpeed(255);
  motor3->setSpeed(255);
  motor4->setSpeed(255);
  

}


//########## LOOP ######################################
void loop() {
  Blynk.run();
  blePeripheral.poll();
}


//######### Subroutines ################################

// This function will set the speed
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V0 Slider value is: ");
  Serial.println(pinValue);
  motor1->setSpeed(pinValue);
  motor2->setSpeed(pinValue);
  motor3->setSpeed(pinValue);
  motor4->setSpeed(pinValue); 
}

//Forward-Button V1
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("Motors 1-4 Forward: ");
  Serial.println(pinValue);
  if(pinValue == 1) {
    motor1->run(FORWARD);
    motor2->run(FORWARD);
    motor3->run(FORWARD);
    motor4->run(FORWARD);
  }
  if(pinValue == 0) {
    motor1->run(RELEASE);
    motor2->run(RELEASE);
    motor3->run(RELEASE);
    motor4->run(RELEASE);
  }
}

//SOFT Right-Button V2
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("SOFT Right Turn: ");
  Serial.println(pinValue);
  if(pinValue == 1) {
    motor1->run(FORWARD);
    motor2->run(FORWARD);
    motor3->run(RELEASE);
    motor4->run(RELEASE);
  }
  if(pinValue == 0) {
    motor1->run(RELEASE);
    motor2->run(RELEASE);
    motor3->run(RELEASE);
    motor4->run(RELEASE);
  }
}


//Reverse-Button V3
BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("Motors 1-4 REVERSE: ");
  Serial.println(pinValue);
  if(pinValue == 1) {
    motor1->run(BACKWARD);
    motor2->run(BACKWARD);
    motor3->run(BACKWARD);
    motor4->run(BACKWARD);
  }
  if(pinValue == 0) {
    motor1->run(RELEASE);
    motor2->run(RELEASE);
    motor3->run(RELEASE);
    motor4->run(RELEASE);
  }
}

//SOFT Left-Button V4
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("SOFT Right Turn: ");
  Serial.println(pinValue);
  if(pinValue == 1) {
    motor1->run(RELEASE);
    motor2->run(RELEASE);
    motor3->run(FORWARD);
    motor4->run(FORWARD);
  }
  if(pinValue == 0) {
    motor1->run(RELEASE);
    motor2->run(RELEASE);
    motor3->run(RELEASE);
    motor4->run(RELEASE);
  }
}

//Hard Right-Button V5
BLYNK_WRITE(V5)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("HARD Right Turn: ");
  Serial.println(pinValue);
  if(pinValue == 1) {
    motor1->run(FORWARD);
    motor2->run(FORWARD);
    motor3->run(BACKWARD);
    motor4->run(BACKWARD);
  }
  if(pinValue == 0) {
    motor1->run(RELEASE);
    motor2->run(RELEASE);
    motor3->run(RELEASE);
    motor4->run(RELEASE);
  }
}

//Hard Left-Button V6
BLYNK_WRITE(V6)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("HARD LEFT Turn: ");
  Serial.println(pinValue);
  if(pinValue == 1) {
    motor1->run(BACKWARD);
    motor2->run(BACKWARD);
    motor3->run(FORWARD);
    motor4->run(FORWARD);
  }
  if(pinValue == 0) {
    motor1->run(RELEASE);
    motor2->run(RELEASE);
    motor3->run(RELEASE);
    motor4->run(RELEASE);
  }
}
