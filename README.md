# Arduino-Blynk-Joystick-4-Motor-Robot-Rover
The code in sketch Four_Motor_JOYSTICK_Blynk_BLE_c was developed to operate a four motor Rover-Robot with an Arduino 101 and 
 Adafruit Motor-Servo Controller Shield using the Blynk iPhone app using the Blynk "Joystick" and Blynk
 "Long Slider" over a BLE Bluetooth connection. This code should work for a 2 motor Rover. You will have to 
 designate the motors appropriately in the Define Motor Port Section ~line 43. 

 Note: This code generates an error "Blynk.begin(auth, blePeripheral);"
                                                                    ^
 during compliation. It doesn't seem to be an issue.
 
 The other sketch in the repository: Four_Motor_Arduino_101_Blynk_BLE  will enable the operation of the Rover-Robot with seven 
 Blynk Pushbuttons instead of the Joystick. 
 
 All of the sketch's presented here were inspired by "Intel Arduino 101 BLE Blynk Tank" posted on Electron 
 Hacks - Thanks! https://goo.gl/gvvyVh
 
 The sketch Four_Motor_JOYSTICK_Blynk_BLE_D updates the steering logic. The long slider speed control is removed and the speed 
 is set to a constant 255. The diagonal turns occur by operating the turning side motors at 50% of the speed of the opposing 
 motors. The "hard" turns occur by releasing the turning side motors.Two pushbutton switches are added that cause the Rover to 
 perform a left or right Pirouette spin.
 
 
 Use this code at your own risk! 
