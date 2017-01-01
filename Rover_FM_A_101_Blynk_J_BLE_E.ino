/**************************************************************
 * This sketch is for an Arduino 101 BLE rover using Blynk and Adafruit Motor Shield V2
 * Code was copied from both Adafruit and Blynk librarie examples
 * Full documentation on building this rover yourself on Hackster.IO and electronhacks.com
 * Sketch is released under MIT license
 *************************************************************/

// The base code for this sketch adapted from Arduino_101_BLE which can be found in the Arduino IDE 
// File>Examples>Blynk>Boards_Bluetooth  The Blynk Library http://www.blynk.cc/getting-started/
// must be loaded to access this example. Thank-you to Blynk for your amazing app!

// Joystick code adapted from SparkFun Joystick Code -  https://goo.gl/FGvKYN  Thank-you to SparkFun
// for your parts and help!

//This sketch was inspired by Intel Arduino 101 BLE Blynk Tank posted on Electron Hacks-Thanks! https://goo.gl/gvvyVh

// The code below was developed to operate a four motor Rover-Robot with an Arduino 101 and 
// Adafruit Motor-Servo Controller Shield-V2 using the Blynk iPhone app. 
 
// On your phone in the Blynk app you need the following 4 widgets:
// 1) Pushbutton switch on Virtual Pin: V1, labelled: Left Pirouette, Mode: Switch
// 2) Pushbutton switch on Virtual Pin: V3, labelled: Right Pirouette, Mode: Switch
// 3) Joystick on V0, Split/Merge: Merge, Auto Return to: ON, Rotate on Tilt to: OFF
// 4) SliderL on Virtual Pin: V4, Labelled: Motor Speed 0..............255 
  
// Note: This code generates an error "Blynk.begin(auth, blePeripheral);"
//                                                                    ^
// during compliation. It doesn't seem to be an issue.

 
// **********Use this code at your own risk!********** 
                                                                   
 
//**********Libraries**********

#define BLYNK_USE_DIRECT_CONNECT
#define BLYNK_PRINT Serial
#include <BlynkSimpleCurieBLE.h>
#include <CurieBLE.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

int pinValue;   //Motor Speed Variable from Blynk Long slider Switch V(4)
int pinValueAdjusted;  //Converted Motor Speed Variable from Blynk Long slider Switch V(4) for turns

//**********Blynk Authentication Token**********
// Get Auth Token in the Blynk Phone App.
// Go to the Project Settings (nut icon).
char auth[] = "43df130281184e12a763e9ccc4bef15e";

BLEPeripheral  blePeripheral;

//**********Create the motor shield object with the default I2C address**********
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

//**********Define the Motor 'ports' M1, M2, M3 or M4 on the Adafruit Shield**********
//LF = Left Front;  LR = Left Rear; RF = Right Front; RR = Right Rear

Adafruit_DCMotor *motorLF = AFMS.getMotor(1);//Left Front Motor
Adafruit_DCMotor *motorLR = AFMS.getMotor(2);//Left Rear Motor
Adafruit_DCMotor *motorRF = AFMS.getMotor(3);//Right Front Motor
Adafruit_DCMotor *motorRR = AFMS.getMotor(4);//Right Rear Motor


//**********VOID SETUP**********
void setup() {
  Serial.begin(9600);
  delay(1000);

// The name your bluetooth service will show up as in the phone app.
// Customize this if you have multiple devices.
  blePeripheral.setLocalName("JLM_Rover");
  blePeripheral.setDeviceName("JLM_Rover");
  blePeripheral.setAppearance(384);

  Blynk.begin(auth, blePeripheral);

  blePeripheral.begin();
  
  Serial.println("Waiting for connections...");
  Serial.println("Adafruit Motorshield v2 - DC Motor");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  //Set the initial motor speed to start to 255 (max speed)
  motorLF->setSpeed(255);
  motorLR->setSpeed(255);
  motorRF->setSpeed(255);
  motorRR->setSpeed(255);
}
//**********END VOID SETUP**********


//**********VOID LOOP**********
void loop() {
  Blynk.run();
  blePeripheral.poll();
}
//**********END VOID LOOP**********


//**********Blynk Functions**********

//**********Read Blynk Slider Swtich and Convert to Motor Speed Variables**********
BLYNK_WRITE(V4) {
 pinValue = param.asInt(); // assigning incoming value from pin V4 to a variable for Motor Speed (0-255)
 pinValueAdjusted = pinValue * 0.6;  //Motor speed reduction for turning side wheels (diagonal turns)
                                 //This value may need to be different for your chassis and wheels

  Serial.print("V4 Slider value is: ");
  Serial.println(pinValue);
  motorLF->setSpeed(pinValue);
  motorLR->setSpeed(pinValue);
  motorRF->setSpeed(pinValue);
  motorRR->setSpeed(pinValue); 
}

//**********Translate the Joystick Position to a Rover Direction**********
//
//This function translates the joystick movement to a Rover direction.
//Blynk Joysick is centered at y=128, x=128 with a range of 0-255. Thresholds set the Joystick 
//sensitivity. These are my adjustments for my touch, you may need something different. Making
//the range too tight will make the rover hard to control. Note: Y values greater than 128 will
//drive the motors FOWARD. Y values less than 128 will drive the motorS in REVERSE. The Rover will
//turn in the direction of the "slow" or unpowered (RELEASE) wheels.
//
//  Joystick Movement along x, y Axis
// (Inside the * is the Threshold Area)
//            y=255--(y_position=255, x_position=128; y_direction=+1, x_direction=0)
//           * | *
//           * | *
//           * | *
//   ********* | *********
//x=0---------------------x=255--(y_position=128, x_position=255; y_direction=0, x_direction=0)
//   ********* | *********
//           * | *
//           * | * (Inside the * is the Threshold Area)
//           * | *
//            y=0--(y_position=0, x_position=128; y_direction=-1, x_direction=0)

BLYNK_WRITE(V0) {
  
  const int X_THRESHOLD_LOW = 108; //x: 128 - 20
  const int X_THRESHOLD_HIGH = 148; //x: 128 + 20   

  const int Y_THRESHOLD_LOW = 108; //y: 128 - 20
  const int Y_THRESHOLD_HIGH = 148; //y: 128 + 20
      
  int x_position = param[0].asInt();  //Read the Blynk Joystick x Position 0-255
  int y_position = param[1].asInt();  //Read the Blynk Joystick y Position 0-255

  int x_direction;  //Variable for Direction of Joystick Movement: x= -1, 0, 1
  int y_direction;  //Variable for Direction of Joystick Movement: y= -1, 0, 1
 
  Serial.print("x_position: ");
  Serial.print(x_position);
  Serial.print("  y_position: ");
  Serial.println(y_position);

//Determine the direction of the Joystick Movement

  x_direction = 0;
  y_direction = 0;

  if (x_position > X_THRESHOLD_HIGH) {
    x_direction = 1;
  } else if (x_position < X_THRESHOLD_LOW) {
    x_direction = -1;
  }
  if (y_position > Y_THRESHOLD_HIGH) {
    y_direction = 1;
  } else if (y_position < Y_THRESHOLD_LOW) {
    y_direction = -1;
  }
//if x and y are within the threshold their values then x_direction = 0 and y_direction = 0

//Move the Rover (Rover will move in the direction of the slower wheels)
//0,0(Stop); 0,1(Forward); 0,-1(Backward); 1,1(Right up diagonal); 1,0(Right); 1,-1(Right down diagonal);
//-1,0(Left); -1,1(Left up diagonal); -1,-1(Left down diagonal)

//**********BACKWARD DIAGONAL (SOFT) LEFT**********
//x = -1 and y = -1 
  if (x_direction == -1) 
    if (y_direction == -1) {
        Serial.print("JOYSTICK: left-down DIRECTION: BACKWARD SOFT LEFT");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction); 

  motorLF->setSpeed(pinValueAdjusted); //Turning Side slower than opposing side
  motorLR->setSpeed(pinValueAdjusted); //Turning Side slower than opposing side
  motorRF->setSpeed(pinValue);
  motorRR->setSpeed(pinValue);
  
        motorLF->run(BACKWARD);
        motorLR->run(BACKWARD);
        motorRF->run(BACKWARD); 
        motorRR->run(BACKWARD);

//**********HARD LEFT TURN**********
//x = -1 and y = 0      
      } else if (y_direction == 0) {
        Serial.print("JOYSTICK: left DIRECTION: HARD LEFT ");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);

  motorLF->setSpeed(pinValue);
  motorLR->setSpeed(pinValue);
  motorRF->setSpeed(pinValue);
  motorRR->setSpeed(pinValue);
        
        motorLF->run(RELEASE);
        motorLR->run(RELEASE);
        motorRF->run(FORWARD);
        motorRR->run(FORWARD);

//FORWARD DIAGONAL (SOFT) LEFT
//x = -1 and y = 1   
      } else {
        //y_direction == 1
        Serial.print("JOYSTICK left-up DIRECTION: FORWARD SOFT LEFT");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);  

  motorLF->setSpeed(pinValueAdjusted); //Turning Side slower than opposing side
  motorLR->setSpeed(pinValueAdjusted); //Turning Side slower than opposing side
  motorRF->setSpeed(pinValue);
  motorRR->setSpeed(pinValue);
  
        motorLF->run(FORWARD);
        motorLR->run(FORWARD);
        motorRF->run(FORWARD);
        motorRR->run(FORWARD);
        
//**********BACKWARD**********
//x = 0 and y = -1 
      } else 
        if (x_direction == 0) 
        if (y_direction == -1) {
        Serial.print("JOYSTICK down DIRECTION BACKWARD");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);

  motorLF->setSpeed(pinValue);
  motorLR->setSpeed(pinValue);
  motorRF->setSpeed(pinValue);
  motorRR->setSpeed(pinValue);
  
        motorLF->run(BACKWARD);
        motorLR->run(BACKWARD);
        motorRF->run(BACKWARD);
        motorRR->run(BACKWARD);

//**********STOP**********
//x = 0 and y = 0 
      } else if (y_direction == 0) {
        Serial.print("JOYSTICK: centered DIRECTION: STOP");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);
        
        motorLF->run(RELEASE);
        motorLR->run(RELEASE);
        motorRF->run(RELEASE);
        motorRR->run(RELEASE);

//**********FORWARD**********
//x = 0 and y = 1 
      } else {
        //y_direction == 1
        Serial.print("JOYSTICK: up DIRECTION: FORWARD");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);

  motorLF->setSpeed(pinValue);
  motorLR->setSpeed(pinValue);
  motorRF->setSpeed(pinValue);
  motorRR->setSpeed(pinValue);
  
        motorLF->run(FORWARD);
        motorLR->run(FORWARD);
        motorRF->run(FORWARD);
        motorRR->run(FORWARD);

//**********BACKWARD DIAGONAL (SOFT) RIGHT**********
//x = 1 and y = -1 
      } else 
        //x_direction == 1
        if (y_direction == -1){ 
        Serial.print("JOYSTICK right-down DIRECTION: BACKWARD SOFT RIGHT");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);

  motorLF->setSpeed(pinValue);
  motorLR->setSpeed(pinValue);
  motorRF->setSpeed(pinValueAdjusted);//Turning Side slower than opposing side
  motorRR->setSpeed(pinValueAdjusted);//Turning Side slower than opposing side
  
        motorLF->run(BACKWARD);
        motorLR->run(BACKWARD);
        motorRF->run(BACKWARD);
        motorRR->run(BACKWARD);

//**********HARD RIGHT TURN**********
//x = 1 and y = 0 Right on x-axis 
      } else 
        if (y_direction == 0){ 
        Serial.print("JOYSITCK: right DIRECTION: HARD RIGHT");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);

  motorLF->setSpeed(pinValue);
  motorLR->setSpeed(pinValue);
  motorRF->setSpeed(pinValue);
  motorRR->setSpeed(pinValue);
  
        motorLF->run(FORWARD);
        motorLR->run(FORWARD);
        motorRF->run(RELEASE);
        motorRR->run(RELEASE); 

//**********FORWARD DIAGONAL (SOFT) RIGHT**********
//x = 1 and y = 1 
      } else { 
        //y_direction == 1
        Serial.print("JOYSTICK: right-up DIRECTION: SOFT RIGHT"); 
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);
        
  motorLF->setSpeed(pinValue);
  motorLR->setSpeed(pinValue);
  motorRF->setSpeed(pinValueAdjusted);//Turning Side slower than opposing side
  motorRR->setSpeed(pinValueAdjusted);//Turning Side slower than opposing side
  
        motorLF->run(FORWARD);
        motorLR->run(FORWARD);
        motorRF->run(FORWARD);
        motorRR->run(FORWARD);      
      }
  }
 //**********END Translate the Joystick Position to a Rover Direction********** 


//**********RIGHT PIROUETTE - SPIN ROUTINE**********    
//PIROUETTE Button V3
BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V3 to a variable
  Serial.print("RIGHT Pirouette: ");
  Serial.println(pinValue);
  if(pinValue == 1) {
    motorLF->run(FORWARD);
    motorLR->run(FORWARD);
    motorRF->run(BACKWARD);
    motorRR->run(BACKWARD);
  }
    
  if(pinValue == 0) {
    motorLF->run(RELEASE);
    motorLR->run(RELEASE);
    motorRF->run(RELEASE);
    motorRR->run(RELEASE);
  }
}
//**********END RIGHT SPIN ROUTINE********** 

//**********LEFT PIROUETTE - SPIN ROUTINE**********    
//PIROUETTE Button V1
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("LEFT Pirouette: ");
  Serial.println(pinValue);
  if(pinValue == 1) {
    motorLF->run(BACKWARD);
    motorLR->run(BACKWARD);
    motorRF->run(FORWARD);
    motorRR->run(FORWARD);
  }
    
  if(pinValue == 0) {
    motorLF->run(RELEASE);
    motorLR->run(RELEASE);
    motorRF->run(RELEASE);
    motorRR->run(RELEASE);
  }
}
//**********END LEFT SPIN ROUTINE********** 


