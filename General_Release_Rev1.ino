/* 
  Pinball input control software written and developed by Maxx (Ben Smith) and Zeb (Steve Ridgley)
  Encoder Routine developed by ElNino (Andrew Huxtable)

                              http://www.zebsboards.com
  
  *********************************************************************************************************************
  *   Tested on very little with high possibility that it may work under Visual Pinball, PinMame and Future Pinball   *
  *   Written and developed under the infulence; specifically beer.                                                   *
  *   Recompling is unnecessary for making it work.. compiles everytime....doesn't mean it works.                     *
  *********************************************************************************************************************

Rev _ Beta Release

Revisions to beta release software include:

- Removal of Hat switch display in control panel
- Complete re-write of the auto calibration and nudging subroutines (WinXP and Win7 compatability)
- Implementation of split axis scaling between x/y (0-255) and z/rz (0-127) axes
- Implementation of inverse output on rz scale
- Implementation of 4 joybutton inputs
- Tweaks to existing sub routines for increased performance
- Washing the laundry, putting the dishes out and folding the cat

Controller used:		Atmega32U4 ProMicro (Leonardo clone)

Button Pins used:		4,5,6,7,8,9,10,16
X/Y axis Pins used:		0,1
z Encoder Pins used:	2,3

Required Libraries:

Arduino:
  Bounce Library
  Keyboard Library
  PJRC Encoder Library
  
Custom:
  Requires PincabController Library  
  

*/

#include <Encoder.h>     
#include <Bounce.h>

 // assign the buttons using the Bounce library to a 10ms debounce
 
Bounce button0 = Bounce(4, 5);     // Left Flipper Button             (Left_Shift)		Physical Pin 4
Bounce button1 = Bounce(5, 5);     // Right Flipper Button            (Right_Shift)		Physical Pin 5
Bounce button2 = Bounce(6, 5);     // Left Secondary Flipper button   (Left_CTRL) 		Physical Pin 6
Bounce button3 = Bounce(7, 5);     // Right Secondary Flipper Button  (Right_CTRL)  	Physical Pin 7
Bounce button4 = Bounce(8, 5);     // Start Button                    (1)				Physical Pin 8	
Bounce button5 = Bounce(9, 5);     // Exit Button                     (E) 				Physical Pin 9
Bounce button6 = Bounce(10, 5);    // Launch Ball Button              (ENTER)			Physical Pin 10
Bounce button7 = Bounce(16, 5);    // Coin                            (5)				Physical Pin 16

// Define Encoder inputs as pins 2(A) and 3(B)

Encoder Plunger(2,3);
long Encoder_Pos = 0;
long Encoder_Max = 0; 

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
    
  //Serial.begin(9600);
  
  //delay (10000);
  
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

/*Serial.print(counter);
  Serial.print("\t");
  Serial.print(x_read);
  Serial.print("\t");
  Serial.print(y_read);
  Serial.println();
  delay (100);
*/  
}
 x_centered = (x_read/30);     // get the average reading 
 y_centered = (y_read/30);
 
 x_calib = (127 - x_centered); // create an offset constant for the axes
 y_calib = (127 - y_centered); 
 
}

void setupPins(void){
   
  // 3.3v used for accelerometer and plunger following line activates the reference voltage
  
  //analogReference(EXTERNAL);
  
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
  
  // prep for Keyboard.press() command usage
  
  Keyboard.begin();  

}
void loop(){
  
  // 50ms delay if needed
  
  // delay(50)
  
 /* delay (2000);
  Serial.print(analogRead(0));
  Serial.print("\t");
  Serial.print(analogRead(1));
  Serial.print("\t");
  Serial.print(x_centered);
  Serial.print("\t");
  Serial.print(y_centered);
  Serial.print("\t");
  Serial.print(x_calib);
  Serial.print("\t");
  Serial.print(y_calib);
  Serial.print("\t");
  Serial.println();
 */
 
  // Always be getting fresh data
  
  dataForController_t controllerData = getControllerData();
  
  // Then send out the data over the USB connection
    
  Joystick.setControllerData(controllerData);
  
  //Analogue plunger with auto zero after 5 seconds of no change.
  //If pulses are missed (possible) then it will still go to zero.
  
  int new_pos;
  new_pos = Plunger.read();
  if (new_pos != Encoder_Pos) {
    lastTimeEncoderChanged = millis();
    if (new_pos > 1650) {
      Plunger.write(1650);
      new_pos = 1650;
    }
    if (new_pos < 0) {
      Plunger.write(0);
      new_pos = 0;
    }
  }
  else {
    if (millis() - lastTimeEncoderChanged > 3500){
       new_pos = 0;
       Plunger.write(0);
    }
  }
  Encoder_Pos = new_pos;
  
  (Encoder_Pos); 
}

  dataForController_t getControllerData(void){  
  
  dataForController_t controllerData = getBlankDataForController();
   
   // Cabinet Button Routine 
  
  button0.update();    //Read the state of the Left Flipper Button
    
   if (button0.fallingEdge())         // is the button pressed
    {
    Keyboard.press(0x81);             //set the left shift key
    }
   if (button0.risingEdge())          // is the button released 
    {
    Keyboard.release(0x81);           // set the release command 
    }
   
  button1.update();   //Read the state of the Right Flipper Button
   
   if (button1.fallingEdge())    
    {
    Keyboard.press(0x85);             //right shift
    }
   if (button1.risingEdge())
    {
    Keyboard.release(0x85);
    }
   
 button2.update();    //Read the state of the Left Secondary Flipper Button
   
   if (button2.fallingEdge())
    {
    Keyboard.press(0x80);            //left control
    }
   if (button2.risingEdge())
    {
    Keyboard.release(0x80);
    }
    
 button3.update();    //Read the state of the Right Secondary Flipper Button
   
   if (button3.fallingEdge())
    {
    Keyboard.press(0x84);           //right control
    }
   if (button3.risingEdge())
    {
    Keyboard.release(0x84);
    }
        
  
  button4.update();    //Read the state of the Start Button
    
   if (button4.fallingEdge())                          
    {
    Keyboard.press(0x31);           //1
    }
   if (button4.risingEdge())                         
    {
    Keyboard.release(0x31);                        
    }
   
  button5.update();   //Read the state of the Exit Button
   
   if (button5.fallingEdge())    
    {
    Keyboard.press(0x65);         //e
    }
   if (button5.risingEdge())
    {
    Keyboard.release(0x65);
    }
   
 button6.update();    //Read the state of the Launch Ball Button  
   
   if (button6.fallingEdge())
    {
    Keyboard.press(0xB0);        //enter
    }
   if (button6.risingEdge())
    {
    Keyboard.release(0xB0);
    }
    
 button7.update();    //Read the state of the Coin Button 
   
   if (button7.fallingEdge())
    {
    Keyboard.press(0x35);        //5
    }
   if (button7.risingEdge())
    {
    Keyboard.release(0x35); 
    }
 
  // Joystick Button Section 
  
  controllerData.JButton1On = !digitalRead(14);       // Joystick Button 1 pressed
  controllerData.JButton2On = !digitalRead(15);       // Joystick Button 2 pressed
  controllerData.JButton3On = !digitalRead(A2);       // Joystick Button 3 pressed
  controllerData.JButton4On = !digitalRead(A3);       // Joystick Button 4 pressed
  
  // Apply calibration to nudge position (x axis, y axis)
  
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
  
  controllerData.nudgex = x_out;
  controllerData.nudgey = y_out; 
  
  // Set the plunger position (z axis, z rotation)
 
  controllerData.plungez = map((Encoder_Pos), 0, 1650, 0, 127); //Remap the encoder values for plunger
  int plungezrot = map((Encoder_Pos), 0, 1650, 0, 127);         //to a value between 0 and 127
  controllerData.plungezrot = (127 - plungezrot);
  
  // send the data
  
  return controllerData;
}
