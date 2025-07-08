/************************************************************************************************************************************************************************************
*                               Button states are read from the expanders as individual bits set in a 16bit integer value per chip.                                                 *
*                               The 16bit value returned is then broken apart to reflect the state of the individual bits (0 - ON/1 - OFF)                                          *
*                               which are then stored in the CurrentButtonState array ( mcp1 = bytes 0 - 15, mcp2 = bytes 16 - 32) which correlate to                               *
*                               Buttons 1 - 32.  The stored state is the inverse of what gets set in the USB report (CurrentButtonState = LOW, USB report State = HIGH)             *
*                                                                                                                                                                                   *
*                               Coin door switching uses a blocking delay of 100ms between activating the switch and deactivating the switch.  This pattern repeats on              *
*                               every state change of the switch ie: a normally closed switch that closes the circuit when the door is opened  (button pressed when door            *
*                               is closed) will see a state change from high to low when the door is opened.  The gamepad button displayed on the screen will show the              *
*                               button being pressed, a delay of 100ms, and the button being released and will stay off until the coin door is closed where the pattern             *
*                               will repeat.  This gives the software a quick on/off pulse when the door is open and again when the door is closed rather than having the           *
*                               input stay ON until it goes OFF with a door closure.                                                                                                *
*                               This feature can be disabled in the configuration utility by turning off the CoinDoorInUse variable (set to 0)                                      *
*                                                                                                                                                                                   *
************************************************************************************************************************************************************************************/
int8_t  CurrentButtonState[32] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};              // Button States of Expander 1 & 2
int8_t  PrevButtonState[32] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};                 // Previously Read Button States of Expander 1 & 2
bool  CoinDoorIsOpen = false;                                                                                    // Flag for coindoor
void Buttons (void) {
  volatile uint16_t val = mcp1.read16();
  volatile uint16_t vals = mcp2.read16();
  int buttonOffset;
  int bankOffset;
  for (int b = 0; b < 2; b++) {
    if (b == 0) {
      val = val;
      bankOffset = 0;
    }
    else {
      val = vals;
      bankOffset = 16;
    }
    for (int pin = 0; pin < 16; pin++) {
      volatile int state = bitRead(val, pin);
      if (pin < 8) {
        buttonOffset = 8;
      }
      else {
        buttonOffset = -8;
      }
      int8_t currentButton = (pin + buttonOffset) + bankOffset;
      CurrentButtonState[currentButton] = state;
    }
  }  
  for (int currentButton = 0; currentButton < 32; currentButton++) {  
    if (PrevButtonState[currentButton] != CurrentButtonState[currentButton]) {
      PrevButtonState[currentButton] = CurrentButtonState[currentButton];
      //if (GamepadFlag != 0 && useHatSwitch != 0 && currentButton > 27) {
      //HatSwitch();
      //}
      //else {
        if (currentButton == CoinDoorButton && CoinDoorInUse != 0) {
          if (CurrentButtonState[currentButton] == LOW && CoinDoorIsOpen != true) {                                  // If coin door is opened
            ZBV7.setButton(currentButton, HIGH);                                                                     // Set GP Button for Coin Door ON
            if (GamepadFlag != 1) {
              Keyboard.pressRaw(KeyCommands[currentButton]);                                                         // In Keyboard Mode so press Coin Door Key
            }
            delay(100);
            ZBV7.setButton(currentButton, LOW);                                                                      // Set GP Button for Coin Door OFF
            if (GamepadFlag != 1) {
              Keyboard.releaseRaw(KeyCommands[currentButton]);                                                       // In Keyboard Mode so release Coin Door Key
            }
            CoinDoorIsOpen = true;
          }
          else if (CurrentButtonState[currentButton] == HIGH && CoinDoorIsOpen == true) {                            // If coin door is closed after being opened
            ZBV7.setButton(currentButton, HIGH);                                                                     // Set GP Button for Coin Door ON
            if (GamepadFlag != 1) {
              Keyboard.pressRaw(KeyCommands[currentButton]);                                                         // In Keyboard Mode so press Coin Door Key
            }
            delay(100);
            ZBV7.setButton(currentButton, LOW);                                                                      // Set GP Button for Coin Door OFF
            if (GamepadFlag != 1) {
              Keyboard.releaseRaw(KeyCommands[currentButton]);                                                       // In Keyboard Mode so release Coin Door Key
            }
            CoinDoorIsOpen = false;
          }
        }
        else {
          if (CurrentButtonState[currentButton] == LOW) {
            ZBV7.setButton(currentButton, HIGH);
            if (GamepadFlag != 1) {                                                                                // If in Keyboard Mode and Button is Pressed
              Keyboard.pressRaw(KeyCommands[currentButton]);                                                       // press keypress for that button
            }
          }
          if (CurrentButtonState[currentButton] == HIGH) {
            ZBV7.setButton(currentButton, LOW);
            if (GamepadFlag != 1) {                                                                                // If in Keyboard Mode and Button is Released
              Keyboard.releaseRaw(KeyCommands[currentButton]);                                                     // release keypress for that button
            }
          }
        }
      //}
    }
  }
}

/*void HatSwitch (void)
{
  // Conditional on status of useHatSwitch flag byte
  // ZBV7.setHatSwitch(direction); CENTERED = HAT_CENTER; UP = 0; RIGHT = 90; DOWN = 180; LEFT = 270;  
   
  int hs_read[4];
  for(int x = 0; x < 4; x++)
  {
    hs_read[x] = CurrentButtonState[x+28];
  }
  if (hs_read[0] == LOW && hs_read[1] == LOW)                                                                   // 0,0,1,1
  {
    ZBV7.hat(HAT_UP_RIGHT);
  }
  else if (hs_read[0] == LOW && hs_read[3] == LOW)                                                              // 0,1,1,0       
  {
    ZBV7.hat(HAT_UP_LEFT);
  }
  else if (hs_read[0] == LOW)                                                                                   // 0,1,1,1
  {
    ZBV7.hat(HAT_UP);
  }  
  else if (hs_read[1] == LOW && hs_read[2] == LOW)                                                              // 1,0,0,1   
  {
    ZBV7.hat(HAT_DOWN_RIGHT);                                              
  }
  else if (hs_read[1] == LOW)                                                                                   // 1,0,1,1
  {
    ZBV7.hat(HAT_RIGHT);
  }
  else if (hs_read[2] == LOW && hs_read[3] == LOW)                                                              // 1,1,0,0                                                                                                                   
  {
    ZBV7.hat(HAT_DOWN_LEFT);                                                         
  }
  else if (hs_read[2] == LOW)                                                                                   // 1,1,0,1                                                                                                                    
  {
    ZBV7.hat(HAT_DOWN);
  }  
  else if (hs_read[3] == LOW)                                                                                   // 1,1,1,0                                                                                                                      
  {
    ZBV7.hat(HAT_LEFT);
  }  
  else 
  { 
    ZBV7.hat(HAT_CENTER); 
  }                                                                                                           // Otherwise set center position
}*/

/*long oldVol;

void Volume(void){
  long newVol = VolumeKnob.read();
  if (newVol != oldVol) 
  {
    if (newVol > oldVol)
    {
      VolumeControl.press(CONSUMER_CONTROL_VOLUME_INCREMENT);
      VolumeControl.release();
    }  
    else
    {
      VolumeControl.press(CONSUMER_CONTROL_VOLUME_DECREMENT);
      VolumeControl.release();
    }
    oldVol = newVol;
    //Serial.println(newVol);
  }
}*/

