int8_t  CurrentButtonState[32] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};              // Button States of Expander 1 & 2
int8_t  PrevButtonState[32] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};                 // Previously Read Button States of Expander 1 & 2
int8_t  CoinDoorIsOpen;                                                                                          // Flag for coindoor
void Buttons (void)
{
  volatile uint16_t val = mcp1.read16();
  volatile uint16_t vals = mcp2.read16();
  int buttonChanged;
  int buttonOffset;
  int bankOffset;
  for (int b = 0; b < 2; b++)
  {
    if (b == 0)
    {
      val = val;
      bankOffset = 0;
    }
    else
    {
      val = vals;
      bankOffset = 16;
    }
    for (int pin = 0; pin < 16; pin++)
    {
      volatile int state = bitRead(val, pin);
      if (pin < 8)
      {
        buttonOffset = 8;
      }
      else
      {
        buttonOffset = -8;
      }
      int8_t currentButton = (pin + buttonOffset) + bankOffset;
      CurrentButtonState[currentButton] = state;
    }
  }  
  for (int currentButton = 0; currentButton < 32; currentButton++)
  {  
    if (PrevButtonState[currentButton] != CurrentButtonState[currentButton])
    {
      PrevButtonState[currentButton] = CurrentButtonState[currentButton];
      if (GamepadFlag != 0 && useHatSwitch != 0 && currentButton > 27)
      {
        HatSwitch();
      }
      else
      {
        if (currentButton != CD_Pos)
        {
          if (CurrentButtonState[currentButton] == LOW)
          {
            Joystick.pressButton(currentButton);
            if (GamepadFlag != 1)                                                                                  // If in Keyboard Mode and Button is Pressed
            {
              Keyboard.pressRaw(KeyCommands[currentButton]);                                                       // press keypress for that button
            }
          }
          if (CurrentButtonState[currentButton] == HIGH)
          {
            Joystick.releaseButton(currentButton);
            if (GamepadFlag != 1)                                                                                  // If in Keyboard Mode and Button is Released
            {
              Keyboard.releaseRaw(KeyCommands[currentButton]);                                                     // release keypress for that button
            }
          }
        }
        else
        {
          if (CurrentButtonState[currentButton] == LOW && CoinDoorIsOpen != 1)                                    // If coin door is opened
          {
            Joystick.pressButton(currentButton);                                                              // Set GP Button for Coin Door ON
            if (GamepadFlag != 1)
            {
              Keyboard.pressRaw(KeyCommands[currentButton]);                                                         // In Keyboard Mode so press Coin Door Key
            }
            delay(100);
            Joystick.releaseButton(currentButton);                                                               // Set GP Button for Coin Door OFF
            if (GamepadFlag != 1)
            {
              Keyboard.releaseRaw(KeyCommands[currentButton]);                                                       // In Keyboard Mode so release Coin Door Key
            }
            CoinDoorIsOpen = 1;
          }
          else if (CurrentButtonState[currentButton] == HIGH && CoinDoorIsOpen == 1)                              // If coin door is closed after being opened
          {
            Joystick.pressButton(currentButton);                                                              // Set GP Button for Coin Door ON
            if (GamepadFlag != 1)
            {
              Keyboard.pressRaw(KeyCommands[currentButton]);                                                         // In Keyboard Mode so press Coin Door Key
            }
            delay(100);
            Joystick.releaseButton(currentButton);                                                               // Set GP Button for Coin Door OFF
            if (GamepadFlag != 1)
            {
              Keyboard.releaseRaw(KeyCommands[currentButton]);                                                       // In Keyboard Mode so release Coin Door Key
            }
            CoinDoorIsOpen = 0;
          }
        }
      }
    }
  }
}

void HatSwitch (void)
{
  // Conditional on status of useHatSwitch flag byte
  // Joystick.setHatSwitch(direction); CENTERED = HAT_CENTER; UP = 0; RIGHT = 90; DOWN = 180; LEFT = 270;  
   
  int hs_read[4];
  for(int x = 0; x < 4; x++)
  {
    hs_read[x] = CurrentButtonState[x+28];
  }
  if (hs_read[0] == LOW && hs_read[1] == LOW)                                                                   // 0,0,1,1
  {
    Joystick.hat(HAT_UP_RIGHT);
  }
  else if (hs_read[0] == LOW && hs_read[3] == LOW)                                                              // 0,1,1,0       
  {
    Joystick.hat(HAT_UP_LEFT);
  }
  else if (hs_read[0] == LOW)                                                                                   // 0,1,1,1
  {
    Joystick.hat(HAT_UP);
  }  
  else if (hs_read[1] == LOW && hs_read[2] == LOW)                                                              // 1,0,0,1   
  {
    Joystick.hat(HAT_DOWN_RIGHT);                                              
  }
  else if (hs_read[1] == LOW)                                                                                   // 1,0,1,1
  {
    Joystick.hat(HAT_RIGHT);
  }
  else if (hs_read[2] == LOW && hs_read[3] == LOW)                                                              // 1,1,0,0                                                                                                                   
  {
    Joystick.hat(HAT_DOWN_LEFT);                                                         
  }
  else if (hs_read[2] == LOW)                                                                                   // 1,1,0,1                                                                                                                    
  {
    Joystick.hat(HAT_DOWN);
  }  
  else if (hs_read[3] == LOW)                                                                                   // 1,1,1,0                                                                                                                      
  {
    Joystick.hat(HAT_LEFT);
  }  
  else 
  { 
    Joystick.hat(HAT_CENTER); 
  }                                                                                                           // Otherwise set center position
}

/*long oldVol;
int countClicks;

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

