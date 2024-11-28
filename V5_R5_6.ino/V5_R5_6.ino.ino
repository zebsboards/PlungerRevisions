/*
  Pinball input control software written and developed by Zeb (Steve Ridgley)
                                     http://www.zebsboards.com

  *************************************************************************************************************************************
      Tested on very little with the high possibility that it may work under Visual Pinball, PBFX2, TPA, PinMame and Future Pinball
      Written and developed under the influence; specifically beer.
      Recompling is unnecessary for making it work.. compiles everytime....doesn't mean it works.
  *************************************************************************************************************************************

  V4Rev5_5 March 2022 Revision

  Revisions to software include:

  - Added provision for mcp23018 expander at 3.4mhz transmission speed (address 2)
  - Removed mcp23017 expander (address 1)
  - Removed 8 sec initialization delay
 
*/


#include <Keyboard.h>
#include <Pincab_Controller_V2HB.h>
#include <HID.h>
#include <ResponsiveAnalogRead.h>
#include <EEPROM.h>
#include "SparkFun_LIS2DH12.h"
#include "Adafruit_MCP23017.h"
#include <Wire.h>
#include <elapsedMillis.h>

//#include "Tlc5940.h"

// Define Plunger INPUT Variables

int NEW_Pos = 0;       // movement reading on the z axis
int OLD_Pos = 0;       // last count of plunger position
int Push_Flag = 0;      // flag to indicate slam event
int Pull_Flag = 0;      // flag to indicate plunger event

int Coin_Door = 0; // Coindoor Open State indicator '0' = Closed , '1' = Open


// Define Serial Control Variables

byte receivedByte;
const int BUFFER_SIZE = 64;
char buf[BUFFER_SIZE];
const int KEY_SIZE = 32;

int Gamepadflag = 1;      // gamepad mode status flag  0 = OFF  1 = ON  start system in default ON (1) Gamepad setting

int zblaunchflag = 0;     // digital / analog plunger status flag  0 = OFF   1 = ON  system starts in OFF (0) setting

int EXT_CAL = 0;          // external calibration loop indicator flag 0 = OFF  1 = ON

int d1 = 0;
int d2 = 0;               // variables for direct input button state conditions
int d3 = 0;

// Define I2c variables

int i2c_present = 0;      // slave board connected = 1 or higher, no slave = 0

SPARKFUN_LIS2DH12 accel;      //Accelerometer
#define ACCEL_ADDRESS 0x18   // address the accelerometer

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;   // address the expanders

#define addr1 0
#define addr2 2           // define the address variables used

// Define Plunger OUTPUT variables

int pl_z;               // z (plunger) axis output variable
int pl_zrot;            // inverted z (plunger) axis variable

// Assign Auto Leveling and Nudging variables

int counter = 0;        // sampling counter start
int counter_end = 101;   // sampling counter duration
int x_read = 0;         // x axis sampling running total
int x_calib = 0;        // x axis final calibration value
int accelX = 0;          // x axis reference value
int x_rest = 0;         // x axis calibrated rest (0) point
int x_out = 0;          // x axis final output value
int y_read = 0;         // y axis sampling running total
int y_calib = 0;        // y axis final calibration value
int accelY = 0;          // Y axis reference value
int y_rest = 0;         // Y axis calibrated rest (0) point
int y_out = 0;          // Y axis final output value
int z_init = 0;         // z axis sampling data
int z_read = 0;         // z axis sampling running total
int z_calib = 0;        // z axis final calibration value
int z_centered = 0;     // z axis offset value
int z_min = 0;          // z axis minimum depressed
int z_out = 0;          // z axis final output value
int z_max = 0;          // z axis maximum drawback
int z_released = 0;     // z axis released flag
int zminSet = 0;        // push length set by plunger calibration routine *********************************** zminSet
int z_rest = 0;         // calibrated at rest (0) point
int zmaxSet = 182;      // pull length set by plunger calibration routine *********************************** zmaxSet
int rel_timer;
elapsedMillis updatetimer;

int tilt_RAW;           // RAW reading of analog Tilt control
int tilt_VAL    ;       // TILT value setting at which tilt occurs
int tilt_button = 17;   // TILT Button assignment - assigned by position in keys array
int tilt_enable = 1;    // TILT enabled/disabled flag default ON

float nGain = 0.00;      // Gain adjustment value for nudging
float nGainFinal = 0.00; // Final Gain adjustment value

elapsedMillis tiltTime;

ResponsiveAnalogRead analog_X(A0, true);
ResponsiveAnalogRead analog_Y(A1, true);
ResponsiveAnalogRead analog_Z(A4, true);
ResponsiveAnalogRead analog_G(A3, true);
ResponsiveAnalogRead analog_T(A2, true);

//******************************* Calibration Switch / Indicator LED / DPad **************************************

int CALSW;
int CALflag=0;
elapsedMillis CswTimer;
elapsedMillis BlinkTimer;

int LEDOUT;
int BlinkMode;
elapsedMillis BlinkModeTimeoutTimer;

int dp_in[8]={A5,7,13,5,10,9,8,4};
int dp_out[9]={0,1,3,5,7,1,3,5,7};
int dp_count = 0;
int dp1 = 0;
int dp2 = 0;

//*************************************** Keystroke Array *************************************************

byte CD_VAL = 0xD5;
byte EX_VAL = 0x71;
byte LB_VAL = 0xB0;
int LB_Pos;
int lengthofkeys;
int bcount;
int count;
int key_flag = 0;
int keys[32] = {                         // default original plunger keystroke assignment and order
  0x81, // LShift  Left Flipper               Button 1
  0x80, // LCTRL   Left MSave                 Button 2  
  0x31, // 1       Start                      Button 3
  14,   // q       Exit 0x71                  Button 4
  0x32, // 2       Extra Buy-in               Button 5
  0x35, // 5       Coin                       Button 6
  12,   // END     Coin Door Open/Close       Button 7  
  0x3D, // =       Volume DOWN                Button 8
  0x2D, // -       Volume UP                  Button 9
  0x38, // 8       SMenu Down                 Button 10
  0x39, // 9       SMenu UP                   Button 11
  0x30, // 0       SMenu Enter                Button 12
  0x37, // 7       SMenu Cancel               Button 13
  0x85, // RShift  Right Flipper              Button 14
  0x84, // RCTRL   Right MSave                Button 15
  11,   // Enter   Launch/Fire                Button 16
  10,   // Tilt Button Assignment             Button 17
};
int keysNew[32] = {                         // default new plunger keystroke assignment and order
  0x81, // LShift  Left Flipper               Button 1
  0x80, // LCTRL   Left MSave                 Button 2  
  0x31, // 1       Start                      Button 3
  14,   // q       Exit 0x71                  Button 4
  0x35, // 5       Coin                       Button 5
  0x85, // RShift  Right Flipper              Button 6
  0x84, // RCTRL   Right MSave                Button 7
  11,   // Enter   Launch/Fire                Button 8
  10,   // Tilt Button Assignment             Button 9
};

int pl_orig[]={4,6,8,9,10,5,13,7,A5,1,0,17,15,16,14,11,2,3,};
int pl_new[]={4,8,9,10,5,13,7,A5,1,0,17};
int pl_use[18];
int pincount;
  

//*****************************************  Output Variables  *********************************************

//int OPvalue[32];
//byte OPData;

//*********************************************** Start Setup ***************************************************

void setup() {

  Serial.begin(115200);

  analogReference(EXTERNAL);         // force 3.3v reference voltage for analog signals

  pinMode(1, OUTPUT);
        
// *********************************************** I2C setup *************************************************************
  
  Wire.begin();                      // Wire communication begin (scanning use only)
  Wire.setClock(1700000);            // set i2c frequency to 1700000hz
  delay(10);

//************ I2c Bus Scan *************************************
  
  digitalWrite(1, HIGH);      // Turn on power (5V to Expansion Board)
  //delay(8000); 
  for (byte i = 8; i < 120; i++) {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0) {
      i2c_present++;
      delay (1);
    }
  }
   
  switch (i2c_present) {

    
    case 0:                                // NO i2c device - Original Plunger Design
        
        //************************* accelerometer setup ******************************

        Wire.end();                      // Wire communication ended
        Pins();
        pincount = 18;
        for (int a = 0; a < pincount; a++) {
          pl_use[a] = pl_orig[a];
        }
        KeyAssign();
        analogRead(A0);
        analogRead(A1);                    // intial warm up of analog devices
        analogRead(A2);
        analogRead(A3);
        analogRead(A4);
        break;
        
    case 1:                                // 1 i2c device - assume accelerometer only
        
        //************************* accelerometer setup ******************************

        Pins();
        pincount = 11;
        LEDOUT = A0;
        CALSW = 6;
        for (int a = 0; a < pincount; a++) {
          pl_use[a] = pl_new[a];
        }
        KeyAssign();
                
        accel.begin(ACCEL_ADDRESS, Wire);   // start accelerometer at defined address (0x18)
        break;
      
    case 2:                                // 2 i2c devices - analog accelerometer - Expansion Board only

        //************************* accelerometer setup ******************************

        Pins();
        CALSW = 6;
        KeyAssign();
        analogRead(A0);
        analogRead(A1);                    // intial warm up of analog devices
        analogRead(A2);
        analogRead(A3);
        analogRead(A4);       
        //************************* expansion board setup ****************************

        mcp1.begin(addr1, &Wire);          // switch to mcp23017 library usage
        mcp2.begin(addr2, & Wire);
        for (int a = 0; a < 16; a++) {
          mcp1.pinMode(a, INPUT);          // pin 21 on chip
          mcp1.pullUp(a, HIGH);            // turn on a 100K pullup internally
          mcp2.pinMode(a, INPUT);          // pin 21 on chip
          mcp2.pullUp(a, HIGH);            // turn on a 100K pullup internally
        }
        delay(100);
        break;
       
    case 3:                         // 3 i2c devices - newer plunger - all 3 devices present

        //************************* accelerometer setup ******************************

        Pins();
        LEDOUT = A0;
        CALSW = 6;
        KeyAssign();        
        accel.begin(ACCEL_ADDRESS, Wire);   // start accelerometer at defined address (0x18)

        //************************* expansion board setup ****************************

        mcp1.begin(addr1, &Wire);         // switch to mcp23017 library usage
        mcp2.begin(addr2, & Wire);
        for (int a = 0; a < 16; a++) {
          mcp1.pinMode(a, INPUT);          // pin 21 on chip
          mcp1.pullUp(a, HIGH);            // turn on a 100K pullup internally
          mcp2.pinMode(a, INPUT);          // pin 21 on chip
          mcp2.pullUp(a, HIGH);            // turn on a 100K pullup internally
        }
        delay(100);
        break;
  }

  autoleveling();                    // Autoleveling at power up

  Pincab_Controller_V2HB.begin();    // Initialize Pincab_Controller Library

  Keyboard.begin();                  // Initialize Keyboard Library
   
  tilt_enable = EEPROM.read(65);     // read TILT enable state from eeprom memory (1 = ON, 0 = OFF)
  LB_VAL = EEPROM.read(66);          // read Launch Ball Key Position in array from memory
  zmaxSet = EEPROM.read(67);          //read stored value of plunger max pull length
  zminSet = EEPROM.read(68);          //read stored value of plunger max push length
  CD_VAL = EEPROM.read(69);           //read stored value of coin door keypress
  EX_VAL = EEPROM.read(70);
  
  //  OUTPUT Initialization if available********************************************************

  //  Read a digital input and verify board is present by connecting pin to ground through harness connecting boards
  //  would also be a valid way of selecting i2c bus being initialized

  //Tlc.init(0); // Initiates the TLC5940 and set all channels off

  SetBlinkMode(0);                  // start led blinking slowly

}
//********************************************************************************************************
//********************************************End Setup **************************************************


//*********************************************************************************************************
//******************************************** Main Code **************************************************

void loop() {

  Blink();

  if (updatetimer > 9){
      // Set the nudge position (x axis, y axis)
      Pincab_Controller_V2HB.setXAxis(x_out);
      Pincab_Controller_V2HB.setYAxis(y_out);
      // Set the plunger state (position z axis, z rotation)
      Pincab_Controller_V2HB.setZAxis(pl_z);
      Pincab_Controller_V2HB.setZAxisRotation(pl_zrot);
      Pincab_Controller_V2HB.sendState();     // Send out the data over the USB connection
  }
  if (Serial.available()) {
    SerCTRL();           // check for serial data
  }

  Direct_Input_Control();  // check for manual switching - Calibration, Gamepad/Keyboard mode change

  if (i2c_present > 1){
   ButtonI2c();            // Read Button State on I2C Button Controller
  }
  else {
   Button(); 
  }
  analog_Z.update();  // read the raw plunger data
  NEW_Pos = map(analog_Z.getValue(),0,1024,0,255);  // use the smoothed value 0-1024 (10bit) brought down to 0-255 scale (8bit)

  // Apply Plunger Calibration

  if ((NEW_Pos > z_centered + 10) || (NEW_Pos < z_centered - 10)) {    //deadzone to accomodate plunger fluctuation at static rest

    NEW_Pos = NEW_Pos + z_calib ;                                     //send the realtime readings
  }
  else  {
    NEW_Pos = z_rest;                                //send the center position
  }


  // Plunger as Digital Button

  if (zblaunchflag > 0)  {  // if the input at pin 1 is Low or serial character from pinballx is 1 use Digital Ball Launch

    DigitalballLaunch();
  }

  // Plunger as Analog Plunger
  else {

    if (NEW_Pos != OLD_Pos) {  // if plunger position has changed
      x_out = x_rest;       // send null values
      y_out = y_rest;
      AnalogPlunger();  // calculate the plunger movement and position
    }
  }

  if ((Push_Flag < 1) && (Pull_Flag < 1)) {  // if plunger not in use
    
    NudgingRoutine();   // calculate cabinet movement and set state in subroutine NudgingRoutine
    
  }
  if ((Push_Flag > 0) && (Pull_Flag > 0)) {  // if plunger is in use

    x_out = x_rest;       // send null values
    y_out = y_rest;

  }
  

  // Plunger Reset

  if ((NEW_Pos == OLD_Pos) && (z_released > 0)) {

    if (rel_timer > 25) {  // reset all values to 0 after plunger movement complete

      //NEW_Pos = 0;
      //OLD_Pos = 0;
      z_centered = OLD_Pos;
      pl_z = z_rest;
      pl_zrot = z_rest;
      Push_Flag = 0;
      Pull_Flag = 0;
      z_max = 0;
      z_released = 0;
      rel_timer = 0;
      counter = 0;
    }
    rel_timer = (rel_timer + 1);
  }

  OLD_Pos = NEW_Pos;    // update the reference position

  
  
}













