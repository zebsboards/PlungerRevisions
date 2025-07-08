/***********************************************************************************************************************************
*
*     VirtuaPin-Zebsboards Collaborative Plunger R1.0038
*     August 2024
*     
*     - Based on ESP32 S2 Mini Module
*     - L1S2DH12 i2c Accelerometer (Nudging)          i2c address 0x18
*     - MCP23017 i2c I/O Expanders (32 Button Input)  i2c address 0x20, 0x21
*     - VCNL4040 i2c Proximity Detector (Plunger)     i2c address 0x60
*     - AT24C256 i2c EEPROM           A0,A1 - GND     i2c address 0x50
*     
*     - accelOrientation - LScableDown = 0, CFcableDown = 1, RScableDown = 2, LScableFlat = 3, CFcableBackFlat = 4, RScableFlat = 5, CBcableDown = 6, CFcableFrontFlat = Default
*           
*     - plunger sensor typte switch determines analog pot (1) or IR sensor (0/default) 
*      
*     - Wire0 SDA - GPIO33, SCL - GPIO35, Wire1 SDA - 37, SCL - 39, Analog IN - GPIO3(A2), Indicator DATA OUT - GPIO7, plungerIntPin - GPIO40
*           
*     - C:\Users\zeb\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.14\libraries\USB\src\USBHID.cpp
*       Line 203 Gamepad plunger name modified / 200 in 3.0.4
*     
*     - C:\Users\zeb\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.14\cores\esp32\esp32-hal-tinyusb
*       Line 559 naming convention string for device entry location / 645 in 3.0.4
*       
*     - C:\Users\zeb\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.14\cores\esp32\usb.cpp
*       Lines 23 - 45 USB Descr. info (name, vendor, manufact, vid, pid)
*      
*     - Deprecated -Volume Knob - Encoder A Pin 39, Encoder B 37
*          
*     - using board core 3.1.3, adafruit neopixel 1.12.3
*      
*
*
***********************************************************************************************************************************/
//#include "pins_arduino.h"
#include "USB.h"
#include "USBHIDKeyboard.h"
//#include "USBHIDConsumerControl.h"
#include "USBHIDGamepad.h"
//#include <Encoder.h>
#include "at24c256.h"
#include "LIS2DH12.h"
#include "VCNL4040.h"
#include "MCP23017.h"
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <elapsedMillis.h>

    /********************************************************************* Hardware Defines *********************************************************************************************************/
LIS2DH12 accel;                              // 12c Accelerometer
MCP23017 mcp1(0x20);                         // initialize the button expander namespaces
MCP23017 mcp2(0x21);                    
USBHIDKeyboard Keyboard;                     // initialize the Keyboard namespace
//USBHIDConsumerControl VolumeControl;         // Volume control namespace
USBHIDGamepad Joystick;
at24c256 eeprom(0x50);                       // initialize the eeprom chip and address at 0x50 (A0 GND, A1 GND)
VCNL4040 plungerSensor;                      // initialize the plunger sensor namespace
#define WSLEDPin 7                           // Data Pin for WS2812 Indicator Strip
Adafruit_NeoPixel plungerIndicator = Adafruit_NeoPixel(15, WSLEDPin, NEO_GRB + NEO_KHZ800);
//Encoder VolumeKnob(39, 37);                  // Volume knob encoder library namespace and pin assignment 
#define Ledpin LED_BUILTIN                   // ESP32 S2 module onboard LED
#define plungerPotPin A2                     // Plunger Sensor - Potentiometer Pin
    /**************************************************************** Serial Control Variables ******************************************************************************************************/
byte receivedByte;
const int BUFFER_SIZE = 200;
char buf[BUFFER_SIZE];
const int KEY_SIZE = 32;    
    /************************************************* Global Configuration Variables (CONFIGURED EXTERNALLY)****************************************************************************************/
byte    GamepadFlag;                         // Gamepad/Keyboard Mode Switch  0 = Keyboard Mode / 1 = Gamepad Mode                          -  8bit EEPROM VALUE / Switchable by BAT command file
byte    plungerIndicatorMode;                // Plunger Visual display mode (0 = OFF, 1 = Lit to Black, 2 = Black to lit)                   -  8bit EEPROM VALUE
byte    indBrightness;                       // Plunger Indicator Brightness Setting (5 - 100)                                              -  8bit EEPROM VALUE
byte    plungerDeadzone;                     // Value of deadzone for plunger movement                                                      -  8bit EEPROM VALUE
byte    accelOrientation;                    // Mounting Orientation of the board in cabinet                                                -  8bit EEPROM VALUE
byte    accelScale;                          // 2g/4g/8g/16g selection value (sensitivity)                                                  -  8bit EEPROM VALUE
byte    XGain;                               // Nudge Gain Setting                                                                          -  8bit EEPROM VALUE
byte    YGain;                               // Nudge Gain Setting                                                                          -  8bit EEPROM VALUE
byte    tilt_enable;                         // TILT enabled/disabled flag default ON                                                       -  8bit EEPROM VALUE
byte    LaunchBallButton;                    // LB Button assignment - denotes in KeyCommands Array                                         -  8bit EEPROM VALUE
byte    useHatSwitch;                        // 32 Button 0 Hat Switch / 24 Button 2 Hat Switch Flag Bit                                    -  8bit EEPROM VALUE
byte    CD_VAL = 0xD5;                       // Coin Door Keystroke                                                                         -  8bit EEPROM VALUE
byte    LBkey;                               // LB Keyboard Code (0x10 is Enter Key)                                                        -  8bit EEPROM VALUE
byte    plungerSenseType;                    // IR or Potentiometer Sensor being read by plunger   (0 - disabled, 1 - IR, 2 - Pot)          -  8bit EEPROM VALUE 
byte    tilt_button;                         // TILT Button assignment - denotes position in keys array                                     -  8bit EEPROM VALUE
byte    trackballFlag;                       // Enable/Disable TrackBall Usage                                                              -  8bit EEPROM VALUE
byte    CD_Pos;                              // CD Button assignment (actual use value)                                                     -  8bit EEPROM VALUE
byte    KeyCommands[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};       //                         - 32bit EEPROM VALUE
byte    LabelCodes[]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};       //                           - 32bit EEPROM VALUE                                               
int16_t plungerDrawLength;                   // Value of Maximum Draw Length                                                                - 16bit EEPROM VALUE
int16_t plungerPushLength;                   // Value of Maximum Push Length                                                                - 16bit EEPROM VALUE
int16_t Xdeadzone;                           // x axis deazone                                                                              - 16bit EEPROM VALUE
int16_t Ydeadzone;                           // y axis deazone                                                                              - 16bit EEPROM VALUE
int16_t tilt_VALX;                           // TILT value setting at which tilt occurs                                                     - 16bit EEPROM VALUE
int16_t tilt_VALY;                           // TILT value setting at which tilt occurs                                                     - 16bit EEPROM VALUE
   /*************************************************************Global Variables (INTERNAL USAGE) **************************************************************************************************/
int8_t  zblaunchflag;                        // Digital/Analog plunger status flag  0 = OFF   1 = ON  system starts in OFF (0) setting      - Switchable by BAT command file
int8_t  plungerIsInMotion;                   // Indicator that Flags a Plunger Event is already in Progress (0 - OFF, 1 - Push Event, 2 Pull Event)
int8_t  accelEnabled = 1;                    // Flag for accelerometer usage
int16_t plungerAtRest;                       // Plunger z axis final offset value (autozeroing)
int8_t  numberOfLeds;                       // Leds to be lit in the Indicator Strip (based off plungerRead)
int16_t plungerRead;                         // Current Reading of Plunger Position by Sensor
int16_t x_offset;                            // x axis final offset value (accelerometer autozeroing)
int16_t y_offset;                            // y axis final offset value (accelerometer autozeroing)
int16_t z_offset;                            // z axis final offset value (accelerometer autozeroing)
elapsedMillis tiltTime;                      // TILT activation debounce timer
int16_t plungerTimeOut;                      // Plunger Reset Timer
int8_t  pauseOnSerial;                       // Control bit to pause plunger routines if serial received (other than output data)
   /****************************************************************** Start of Setup ****************************************************************************************************/
void setup() {
   Serial.begin(115200);
   delay(5);
   pinMode(Ledpin,OUTPUT);
   /****************************************************************** Security Scan / Board Verification ****************************************************************************************************/
   Wire.begin();                               // Wire communication begin (scanning use only)
   byte error, address;
   int nDevices = 0;
   for(address = 0x01; address < 0x7f; address++)
   {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      if (error == 0){
        nDevices++;
      }
   }
   if (nDevices < 3)
   {
    Serial.println("BOARD ERROR!!! BOOT HALTED");
    for(;;)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
   }
   /****************************************************************** i2c Frequency Initialization ****************************************************************************************************/ 
   Wire.setClock(1700000);                     // set i2c frequency to 1700000hz
   delay(5);
   /******************************************************************* Button i/o Expanders Initialization ******************************************************************************************/
   mcp1.begin(true);                           // switch to mcp23017 library usage
   mcp2.begin(true);
   mcp1.pinMode16(0xFFFF);
   mcp2.pinMode16(0xFFFF);
   USB.begin();
   /********************************************************************** Plunger Initialization  VALUES TO BE READ FROM EEPROM**********************************************************************/
   BootConfig();                                                                  // Read stored values and assign to Control Variables
   /******************************************************************** Plunger IR Sensor Initialization ********************************************************************************************/
   if (plungerSenseType == 1)
   {
     plungerSensor.begin();
     plungerSensor.powerOnProximity();
     plungerSensor.setProxResolution(16);
     plungerSensor.setLEDCurrent(200);
     plungerSensor.setIRDutyCycle(40);
   }
   Keyboard.begin();
   //VolumeControl.begin();
   Joystick.begin();
   /******************************************************************* Accelerometer Initialization *************************************************************************************************/
   switch (accelScale)
   {
    case 0: // disabled
      accelEnabled = 0;
      break;
    case 2:
      accel.begin();                              // start accelerometer at defined address (0x18)
      accel.setScale(LIS2DH12_2g);                // set accelerometer tracking rate
      accel.setMode(LIS2DH12_HR_12bit);           // set accelerometer resolution
      accel.setDataRate(LIS2DH12_ODR_5kHz376_LP_1kHz344_NM_HP);
      break;
    case 4:
      accel.begin();                              // start accelerometer at defined address (0x18)
      accel.setScale(LIS2DH12_4g);                // set accelerometer tracking rate
      accel.setMode(LIS2DH12_HR_12bit);           // set accelerometer resolution
      accel.setDataRate(LIS2DH12_ODR_5kHz376_LP_1kHz344_NM_HP);
      break;
    case 8:
      accel.begin();                              // start accelerometer at defined address (0x18)
      accel.setScale(LIS2DH12_8g);                // set accelerometer tracking rate
      accel.setMode(LIS2DH12_HR_12bit);           // set accelerometer resolution
      accel.setDataRate(LIS2DH12_ODR_5kHz376_LP_1kHz344_NM_HP);
      break;
    case 16:
      accel.begin();                              // start accelerometer at defined address (0x18)
      accel.setScale(LIS2DH12_16g);               // set accelerometer tracking rate
      accel.setMode(LIS2DH12_HR_12bit);           // set accelerometer resolution
      accel.setDataRate(LIS2DH12_ODR_5kHz376_LP_1kHz344_NM_HP);
      break;        
   }
   /******************************************************************************** Automated Initial Calibration ***********************************************************************************/
   if (plungerIndicatorMode != 0 && plungerSenseType != 0)
   {
      plungerIndicator.begin();
      plungerIndicator.setBrightness(indBrightness);                              // Plunger Indicator Brightness Set
      plungerIndicator.clear();
      ClearIndicator();
      plungerIndicator.show();                                                    // Plunger Lightstrip Reset Function
   }
   digitalWrite(Ledpin, HIGH);
   autozeroing();                                                                 // Automated zeroing of accelerometer and creation of plungerAtRest function
}
void loop() 
{
  //Volume();
  if (Serial.available()) 
  {
    SerCTRL();                                                                    // check for serial data
  }
  while (pauseOnSerial > 0){}                                                    // Serial Event on other core trips pause flag and suspends plunger, nudge and button read 
  //long StartTime = millis();
  Buttons();  
  //Serial.print(millis() - StartTime); Serial.println("ms Buttons");  
  //StartTime = millis();                                                               // Output data reception does not interrupt plunger, nudge and button read routines
  if (plungerSenseType != 0)
  {
    Plunger();
  }
  //Serial.print(millis() - StartTime); Serial.println("ms Plunger");
  //StartTime = millis();
  if (accelEnabled != 0 && plungerIsInMotion == 0)
  {
    Nudge();
  }
  else
  {
   Joystick.leftStick(0 , 0);                                                         // Send NULL position values (x axis, y axis)
  }
  //Serial.print(millis() - StartTime); Serial.println("ms Nudge");
  //Serial.println("");
  delay(5);
}
