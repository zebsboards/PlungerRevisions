/***********************************************************************************************************************************
*
*     
*     Zebsboards V7 Plunger Rev 1.0.0 (Board Design Formerly VirtuaPin V4 - Plunger Sensor based on AS5600 rotary magnetic encoder in analog mode)
*     June 2025
*     
*     - Based on ESP32 S2 Mini Module
*     - L1S2DH12 i2c Accelerometer (Nudging)          i2c address 0x18
*     - MCP23017 i2c I/O Expanders (32 Button Input)  i2c address 0x20, 0x21
*     - AT24C256 i2c EEPROM           A0,A1 - GND     i2c address 0x50
*     - Wire1 SDA - 37, SCL - 39
*
*     - accel/Board Orientation - LScableDown = 0, CFcableDown = 1, RScableDown = 2, LScableFlat = 3, CFcableFrontFlat = 4, RScableFlat = 5, CBcableDown = 6, CFcableBackFlat = Default
*           
*     - Wire0 SDA - GPIO33, SCL - GPIO35,  Analog IN - GPIO3(A2)
*
*     - AS5600 sensor has 3.3v to VCC, GND to GND, A2 (D3) pin to OUT, 3.3V to GPO pad (sets mode to analog out) and DIR pad (clockwise rotation decreases count)
*             
*     - C:\Users\zeb\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.1.3\libraries\USB\src\USBHID.cpp
*       Line 200 Gamepad plunger name modified in 3.1.3
*     
*     - C:\Users\zeb\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.14\cores\esp32\esp32-hal-tinyusb
*       Line 645 naming convention string for device entry location in 3.1.3
*       
*      
*     - Deprecated -Volume Knob - Encoder A Pin 39, Encoder B 37
*          
*     
*      
*
*
***********************************************************************************************************************************/

#include "ConfigVariables.h"
#include "pins_arduino.h"
#include <USB.h>
#include <USBHID.h>
#include <USBHIDKeyboard.h>
//#include <USBHIDConsumerControl.h>
#include <elapsedMillis.h>
#include <Wire.h>
#include "Joystick_ESP32S2.h"
#include "at24c256.h"
#include "MCP23017.h"
#include "LIS2DH12.h"

Joystick_ ZBV7(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  32, 0,                                     // Button Count, Hat Switch Count
  true, true, true,                          // X and Y and Z Axis
  false, false, false,                       // No Rx, No Ry, or Rz
  false, false,                              // No rudder or throttle
  false, false, false);                      // No accelerator, brake, or steering
USBHIDKeyboard Keyboard;                     // initialize the Keyboard namespace
//USBHIDConsumerControl VolumeControl;       // Volume control namespace
at24c256 eeprom(0x50);                       // initialize the eeprom chip and address at 0x50 (A0 GND, A1 GND)
MCP23017 mcp1(0x20);                         // initialize the button expander namespaces
MCP23017 mcp2(0x21);                    
LIS2DH12 Nudge;                              // initialize 12c Accelerometer namespace
#define plungerPotPin 3                      // AS5600 Analog Plunger Sensor - Potentiometer Pin
#define Ledpin LED_BUILTIN                   // ESP32 S2 module onboard LED
byte receivedByte;
const int BUFFER_SIZE = 200;
char buf[BUFFER_SIZE];
const int KEY_SIZE = 32;   
elapsedMillis vector;                        // 10ms limitation for reporting 
int vectorVal = 9;                           // 9ms, report at 10ms

void setup() {
  USB.begin();
  Serial.begin(115200);
  Wire.begin();
  eeprom.init();
  initializeVariables();                      // read in values for variables from eeprom
  eeprom.close();
  Wire.setClock(1700000);                     // set i2c frequency to 1700000hz
  initializeButtons();                        // start the button expanders and set the bit mode (16bit)
  initializeNudge();                          // set the accelerometer variables, start the accelerometer & initial calibration
  initializePlunger();                        // set analog resolution, initial variables and states
  initializeController();                     // set axis ranges and start ZBV7 controller
  pinMode(Ledpin,OUTPUT);
  digitalWrite(Ledpin, HIGH);
}

void loop() {
  //long starttime = millis();
  if (vector > vectorVal){
    if (Serial.available()) {
      SerCTRL();                              // check for serial data - commands, switches, etc
    }
    Buttons();                                // scan and report the button states
    if (pull != true && push != true) {       // if plunger isn't moving
      Nudging();                              // scan accelerometer and report the nudging (if any)
    }
    Plunger();                                // scan and report plunger movement (if any)
    vector = 0;
    //long timetorun = millis() - starttime;
    //Serial.print("Total Time Elapsed  ");  Serial.println(timetorun);
  }
  
}

