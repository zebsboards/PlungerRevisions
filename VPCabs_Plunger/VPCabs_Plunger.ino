/* 
 *  VPCabs Plunger Firmware R1 2/16/2019
 *  written and developed by Zebsboards.com
 *  ownership and copyright remains with Zebsboards.com
 */
 
#include "Keyboard.h"
#include "Pincab_ControllerVPC.h"
#include <Bounce.h>
#include <HID.h>
#include <ResponsiveAnalogRead.h>


// Define Plunger INPUT Variables

int OLD_Pos = 0;       // last count of plunger position
int NEW_Pos = 0;        // current plunger position
int Push_Flag = 0;      // flag to indicate slam event
int Pull_Flag = 0;      // flag to indicate plunger event

int Coin_Door = 0; // Coindoor Open State indicator '0' = Closed , '1' = Open


// Define Plunger OUTPUT variables

int pl_z;       // z (plunger) axis output variable
int pl_zrot;    // inverted z (plunger) axis variable

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
int x_out = 0;          // x axis final output value
int y_init = 0;         // y axis sampling data
int y_read = 0;         // y axis sampling running total
int y_calib = 0;        // y axis final calibration value
int y_centered = 0;     // y axis offset value
int y_ref = 0;          // Y axis reference value
int y_raw = 0;          // y axis raw smoothed value
int y_out = 0;          // Y axis final output value
int z_init = 0;         // z axis sampling data
int z_read = 0;         // z axis sampling running total
int z_calib = 0;        // z axis final calibration value
int z_centered = 0;     // z axis offset value
int z_ref = 0;          // z axis reference value
int z_out = 0;          // z axis final output value
int z_max = 0;          // z axis maximum drawback
int z_released = 0;      // z axis released flag
int rel_timer;

int tilt_VAL = 0;       // TILT value setting at which tilt occurs
int G_factor = 10;      // divisor for Gain adjustment

float nGain = 0.00;     // Gain adjustment value for nudging
float nGainFinal = 0.00; // Final Gain adjustment value

unsigned long tilt_time;      // Sensor reading for setting tilt_VAL

ResponsiveAnalogRead analog_X(A0, true);
ResponsiveAnalogRead analog_Y(A1, true);
ResponsiveAnalogRead analog_Z(A4, true);
ResponsiveAnalogRead analog_G(A3, true);
ResponsiveAnalogRead analog_T(A2, true);


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

  //Serial.begin (9600);               // debugging/ zb launch monitor
}

//******************************************** Main Code **************************************************

void loop() {

  Pincab_ControllerVPC.sendState();     // Send out the data over the USB connection

  Buttoninput();       // read buttons and set state in subroutine Buttoninput

  analog_Z.update();  // read the raw plunger data

  NEW_Pos = (analog_Z.getValue());    // use the smoothed value


  // Apply Plunger Calibration

  if (NEW_Pos > (z_centered + 20) || NEW_Pos < (z_centered - 20)) {    //deadzone to accomodate plunger fluctuation at static rest

    NEW_Pos = NEW_Pos + z_calib;                                     //send the realtime readings
    if (NEW_Pos > 632) {
      NEW_Pos = 632;                                                //limit the readings to a maximum set scale
    }
    if (NEW_Pos < -210) {
      NEW_Pos = -210;                                               //limit the redaings to a minimum set scale
    }
  }

  else  {
    NEW_Pos = z_centered + z_calib;                                //send the center position
  }


  /*// Plunger as Digital Button

  if (digitalRead(2) == LOW)  {  // if the input at pin 1 is Low or serial character from pinballx is 1 use Digital Ball Launch

    DigitalballLaunch();
  }*/

  // Plunger as Analog Plunger


  if (NEW_Pos != OLD_Pos) {  // if plunger position has changed

    AnalogPlunger();  // calculate the plunger movement and position

  }

  if ((Push_Flag < 1) && (Pull_Flag < 1)) {  // if plunger not in use

    NudgingRoutine();   // calculate cabinet movement and set state in subroutine NudgingRoutine

  }

  if ((Push_Flag > 0) && (Pull_Flag > 0)) {  // if plunger is in use

    x_out = 0;       // send null values
    y_out = 0;

  }

  // Plunger Reset

  if ((NEW_Pos == OLD_Pos) && (z_released > 0)) {

    if (rel_timer > 3) {  // reset all values to 0 after plunger movement complete

      NEW_Pos = 0;
      OLD_Pos = 0;
      pl_z = 0;
      pl_zrot = 0;
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

//************************************************************************** Sub-routines ********************************************


void setupPins(void) { //************************************************* Pin Map **************************************************

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
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT_PULLUP);
  pinMode(30, INPUT_PULLUP);
}

void autoleveling(void) { //************************************************ Analog Calibration **********************************************

  while (buffer < buffer_end) {      //initialization routine for accelerometer
    analogRead(0);                     //dummy values that are not used anywhere else
    analogRead(1);                     //
    buffer = buffer + 1;               //advance the count
  }

  while (counter < counter_end) {

    analog_X.update();                 //sample the axis value
    analog_Y.update();                 //sample the axis value
    analog_Z.update();                 //sample the axis value

    x_init = (analog_X.getValue());    //assign the smoothed axis value
    x_read = x_read + x_init;          //accumulate the totals
    y_init = (analog_Y.getValue());
    y_read = y_read + y_init;
    z_init = (analog_Z.getValue());
    z_read = z_read + z_init;

    counter = counter + 1;             //advance the counter
  }

  x_centered = (x_read / 30);   // get the average reading
  y_centered = (y_read / 30);
  z_centered = (z_read / 30);


  x_calib = (0 - x_centered); // create an offset constant for the axes
  y_calib = (0 - y_centered);
  z_calib = (0 - z_centered);
}

void Buttoninput(void) {  //************************************************** Cabinet button  In-Game sub routine *****************************

  // Read and set the button states

  button0.update();    // Left Flipper

  if (button0.fallingEdge())
  {
    Keyboard.press(KEY_LEFT_ARROW);         // LEFT_ARROW
  }  
  if (button0.risingEdge())
  {
    Keyboard.release(KEY_LEFT_ARROW);
  }

  button1.update();    // LEFT MAGNASAVE

  if (button1.fallingEdge())
  {
    Keyboard.press(KEY_LEFT_CTRL);           // LEFT_CTRL
  }
  if (button1.risingEdge())
  {
    Keyboard.release(KEY_LEFT_CTRL);
  }

  button2.update();    //START

  if (button2.fallingEdge())
  {
    Keyboard.press('1');             // 1
  }
  if (button2.risingEdge())
  {
    Keyboard.release('1');
  }

  button3.update();   //EXIT

  if (button3.fallingEdge())
  {
    Keyboard.press(KEY_ESC);         // Esc 
  }
  if (button3.risingEdge())
  {
    Keyboard.release(KEY_ESC);
  }

  button4.update();    //Coin

  if (button4.fallingEdge())
  {
    Keyboard.press('5');            // 5
  }
  if (button4.risingEdge())
  {
    Keyboard.release('5');
  }

  button5.update();   // R_FLIPPER

  if (button5.fallingEdge())
  {
    Keyboard.press(KEY_RIGHT_ARROW);         // R_ARROW
  }
  if (button5.risingEdge())
  {
    Keyboard.release(KEY_RIGHT_ARROW);
  }

  button6.update();    // R_MAGNASAVE

  if (button6.fallingEdge())
  {
    Keyboard.press(KEY_RIGHT_CTRL);        // R_CTRL
  }
  if (button6.risingEdge())
  {
    Keyboard.release(KEY_RIGHT_CTRL);
  }

  button7.update();    // LAUNCH

  if (button7.fallingEdge())
  {
    Keyboard.press(0xB0);        // ENTER
  }
  if (button7.risingEdge())
  {
    Keyboard.release(0xB0);
  }

  button8.update();    //GP1

  if (button8.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(0, HIGH);
  }
  if (button8.risingEdge())
  {
    Pincab_ControllerVPC.setButton(0, LOW);
  }

  button9.update();    //GP2

  if (button9.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(1, HIGH);
  }
  if (button9.risingEdge())
  {
    Pincab_ControllerVPC.setButton(1, LOW);
  }

  button10.update();   //GP3

  if (button10.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(2, HIGH);
  }  
  if (button10.risingEdge())
  {
    Pincab_ControllerVPC.setButton(2, LOW);
  }

  button11.update();    //GP4

  if (button11.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(3, HIGH);
  }
  if (button11.risingEdge())
  {
    Pincab_ControllerVPC.setButton(3, LOW);
  }

  button12.update();    //GP5

  if (button12.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(4, HIGH);
  }
  if (button12.risingEdge())
  {
    Pincab_ControllerVPC.setButton(4, LOW);
  }

  button13.update();   //GP6

  if (button13.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(5, HIGH);
  }
  if (button13.risingEdge())
  {
    Pincab_ControllerVPC.setButton(5, LOW);
  }

  button14.update();    //GP7

  if (button14.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(6, HIGH);
  }
  if (button14.risingEdge())
  {
    Pincab_ControllerVPC.setButton(6, LOW);
  }

  button15.update();    //GP8

  if (button15.fallingEdge())
  {
   Pincab_ControllerVPC.setButton(7, HIGH);
  }
  if (button15.risingEdge())
  {
   Pincab_ControllerVPC.setButton(7, LOW);
  }

  button16.update();    //GP9

  if (button16.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(8, HIGH);
  }
  if (button16.risingEdge())
  {
    Pincab_ControllerVPC.setButton(8, LOW);
  }

  button17.update();    //GP10

  if (button17.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(9, HIGH);
  }
  if (button17.risingEdge())
  {
    Pincab_ControllerVPC.setButton(9, LOW);
  }

}


void NudgingRoutine (void) {  // ************************* Nudging sub routine ************************************

  analog_G.update();                                    // read analog gain signal
  nGain = ((analog_G.getValue()) / G_factor);           // scale gain value to 0 - 102
  nGainFinal = (nGain * .01);                           // calculate final gain value to be applied to nudging output values

  // Read the nudge sensor and apply calibration to nudge position (x axis, y axis)

  analog_X.update();    //sample the x axis
  analog_Y.update();    //sample the y axis

  x_ref = (analog_X.getValue());   //use the smoothed values
  y_ref = (analog_Y.getValue());

  x_raw = round(x_ref + x_calib);
  y_raw = round(y_ref + y_calib);

  if (x_ref > (x_centered + 10) || x_ref < (x_centered - 10)) {    //deadzone to accomodate accelerometer fluctuation at static rest
    x_out = round(x_raw * nGainFinal);                                   //send the realtime readings
  }

  else  {
    x_out = round(x_centered + x_calib);                                //send the center position
  }

  if (y_ref > (y_centered + 10) || y_ref < (y_centered - 10)) {
    y_out = round(y_raw * nGainFinal);
  }

  else  {
    y_out = round(y_centered + y_calib);
  }

  // read the TILT setting and verify that we aren't tilting the machine

  analog_T.update();                                                   // read analog TILT signal
  tilt_VAL = (analog_T.getValue());                                    // assign TITL variable signal to TILT threshold

  if ((x_out > tilt_VAL) || (y_out > tilt_VAL)) {       // if tilt has been activated

    if (millis() - tilt_time > 1000) {     // restricting the tilt alert to once every 1sec
      Keyboard.press('t');        // END
      Pincab_ControllerVPC.setButton(10, HIGH);
      Keyboard.release('t');
      delay(5);
      Pincab_ControllerVPC.setButton(10, LOW);
      //Serial.println("TILT!!!!!");
      tilt_time = millis();
    }
  }

}

void DigitalballLaunch (void) {  // ********************** Plunger as Enter Key sub routine ********************

    
  pl_z = map(NEW_Pos, 0, -127, 0, -127); // scale plunger actual reading (-) to full scale reading (+) due to smaller travel distance
  constrain(pl_z, 0, -127);

  pl_zrot = pl_z * -1;

  if (pl_z < 0) {    // if plunger is pressed
    Keyboard.press (0xB0);    // Press Enter Key
    z_max = pl_z;
  }
  if (pl_z > z_max)  {
    Keyboard.release (0xB0);  // Release Enter Key
    rel_timer = 0;
    z_released = 1;
  }

}

void AnalogPlunger (void) {  // *************************** Analog Plunger sub Routine *************************

    
  //  1st step - determine push or pull event

  if ((NEW_Pos > OLD_Pos) && (Pull_Flag < 1) && (Push_Flag < 1)) {

    Push_Flag = 0;
    Pull_Flag = 1;

  }
  if (Pull_Flag > 0) {                                                           // Pull Flag = 1

    //  2nd step - factor plunger movement to axis scale

    if (NEW_Pos > 0) {
      pl_z = map(NEW_Pos, 0, 620, 0, 127);
      if (pl_z > 127) {
        pl_z = 127;
      }

      if ((NEW_Pos > OLD_Pos) && (z_released < 1)) {
        z_max = NEW_Pos;                                                           // set max pullback value
        constrain(z_max, 0, 620);
      }
      if ((NEW_Pos < round(z_max * .75)) && (z_released < 1)) {                 // Set the release flag as plunger is released
        z_released = 1;   // plunger released, set flag
        rel_timer = 0;  // start timer
      }
    }

    if (NEW_Pos < 0) {                                                         // release has gone below 0
      pl_z = map(NEW_Pos, -117, 0, -127, 0);
      constrain(pl_z, -127, 0);
    }

    pl_zrot = pl_z * -1;

  }

  if ((NEW_Pos < OLD_Pos) && (Pull_Flag < 1) && (Push_Flag < 1)) {

    Push_Flag = 1;
    Pull_Flag = 0;

  }
  if (Push_Flag > 0) {    // Push Flag = 1

    pl_z = map(NEW_Pos, -117, 0, -127, 0);
    constrain(pl_z, -127, 0);

    if (NEW_Pos < OLD_Pos) {
      z_max = NEW_Pos;  // set max pullback value
      constrain(z_max, -127, 0);
    }
    if ((NEW_Pos > z_max) && (z_released < 1)) {
      z_released = 1;   // plunger released, set flag
      rel_timer = 0;  // start timer
    }

    pl_zrot = pl_z * -1;
  }
  return;
}



