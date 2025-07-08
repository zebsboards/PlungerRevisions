int16_t MaxPush;

void Plunger(void)
{
  plungerGetVal();                                                                                                                      // GoSub and get plunger position based on sensor installed
  if ((plungerRead > plungerAtRest + plungerDeadzone) || (plungerRead < plungerAtRest - plungerDeadzone))                               // If plunger is outside of the deadzone
  {
    if (plungerRead < plungerAtRest)                                                                                                    // PULLING
    {
      plungerTimeOut = 0;
      //Serial.print(plungerRead); Serial.print("  ");
      plungerRead = plungerAtRest - plungerRead;
      constrain(plungerRead, 0, plungerDrawLength);
      //Serial.print(plungerRead); Serial.print("  ");
      plungerRead = map(plungerRead, 0, plungerDrawLength, 0, 32767);                                                                   // set readings to Pull Event scale
      plungerRead = plungerRead * - 1;
      constrain(plungerRead, 0, 32767);
      //Serial.println(plungerRead);
    }
    else if (plungerRead > plungerAtRest)                                                                                               // PUSHING
    {
      plungerTimeOut = 0;
      if (zblaunchflag != 0)                                                                                                            // plunger is in DIGITAL MODE
      {
        if (plungerRead > MaxPush)                                                                                                      // if plunger is being PRESSED
        {
          MaxPush = plungerRead;                                                                                                        // store the farthest distance pressed
          if (GamepadFlag != 1)                                                                                                         // is plunger in gamepad mode (NO)
          {
            Keyboard.pressRaw(LBkey);                                                                                                   // press the keyboard button for the Launch Ball
          }    
          else                                                                                                                          // is plunger in gamepad mode (YES)
          {
            Joystick.pressButton(LaunchBallButton);                                                                                     // set gamepad button used for Launch Ball to ON
          }        
        }
        else if (plungerRead < MaxPush)                                                                                                 // plunger has been RELEASED
        {
          if (GamepadFlag != 1)                                                                                                         // is plunger in gamepad mode (NO)
          {
            Keyboard.releaseRaw(LBkey);                                                                                                 // release the keyboard button for the Launch Ball
          }    
          else                                                                                                                          // is plunger in gamepad mode (YES)
          {
            Joystick.releaseButton(LaunchBallButton);                                                                                   // set gamepad button used for Launch Ball to OFF
          }        
        }
      }
      plungerRead = map(plungerRead,plungerAtRest, plungerAtRest + plungerPushLength, 0, 32767);                                        // set readings to Push Event scale
    }
  } 
  else                                                                                                                                  // plunger is INSIDE DEADZONE 
  {
    plungerTimeOut = plungerTimeOut + 1;                                                                                                // advance the timer counter 
    if (plungerTimeOut > 5)                                                                                                             // Has plunger TIMED OUT
    {
      ClearIndicator();                                                                                                                 // reset indicator lights
      Joystick.rightStick(0);
      plungerGetVal();                                                                                                                  // GoSub and get the current position for recalibration of At Rest point
      plungerAtRest = plungerRead;                                                                                                      // set the new resting position
      plungerTimeOut = 0;                                                                                                               // Reset plunger Time Out timer      
    }
    plungerRead = 0;                                                                                                                    // plunger is either at rest or returning to rest
  } 
  Joystick.rightStick(plungerRead);                                                                                                     // send plunger position
  numberOfLeds = round(plungerRead / 2174.44444475);                                                                                    // numberOfLeds is the number of leds to light on indicator strip
  if (plungerRead != plungerAtRest)                                          
  {
    ShowIndicator();                                                                                                                    // reflect plunger position on indicator
  }  
}


void plungerGetVal (void)
{
  switch (plungerSenseType)
  {
    case 2:
      plungerRead = (analogRead(plungerPotPin) + (plungerAtRest * -1));                                                                   // Current position of plunger modified by offset ANALOG POT SENSOR
      break;
    default:
      plungerRead = plungerSensor.getProximity();                                                           // Current position of plunger modified by offset and movement reversed  IR SENSOR
      break;
  }
}