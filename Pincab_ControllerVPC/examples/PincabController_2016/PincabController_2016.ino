/* 
  Pinball input control software written and developed by Maxx (Ben Smith) and Zeb (Steve Ridgley)
  

                                     http://www.zebsboards.com
  
  *************************************************************************************************************************
  *   Tested on very little with the high possibility that it may work under Visual Pinball, PinMame and Future Pinball   *
  *   Written and developed under the influence; specifically beer.                                                       *
  *   Recompling is unnecessary for making it work.. compiles everytime....doesn't mean it works.                         *
  *************************************************************************************************************************

Rev _ Palm Smash with Launch Ball_Windows 10 Compatibility - January 2016

Revisions to beta release software include:

- Port code to Windows 10 pluggable usb compatibility
- Remove keyboard functions and replace with gamepad buttons
- Remove redundant code

TO DO

-Fix button display - remove 4 extra
-Create Ledwiz object for internal monitoring of the Digital Launch Feature
-Write Bill Gates a strongly worded letter expressing my appreciation for fucking up my existing code performance with his latest release

*/

#include "Joystick.h"
#include <Encoder.h>     
#include <Bounce.h>

 
// Define Encoder inputs as pins 2(A) and 3(B)

Encoder Plunger(2,3);
long Encoder_Pos = 0;
long Encoder_Max = 0; 
int new_pos;
int palm_smash;
int pl_z;
int pl_zrot;

volatile unsigned long lastTimeEncoderChanged;

// Assign Auto Leveling and Nudging variables

int counter = 0;        // sampling counter start
int counter_end = 30;   // sampling counter duration 
int buffer=0;           // accelerometer initialization start 
int buffer_end=100;      // accelerometer initialization duration
int x_init = 0;         // x axis sampling data 
int x_read = 0;         // x axis sampling running total 
int x_calib = 0;        // x axis final calibration value
int x_centered = 0;     // x axis offset value
int x_ref = 0;          // x axis reference value
int x_out = 0;              // x axis final output value 
int y_init = 0;         // y axis sampling data 
int y_read = 0;         // y axis sampling running total
int y_calib = 0;        // y axis final calibration value
int y_centered = 0;     // y axis offset value
int y_ref = 0;          // Y axis reference value
int y_out = 0;              // Y axis final output value


void setup(){

 setupPins();
  
  //'Auto leveling' at power up
  
 while (buffer < buffer_end) {      //initialization routine for accelerometer
  analogRead(0);                    //dummy values that are not used anywhere else
  analogRead(1);                    //
  buffer = buffer + 1;              //advance the count
}

while (counter < counter_end){
  
  x_init = analogRead(0);    //sample the axis value
  x_read = x_read + x_init;  //accumulate the totals
  y_init = analogRead(1);
  y_read = y_read + y_init;
  counter = counter + 1;     //advance the counter 
}
 x_centered = (x_read/30);     // get the average reading 
 y_centered = (y_read/30);
 
 x_calib = (127 - x_centered); // create an offset constant for the axes
 y_calib = (127 - y_centered); 
 
}

void setupPins(void){
    
  // set the internal pullup resistors on the inputs to active
  
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);

// assign the buttons using the Bounce library to a 5ms debounce
 
  Bounce button0 = Bounce(7, 5);    
  Bounce button1 = Bounce(8, 5);     
  Bounce button2 = Bounce(4, 5);     
  Bounce button3 = Bounce(5, 5);     
  Bounce button4 = Bounce(6, 5);     
  Bounce button5 = Bounce(9, 5);     
  Bounce button6 = Bounce(10, 5); 
  Bounce button7 = Bounce(16, 5);    
  Bounce button8 = Bounce(14, 5);
  Bounce button9 = Bounce(15, 5); 
  Bounce button10 = Bounce(A2, 5);    
  Bounce button11 = Bounce(A3, 5);

// Initialize Joystick Library
  Joystick.begin();
  
}

void loop(){
  
// Send out the data over the USB connection
    
    Joystick.sendState();

// Read the buttons and set the state

    Joystick.setButton(0,!digitalRead(7));          // Joystick Button 1 state
    Joystick.setButton(1,!digitalRead(8));          // Joystick Button 2 state
    Joystick.setButton(2,!digitalRead(4));          // Joystick Button 3 state
    Joystick.setButton(3,!digitalRead(5));          // Joystick Button 4 state
    Joystick.setButton(4,!digitalRead(6));          // Joystick Button 5 state
    Joystick.setButton(5,!digitalRead(9));          // Joystick Button 6 state
    Joystick.setButton(6,!digitalRead(10));         // Joystick Button 7 state
    Joystick.setButton(7,!digitalRead(16));         // Joystick Button 8 state
    Joystick.setButton(8,!digitalRead(14));         // Joystick Button 9 state
    Joystick.setButton(9,!digitalRead(15));        // Joystick Button 10 state
    Joystick.setButton(10,!digitalRead(A2));        // Joystick Button 11 state
    Joystick.setButton(11,!digitalRead(A3));        // Joystick Button 12 state
  
// Read the nudge sensor and apply calibration to nudge position (x axis, y axis)
  
    x_ref= analogRead(0);
    y_ref= analogRead(1);
    
    if (x_ref > (x_centered + 10) || x_ref < (x_centered - 10)) {    //deadzone to accomodate accelerometer fluctuation at static rest
      x_out = x_ref + x_calib;                                     //send the realtime readings 
    }
    
    else  {
      x_out = x_centered + x_calib;                                //send the center position
    }
    
    if (y_ref > (y_centered + 10) || y_ref < (y_centered - 10)) {
      y_out = y_ref + y_calib;
    }
    
    else  {
      y_out = y_centered + y_calib;
    }
 
// Set the analog nudge position (x axis, y axis) 
  
    Joystick.setXAxis(x_out);
    Joystick.setYAxis(y_out); 
  
// Read the plunger postion
  
  new_pos = Plunger.read();
          
    if (new_pos > 2600) {  // constrain encoder to maximum value on scale
       new_pos = 2600;
       Encoder_Pos = 2600;
      }
      
    if (new_pos < -400) {  //constrain encoder to minimum value on scale
        new_pos = -400;
        Encoder_Pos = -400;
      }  
    
    if (new_pos != Encoder_Pos) {    // if encoder position has changed
      
      lastTimeEncoderChanged = millis();  //reference watchdog timer value
      palm_smash = new_pos;
/*
      if ((digitalRead(10) == LOW) && (palm_smash < 0)) {  // read the input at pin 10  Low= Digital High = Analog
          Keyboard.press(0xB0);        //enter
          Keyboard.release(0xB0);
          new_pos = 0;
          Encoder_Pos = 0;
      }*/
                 
     //Pull Back Routine 
      if ((palm_smash > 0) && (palm_smash < 2601)) {
         new_pos = palm_smash;
      }
     
     //Palm Smash Routine
      if ((palm_smash < -20)) {
            new_pos = palm_smash * -7.5;
            
            if (new_pos < -400) {  //constrain encoder to minimum value on scale
              new_pos = -400;
              Encoder_Pos = -400;
            }
            if (new_pos > 2600) {  //constrain encoder to maximum value on scale
              new_pos = 2600;
              Encoder_Pos = 2600;
            }  
         }
    }
    else {
      if (millis() - lastTimeEncoderChanged > 2000){  // watchdog reset of stuck plunger
              new_pos = 0;                            // auto zero after 2.0 seconds of no change.
              Encoder_Pos = 0;                        // If pulses are missed (possible) then it will still go to zero.
              Plunger.write(0);
      }
   }
  Encoder_Pos = new_pos;
  pl_z= map((new_pos), -400, 2600,0, 127); //Remap the encoder values for plunger
  pl_zrot = pl_z;                          //to a value between 0 and 127

// Set the plunger position (z axis, z rotation)
 
  Joystick.setZAxis(pl_z); 
  Joystick.setZAxisRotation(pl_zrot);         
  //controllerData.plungezrot = (127 - plungezrot);
}


