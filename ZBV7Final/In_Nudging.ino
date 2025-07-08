elapsedMillis tiltTime;                      // TILT activation debounce timer

void Nudging (void) {
  int16_t accelXtmp = (Nudge.getRawX());
  int16_t accelYtmp = (Nudge.getRawY());
  int16_t accelZtmp = (Nudge.getRawZ());
  accelXtmp = (accelXtmp + NuXatRest);
  accelYtmp = (accelYtmp + NuYatRest);
  accelZtmp = (accelZtmp + NuZatRest);
  int16_t accelX;
  int16_t accelY;
  switch (NuOrientation)
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
  if (accelX > NuXdeadzone || (accelX < NuXdeadzone * -1)) {                                                        // deadzone to accomodate accelerometer fluctuation at static rest                                                         
    accelX = map(accelX,-16450,16450,-32767,32767);                                                             
    accelX = accelX * NuXGain;
    if (accelX < -32767) { accelX = -32767;}
    else if (accelX > 32767){ accelX = 32767;}
  }
  else {
   accelX = 0;                                                                                                      // send the 0 position
  }
  if (accelY > NuYdeadzone || (accelY < NuYdeadzone * -1)) {                                                        // deadzone to accomodate accelerometer fluctuation at static rest
    accelY = map(accelY,-16450,16450,-32767,32767);                                                             
    accelY = accelY * NuYGain;
    if (accelY < -32767) { accelY = -32767;}
    else if (accelY > 32767) { accelY = 32767;}
  }
  else {
    accelY = 0;                                                                                                     // send the 0 position
  }
  ZBV7.setXAxis(accelX);                                                                                            // Set the nudge position (x axis, y axis)
  ZBV7.setYAxis(accelY);                                                                                            // Set the nudge position (x axis, y axis)
  if (UseTilt == 1) {                                                                                               // if using the Tilt feature
    if ((accelX > TiltXval) || (accelX < (TiltXval * -1)) || (accelY > TiltYval) || (accelY < (TiltYval * -1))) {   // if tilt has been activated
      if (tiltTime > BounceTime) {                                                                                  // restricting the tilt alert to once every 0 - 2.5sec
        ZBV7.setButton(TiltButton - 1, HIGH);                                                                       // set Tilt Button ON
        tiltTime = 0;
      }
    }
    else {
      ZBV7.setButton(TiltButton - 1, LOW);                                                                          // set Tilt Button OFF (blocks physical input switching when TiltInUse is active) 
    }
  }    
}