/*
  Pinball input control software written and developed by Zeb (Steve Ridgley)


                                     http://www.zebsboards.com

  *************************************************************************************************************************************
      Tested on very little with the high possibility that it may work under Visual Pinball, PBFX2, TPA, PinMame and Future Pinball
      Written and developed under the influence; specifically beer.
      Recompling is unnecessary for making it work.. compiles everytime....doesn't mean it works.
  *************************************************************************************************************************************

  VPCabs Initial Vertigo Controller Firmware

*/

#include "Keyboard.h"
#include "Pincab_ControllerVPC.h"
#include <Bounce.h>
#include <HID.h>
#include <ResponsiveAnalogRead.h>


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


ResponsiveAnalogRead analog_X(A1, true);
ResponsiveAnalogRead analog_Y(A0, true);

String readString; // variable for Serial Solenoid Command

int LsolOUT = A3;  // left flipper OUTPUT signal
int RsolOUT = A4;  // right flipper OUTPUT signal

int inputs[]{7,A5}; // Solenoid state input pins
int in_val[]{0,0};      // pin values of inputs
int SolState[]{0,0};      // Solenoid state flags 0=OFF, 1=ON

int solOutCTRL = A2; // solenoid relay enable/disable signal
int solOUTflag = 0;  // solenoid relay status flag  0 = OFF  1 = ON  start system in default OFF(0) setting

unsigned long solRuntime[]{0,0};      // watchdog current timer value for solenoids

int timer_out = 10000;  // timer variable for relay kill (in millis) default is 10 seconds 


//*************************************** Debounce (5ms) **************************************************

Bounce button0 = Bounce(2, 5);    // (ESC & N)    (0x01)/(Ox4E)
Bounce button1 = Bounce(12, 5);    // (L_Alt)      (0x5D)
Bounce button2 = Bounce(3, 5);    // (5)          (0x05)
Bounce button3 = Bounce(13, 5);   // (Enter))     (0xB0)
Bounce button4 = Bounce(8, 5);    // (L_Ctrl)     (0x80)
Bounce button5 = Bounce(4, 5);    // (R_Arrow)    (0x63)
Bounce button6 = Bounce(5, 5);    // (2)          (0x02)
Bounce button7 = Bounce(10, 5);   // (1)          (0x01)
Bounce button8 = Bounce(9, 5);    // (D_Arrow)    (0x62)
Bounce button9 = Bounce(1, 5);    // (L_Arrow)    (0x61)
Bounce button10 = Bounce(6, 5);   // (Space)      (0x5E)
Bounce button11 = Bounce(0, 5);   // (U_Arrow)    (0x57)


//***********************************************Setup ***************************************************

void setup() {

  //analogReference(EXTERNAL);         // force 3.3v reference voltage for analog signals
  analogRead(A0);
  analogRead(A1);                    // intial warm up of analog devices
  
  setupPins();                       // set pinmode to pullup (LOW trigger)

  autoleveling();                    // Autoleveling at power up

  Pincab_ControllerVPC.begin();      // Initialize Pincab_Controller Library

  Keyboard.begin();                  // Initialize Keyboard Library

  Serial.begin (9600);               // solenoid output control
  
}

//******************************************** Main Code **************************************************

void loop() {

  Pincab_ControllerVPC.sendState();     // Send out the data over the USB connection

  SolControl();       // Serial read and switch solenoid output on/off based on data received 

  if (solOUTflag > 0){
    solWatchdog();      // call solenoid watchdog routine
  }
  else{
    for (int i = 0; i < 2; i++){
      solRuntime[i] = 0; // reset the runtime timers 
    }
  }
  
  Buttoninput();       // read buttons and set state in subroutine Buttoninput

  NudgingRoutine();   // calculate cabinet movement and set state in subroutine NudgingRoutine

  // Set the analog nudge position (x axis, y axis)

  Pincab_ControllerVPC.setXAxis(0);
  Pincab_ControllerVPC.setYAxis(0);

  // Set the z axis dummy values
  
  Pincab_ControllerVPC.setZAxis(0);
  Pincab_ControllerVPC.setZAxisRotation(0);
  
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
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(7, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);

  // set the ouput pins
  
  pinMode(solOutCTRL, OUTPUT);
  pinMode(LsolOUT, OUTPUT); 
  pinMode(RsolOUT, OUTPUT);
  
  // Pins not in Use/Available  
  
  pinMode(11, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
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
    //analog_Z.update();                 //sample the axis value

    x_init = (analog_X.getValue());    //assign the smoothed axis value
    x_read = x_read + x_init;          //accumulate the totals
    y_init = (analog_Y.getValue());
    y_read = y_read + y_init;
    

    counter = counter + 1;             //advance the counter
  }

  x_centered = (x_read / 30);   // get the average reading
  y_centered = (y_read / 30);
  


  x_calib = (0 - x_centered); // create an offset constant for the axes
  y_calib = (0 - y_centered);
  
}

void SolControl(void) {  //********************************************** Serial Solenoid Control Routine **************************************

  while (Serial.available()) {
    delay (30);
    char c = Serial.read();
    readString += c ;
   }
   if (readString.length()> 0) {
    if (readString == "VPCON"){
      digitalWrite(solOutCTRL, HIGH);
      solOUTflag = 1;
    }
    if (readString == "VPCOFF"){
      digitalWrite(solOutCTRL, LOW);
      solOUTflag = 0;
    }
    readString ="";
   }
  
}

void Buttoninput(void) {  //************************************************** Cabinet button  In-Game sub routine *****************************

  // Read and set the button states

  button0.update();    // 

  if (button0.fallingEdge())
  {
    Keyboard.press(KEY_ESC);         // Esc 
    Keyboard.press('N');         // N
  }
  if (button0.risingEdge())
  {
    Keyboard.release(KEY_ESC);
    Keyboard.release('N');
  }

  button1.update();    

  if (button1.fallingEdge())
  {
    Keyboard.press(KEY_LEFT_ALT);           // Left Alt
  }
  if (button1.risingEdge())
  {
    Keyboard.release(KEY_LEFT_ALT);
  }

  button2.update();    

  if (button2.fallingEdge())
  {
    Keyboard.press('5');             // 5
  }
  if (button2.risingEdge())
  {
    Keyboard.release('5');
  }

  button3.update();   

  if (button3.fallingEdge())
  {
    Keyboard.press(0xB0);             //Enter
  }
  if (button3.risingEdge())
  {
    Keyboard.release(0xB0);
  }

  button4.update();    

  if (button4.fallingEdge())
  {
    Keyboard.press(KEY_LEFT_CTRL);            // Left CTRL
  }
  if (button4.risingEdge())
  {
    Keyboard.release(KEY_LEFT_CTRL);
  }

  button5.update();   

  if (button5.fallingEdge())
  {
    Keyboard.press(KEY_RIGHT_ARROW);         // Right Arrow
    digitalWrite(RsolOUT,HIGH);
  }
  if (button5.risingEdge())
  {
    Keyboard.release(KEY_RIGHT_ARROW);
    digitalWrite(RsolOUT,LOW);
  }

  button6.update();    

  if (button6.fallingEdge())
  {
    Keyboard.press('2');        // 2
  }
  if (button6.risingEdge())
  {
    Keyboard.release('2');        
  }

  button7.update();    

  if (button7.fallingEdge())
  {
    Keyboard.press('1');        // 1
  }
  if (button7.risingEdge())
  {
    Keyboard.release('1');
  }

  button8.update();    

  if (button8.fallingEdge())
  {
    Keyboard.press(KEY_DOWN_ARROW);           // Down Arrow
  }
  if (button8.risingEdge())
  {
    Keyboard.release(KEY_DOWN_ARROW);
  }

  button9.update();    

  if (button9.fallingEdge())
  {
    Keyboard.press(KEY_LEFT_ARROW);             // Left Arrow
    digitalWrite(LsolOUT,HIGH);
  }
  
  if (button9.risingEdge())
  {
    Keyboard.release(KEY_LEFT_ARROW);
    digitalWrite(LsolOUT,LOW);
  }

  button10.update();   

  if (button10.fallingEdge())
  {
    Keyboard.press(' ');             // Space
  }
  if (button10.risingEdge())
  {
    Keyboard.release(' ');
  }

  button11.update();    

  if (button11.fallingEdge())
  {
    Keyboard.press(KEY_UP_ARROW);           //Up Arrow
  }
  if (button11.risingEdge())
  {
    Keyboard.release(KEY_UP_ARROW);
  }
 
}


void NudgingRoutine (void) {  // ************************* Nudging sub routine ************************************

  // Read the nudge sensor and apply calibration to nudge position (x axis, y axis)

  analog_X.update();    //sample the x axis
  analog_Y.update();    //sample the y axis

  x_ref = (analog_X.getValue());   //use the smoothed values
  y_ref = (analog_Y.getValue());

  x_raw = round(x_ref + x_calib);
  y_raw = round(y_ref + y_calib);

  if (x_ref > (x_centered + 10) || x_ref < (x_centered - 10)) {    //deadzone to accomodate accelerometer fluctuation at static rest
    x_out = (x_raw * -1);                                   //send the realtime readings
  }

  else  {
    x_out = round(x_centered + x_calib);                                //send the center position
  }

  if (y_ref > (y_centered + 10) || y_ref < (y_centered - 10)) {
    y_out = (y_raw * -1);
  }

  else  {
    y_out = round(y_centered + y_calib);
  }

}

void solWatchdog(void){  //************************ Solenoid watchdog timer **************************************************************

   for (int swd = 0; swd < 2; swd++){
    in_val[swd] = digitalRead(inputs[swd]);
    if (in_val[swd] < 1){  // if input is on 
      if (SolState[swd] < 1){  // if solenoid on flag hasn't been set
       SolState[swd] = 1;     // set solenoid state to ON
       solRuntime[swd] = millis(); // get initial time for timer
      }
      else {   // if solenoid flag has been set
        if (millis() - solRuntime[swd] > timer_out){ 
          digitalWrite(solOutCTRL, LOW);  // if timer exceeds timer value then kill relay
          SolState[swd] = 0;              // set solenoid state flag to 0 "OFF"
          solRuntime[swd] = 0;            // reset runtimer
       }
      }
    }
    
    else {
      SolState[swd] = 0;    // set solenoid flag to 0 "OFF"  
      solRuntime[swd] = 0;  // reset runtimer
    }
   }
}
