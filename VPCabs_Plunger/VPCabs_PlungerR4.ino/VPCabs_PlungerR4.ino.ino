
/*
  Pinball input control software written and developed by Zeb (Steve Ridgley)
                                     http://www.zebsboards.com

  *************************************************************************************************************************************
      Tested on very little with the high possibility that it may work under Visual Pinball, PBFX2, TPA, PinMame and Future Pinball
      Written and developed under the influence; specifically beer.
      Recompling is unnecessary for making it work.. compiles everytime....doesn't mean it works.
  *************************************************************************************************************************************

  V4Rev2_December 2020 Revision
  
  Revisions to software include:

 - Implemented plunger calibration routine through combined Left Flipper/Start button press

 - Refined digital ball launch routine, added Gamepad Button 32 as virtual button

 - Refined Nudging and Plunger reporting routine

 - Refined the Tilt timing routine, removed the potentially troublesome 't' stuck keypress, moved Tilt button to Button 31

 - Fixed/Added coin door single click in gamepad mode

 - Added physical Gamepad/Keyboard mode switching through combined Right Flipper/Start Button press
 
*/



#include <Keyboard.h>
#include "Pincab_ControllerVPC.h"
#include <Bounce.h>
#include <HID.h>
#include <ResponsiveAnalogRead.h>
#include <EEPROM.h>



// Define Plunger INPUT Variables

int NEW_Pos = 0;       // movement reading on the z axis
int OLD_Pos = 0;       // last count of plunger position
int Push_Flag = 0;      // flag to indicate slam event
int Pull_Flag = 0;      // flag to indicate plunger event

int Coin_Door = 0; // Coindoor Open State indicator '0' = Closed , '1' = Open


// Define Serial Control Variablest

String readString; // variable for Serial Solenoid Command Serial commands

int Gamepadflag = 0;  // gamepad mode status flag  0 = OFF  1 = ON  start system in default OFF (0) setting

int zblaunchflag = 0;     // digital / analog plunger status flag  0 = OFF   1 = ON  system starts in OFF (0) setting

int EXT_CAL = 0;          // external calibration loop indicator flag 0 = OFF  1 = ON




// Define Plunger OUTPUT variables

int pl_z;               // z (plunger) axis output variable
int pl_zrot;            // inverted z (plunger) axis variable


// Assign Auto Leveling and Nudging variables

int counter = 0;        // sampling counter start
int counter_end = 30;   // sampling counter duration
int buffer = 0;         // accelerometer initialization start
int buffer_end = 100;    // accelerometer initialization duration
int x_init = 0;         // x axis sampling data
int x_read = 0;         // x axis sampling running total
int x_calib = 0;        // x axis final calibration value
int x_centered = 0;     // x axis offset value
int x_ref = 0;          // x axis reference value
int x_raw = 0;          // x axis raw smoothed value
int x_rest = 0;         // x axis calibrated rest (0) point
int x_out = 0;          // x axis final output value
int y_init = 0;         // y axis sampling data
int y_read = 0;         // y axis sampling running total
int y_calib = 0;        // y axis final calibration value
int y_centered = 0;     // y axis offset value
int y_ref = 0;          // Y axis reference value
int y_raw = 0;          // y axis raw smoothed value
int y_rest = 0;         // Y axis calibrated rest (0) point
int y_out = 0;          // Y axis final output value
int z_init = 0;         // z axis sampling data
int z_read = 0;         // z axis sampling running total
int z_calib = 0;        // z axis final calibration value
int z_centered = 0;     // z axis offset value
int z_ref = 0;          // z axis reference value
int z_out = 0;          // z axis final output value
int z_max = 0;          // z axis maximum drawback
int z_released = 0;     // z axis released flag
int zminSet = 0;     // push length set by plunger calibration routine *********************************** zminSet
int z_rest = 0;         // calibrated at rest (0) point
int zmaxSet = 0;      // pull length set by plunger calibration routine *********************************** zmaxSet
int rel_timer;

int tilt_RAW;           // RAW reading of analog Tilt control
int tilt_VAL = 0;       // TILT value setting at which tilt occurs

float nGain = 0.00;     // Gain adjustment value for nudging
float nGainFinal = 0.00; // Final Gain adjustment value

unsigned long tilt_time;      // Sensor reading for setting tilt_VAL

ResponsiveAnalogRead analog_X(A0, true);
ResponsiveAnalogRead analog_Y(A1, true);
ResponsiveAnalogRead analog_Z(A4, true);
ResponsiveAnalogRead analog_G(A3, true);
ResponsiveAnalogRead analog_T(A2, true);

char END = KEY_END;

//*************************************** Keystroke Array *************************************************

/*int keys[] = {
  0x81, // LShift  Left Flipper
  0x80, // LCTRL   Left MSave
  0x31, // 1       Start
  0x71, // q       Exit
  0x32, // 2       Extra Buy-in
  0x35, // 5       Coin
  END,  // END     Coin Door Open/Close
  0x3D, // =       Volume DOWN
  0x2D, // -       Volume UP
  0x38, // 8       SMenu Down
  0x39, // 9       SMenu UP
  0x30, // 0       SMenu Enter
  0x37, // 7       SMenu Cancel
  0x85, // RShift  Right Flipper
  0x84, // RCTRL   Right MSave
  0xB0, // Enter   Launch/Fire 
  12,   // Gamepad setbutton 12 (GP Button1)
  11    // Gamepad setbutton 11 (GP Button2)
};*/

//*************************************** Debounce (5ms) **************************************************

Bounce button0 = Bounce(4, 5);    // Left Flipper         (L_Arrow)    
Bounce button1 = Bounce(6, 5);    // Left Magnasave       (L_CTRL)     
Bounce button2 = Bounce(8, 5);    // Start                (1)          
Bounce button3 = Bounce(9, 5);    // Exit                 (ESC)         
Bounce button4 = Bounce(10, 5);   // Coin   (5)           (5)
Bounce button5 = Bounce(5, 5);    // Right Flipper        (R_Arrow)       
Bounce button6 = Bounce(13, 5);   // Right Magnasave      (R_CTRL)    
Bounce button7 = Bounce(7, 5);    // Launch Ball          (0xB0 Enter)          
Bounce button8 = Bounce(A5, 5);   // Gamepad Button 1
Bounce button9 = Bounce(1, 5);    // Gamepad Button 2
Bounce button10 = Bounce(0, 5);   // Gamepad Button 3
Bounce button11 = Bounce(2, 5);   // Gamepad Button 4
Bounce button12 = Bounce(3, 5);   // Gamepad Button 5
Bounce button13 = Bounce(17, 5);  // Gamepad Button 6
Bounce button14 = Bounce(15, 5);  // Gamepad Button 7
Bounce button15 = Bounce(16, 5);  // Gamepad Button 8
Bounce button16 = Bounce(14, 5);  // Gamepad Button 9
Bounce button17 = Bounce(11, 5);  // Gamepad Button 10



//***********************************************Setup ***************************************************

void setup() {

  
  analogReference(EXTERNAL);         // force 3.3v reference voltage for analog signals
  analogRead(A4);
  analogRead(A0);
  analogRead(A1);                    // intial warm up of analog devices
  analogRead(A2);
  analogRead(A3);

  tilt_time = 0;                     // set timer to 0

  setupPins();                       // set pinmode to pullup (LOW trigger)

  autoleveling();                    // Autoleveling at power up

  Pincab_ControllerVPC.begin();      // Initialize Pincab_Controller Library

  Keyboard.begin();                  // Initialize Keyboard Library

  //Serial.begin (9600);               // serial control initialization

  zmaxSet = EEPROM.read(0);          //read stored value of plunger max pull length
  if(zmaxSet < 1){                   //if calibration not run
    zmaxSet = 182;                   //default value to get up and running 
  }
  zminSet = EEPROM.read(1);          //read stored value of plunger max push length
  if(zminSet > 1){                   //if calibration not run
    zminSet = 0;                  //default value to get up and running 
  }
  

}

//******************************************** Main Code **************************************************

void loop() {

  Pincab_ControllerVPC.sendState();     // Send out the data over the USB connection

  //SerCTRL();           // check for serial data

  Direct_Input_Control();  // check for manual switching - Calibration, Gamepad/Keyboard mode change
    
  Buttoninput();       // read buttons and set state in subroutine Buttoninput
  
  analog_Z.update();  // read the raw plunger data

  NEW_Pos = (analog_Z.getValue()/4);    // use the smoothed value 0-1024 (10bit) brought down to 0-255 scale (8bit)
  
  
  // Apply Plunger Calibration

  if ((NEW_Pos > z_centered + 2) || (NEW_Pos < z_centered - 2)) {    //deadzone to accomodate plunger fluctuation at static rest

    NEW_Pos = NEW_Pos + z_calib ;                                     //send the realtime readings
  }
  else  {
    NEW_Pos = z_rest;                                //send the center position
  }


 /* // Plunger as Digital Button

  if (zblaunchflag > 0)  {  // if the input at pin 1 is Low or serial character from pinballx is 1 use Digital Ball Launch

    DigitalballLaunch();
  }

  // Plunger as Analog Plunger
  else {*/
    
    if (NEW_Pos != OLD_Pos) {  // if plunger position has changed

      AnalogPlunger();  // calculate the plunger movement and position
    }

 // }

  if ((Push_Flag < 1) && (Pull_Flag < 1)) {  // if plunger not in use

    NudgingRoutine();   // calculate cabinet movement and set state in subroutine NudgingRoutine

  }

  if ((Push_Flag > 0) && (Pull_Flag > 0)) {  // if plunger is in use

    x_out = x_rest;       // send null values
    y_out = y_rest;

  }

  // Plunger Reset

  if ((NEW_Pos == OLD_Pos) && (z_released > 0)) {

    if (rel_timer > 8) {  // reset all values to 0 after plunger movement complete
      
      NEW_Pos = 0;
      OLD_Pos = 0;
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

  // Set the analog nudge position (x axis, y axis)

  Pincab_ControllerVPC.setXAxis(x_out);
  Pincab_ControllerVPC.setYAxis(y_out);

  // Set the plunger state (position z axis, z rotation)
  
  Pincab_ControllerVPC.setZAxis(pl_z);
  Pincab_ControllerVPC.setZAxisRotation(pl_zrot);

}













