void Nudge (void)
{
  int16_t accelXtmp = (accel.getRawX()*2);
  int16_t accelYtmp = (accel.getRawY()*2);
  int16_t accelZtmp = (accel.getRawZ()*2);
  accelXtmp = (accelXtmp + x_offset);
  accelYtmp = (accelYtmp + y_offset);
  accelZtmp = (accelZtmp + z_offset);
  int16_t accelX;
  int16_t accelY;
  switch (accelOrientation)
  {
    case 0:                                                                                                         // LSCDown
      accelX = accelZtmp;                   
      accelY = accelYtmp;
      break;
    case 1:                                                                                                         // CFCDown
      accelX = accelYtmp;
      accelY = accelZtmp;
      break;
    case 2:                                                                                                         // CBCDown
      accelX = accelYtmp * -1;
      accelY = accelZtmp * -1;  
      break;
    case 3:                                                                                                         // RSCDown
      accelX = accelZtmp * -1;
      accelY = accelXtmp * -1;
      break;
    case 4:                                                                                                         // LSCFlat mount
      accelX = accelXtmp;
      accelY = accelYtmp * -1;
      break; 
    case 5:                                                                                                         // CFFlat mount
      accelX = accelYtmp * -1;                                                                                      
      accelY = accelXtmp * -1;                                                                                      
      break;
    case 6:                                                                                                         // RSCFlat mount
      accelX = accelXtmp * -1;                                                                                      
      accelY = accelYtmp;
      break;
    default:                                                                                                        // CBFlat mount
      accelX = accelYtmp;
      accelY = accelXtmp;                                                                                           
      break;              
  }
  if (accelX > Xdeadzone || (accelX < Xdeadzone * -1)) 
  {                                                                                                                 // deadzone to accomodate accelerometer fluctuation at static rest
    accelX = map(accelX,-16450,16450,-32767,32767);                                                             
    accelX = accelX * XGain;
    if (accelX < -32767)
    { accelX = -32767;}
    else if (accelX > 32767)
    { accelX = 32767;}
  }
  else  
  {
   accelX = 0;                                                                                                      // send the 0 position
  }
  if (accelY > Ydeadzone || (accelY < Ydeadzone * -1))
  {                                                                                                                 // deadzone to accomodate accelerometer fluctuation at static rest
    accelY = map(accelY,-16450,16450,-32767,32767);                                                             
    accelY = accelY * YGain;
    if (accelY < -32767)
    { accelY = -32767;}
    else if (accelY > 32767)
    { accelY = 32767;}
  }
  else  
  {
    accelY = 0;                                                                                                     // send the 0 position
  }
  Joystick.leftStick(accelX, accelY);                                                                                        // Set the nudge position (x axis, y axis)
  if (tilt_enable == 1)
  {
    if ((accelX > tilt_VALX) || (accelX < (tilt_VALX * -1)) || (accelY > tilt_VALY) || (accelY < (tilt_VALY * -1)))   // if tilt has been activated
    {   
      if (tiltTime > 1000) 
      {                                                                                                           // restricting the tilt alert to once every 1sec
        Joystick.pressButton(tilt_button - 1);                                                                  // send button TB on command as indicator
        tiltTime = 0;
      }
    }
    else 
    {
      Joystick.releaseButton(tilt_button - 1);
    }
  }
}    

