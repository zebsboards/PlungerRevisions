void Ext_Calib_Plunger(void){
  
  delay(500);
  
  EXT_CAL = 1;                                  // set the flag to keep us in the calibration loop

  int ModeInit = Gamepadflag;                   // Store mode state **************** for future use if buttons are used for nudge gain and tilt calibration***************
  
  if(Gamepadflag > 0){                          // if in gamepad mode coming into the routine
    for (int i=0; i<32; i++){
     Pincab_Controller_V2HB.setButton(i, LOW);  // turn off all button states
    }
    Pincab_Controller_V2HB.sendState();
    delay(10);
  }
 
  else {                                        // if in Keyboard mode coming into the routine
    for (int i=0; i<16; i++){
     Keyboard.release(keys[i]);                 // Release the keypress (if any)
    }
    Pincab_Controller_V2HB.setButton(16, LOW);  // Release Gamepad Button 1
    Pincab_Controller_V2HB.setButton(17, LOW);  // Release Gamepad Button 2
    
    Gamepadflag = 1;                            // switch to gamepad mode
  }

  int pull = 0;
  int push = 0;

  zmaxSet = 0;                                  // plunger calibration max (pull length) value
  zminSet = z_centered;                         // plunger calibration min (push length) value set to rest position by default

  int addr = 67;                                // eeprom starting address (starting at address 67 after eeprom flag,32 buttons,32 button usage,Tilt enable flag,launch ball button assignment)
  
  
  while(EXT_CAL > 0){

    
     Blink();                                      // Start blinking
    
    
    if (digitalRead(CALSW) == LOW){
      SetBlinkMode(0); 
      zmaxSet=EEPROM.read(67);
      zminSet=EEPROM.read(68);
      break;
    }
    if ((digitalRead(4) == LOW) && (digitalRead(8) == LOW)) {       //Left Flipper and Start
      zmaxSet=EEPROM.read(67);
      zminSet=EEPROM.read(68);
      break;
     }  
    
    for(int i= 3; i<5; i++){
    Pincab_Controller_V2HB.setButton((i), HIGH);
    }
    for(int i= 9; i<15; i++){
    Pincab_Controller_V2HB.setButton((i), HIGH);
    }
    for(int i= 18; i<22; i++){
    Pincab_Controller_V2HB.setButton((i), HIGH);
    }
    for(int i= 27; i<29; i++){
    Pincab_Controller_V2HB.setButton((i), HIGH);
    }
        
    Pincab_Controller_V2HB.sendState();
    
    while (pull == 0){  //Calibrate plunger MAX (Pull)

     
     if (digitalRead(CALSW) == LOW){
      SetBlinkMode(0); 
      zmaxSet=EEPROM.read(67);
      zminSet=EEPROM.read(68);
      break;
     }
     if ((digitalRead(4) == LOW) && (digitalRead(8) == LOW)) {       //Left Flipper and Start
      zmaxSet=EEPROM.read(67);
      zminSet=EEPROM.read(68);
      break;
     }  
     
     Pincab_Controller_V2HB.sendState();
      
     analog_Z.update();  // read the raw plunger data

     NEW_Pos = map(analog_Z.getValue(),0,512,0,255);                   // use the smoothed value 0-1024 (10bit) brought down to 0-255 scale (8bit) and apply calibration
     
     if (NEW_Pos > (z_centered + 5) || NEW_Pos < (z_centered - 5)) {    // deadzone to accomodate plunger fluctuation at static rest

         NEW_Pos = NEW_Pos;                                             // send the realtime readings
     }
     else  {
         NEW_Pos = z_rest;                                              // send the center position
     }
     if (NEW_Pos != OLD_Pos) {                                          // if plunger position has changed

         if (NEW_Pos < z_centered - 10) {                                    // if plunger being pressed

           for (int f = 0; f < 3; f++) {
            for (int i=0; i<32; i++){
             Pincab_Controller_V2HB.setButton(i, LOW);
            }
            Pincab_Controller_V2HB.sendState();
            delay(250);
                    
            for(int i= 3; i<5; i++){
             Pincab_Controller_V2HB.setButton((i), HIGH);               // ERROR
            }
            for(int i= 9; i<15; i++){
             Pincab_Controller_V2HB.setButton((i), HIGH);
            }                                                           // flash UP arrow
            for(int i= 18; i<22; i++){
             Pincab_Controller_V2HB.setButton((i), HIGH);
            }
            for(int i= 27; i<29; i++){
             Pincab_Controller_V2HB.setButton((i), HIGH);
            }
            Pincab_Controller_V2HB.sendState();               
            delay(250);
         } 
        }
        if ((NEW_Pos > z_centered)&&(NEW_Pos > zmaxSet)) {              // if plunger being drawn back
             zmaxSet = NEW_Pos;
        }
        if ((NEW_Pos > z_centered)&&(NEW_Pos < zmaxSet)){               // if plunger is maxxed out
             EEPROM.write(addr, zmaxSet);                               // write plunger zmaxSet value to eeprom
             delay(100);
             pull = 1;                                                  // set bit to leave the loop
        }
       }
     
     OLD_Pos = NEW_Pos;                                                 // update the reference position
          
    }

    for (int i=0; i<32; i++){
        Pincab_Controller_V2HB.setButton(i, LOW);                       // clear the indicator lights
    }

    delay(1000);

          Pincab_Controller_V2HB.setButton(3, HIGH);
          Pincab_Controller_V2HB.setButton(4, HIGH);
          for(int i= 10; i<14; i++){
             Pincab_Controller_V2HB.setButton((i), HIGH);
          }
          for(int i= 17; i<23; i++){
             Pincab_Controller_V2HB.setButton((i), HIGH);
          }
          for(int i= 27; i<29; i++){
             Pincab_Controller_V2HB.setButton((i), HIGH);
          }                 
          

    delay(1000);
    analog_Z.update();  // read the raw plunger data
    NEW_Pos = map(analog_Z.getValue(),0,512,0,255);    // use the smoothed value 0-1024 (10bit) brought down to 0-255 scale (8bit) and apply the offset
    zminSet = NEW_Pos;
    
    while (push == 0){  //Calibrate plunger MIN (Push)**************************************************************************************************
           
     Blink();                                      // Start blinking
     
     if (digitalRead(CALSW) == LOW){
      SetBlinkMode(0); 
      zmaxSet = EEPROM.read(67);
      zminSet = EEPROM.read(68);
      break;
     } 
     if ((digitalRead(4) == LOW) && (digitalRead(8) == LOW)) {       //Left Flipper and Start
      zmaxSet=EEPROM.read(67);
      zminSet=EEPROM.read(68);
      break;
     }  
        
     Pincab_Controller_V2HB.sendState();

      
     analog_Z.update();  // read the raw plunger data

     NEW_Pos = map(analog_Z.getValue(),0,512,0,255);    // use the smoothed value 0-1024 (10bit) brought down to 0-255 scale (8bit) and apply the offset
     if (NEW_Pos > (z_centered + 5) || NEW_Pos < (z_centered - 5)) {    // deadzone to accomodate plunger fluctuation at static rest

         NEW_Pos = NEW_Pos;                                             // send the realtime readings
     }
     else  {
         NEW_Pos = z_rest;                                              // send the center position
     }     
     if (NEW_Pos != OLD_Pos) {                            // if plunger position has changed

         if (NEW_Pos > z_centered) {                          // if plunger being pulled

          for (int f = 0; f < 3; f++) {
            for (int i=0; i<32; i++){
              Pincab_Controller_V2HB.setButton(i, LOW);
            }
            Pincab_Controller_V2HB.sendState();
            delay(250);
          
            Pincab_Controller_V2HB.setButton(3, HIGH);
            Pincab_Controller_V2HB.setButton(4, HIGH);
            for(int i= 10; i<14; i++){
             Pincab_Controller_V2HB.setButton((i), HIGH);
            }                                                     // flash direction arrow
            for(int i= 17; i<23; i++){
             Pincab_Controller_V2HB.setButton((i), HIGH);
            }
            for(int i= 27; i<29; i++){
             Pincab_Controller_V2HB.setButton((i), HIGH);
            }
            Pincab_Controller_V2HB.sendState();               
            delay(250);
          }             
        }
        else{
          if (NEW_Pos < zminSet) {        // if plunger being pushed in
             zminSet = NEW_Pos;
          }
          if (NEW_Pos > zminSet){                                                   // if plunger is returning to rest
            addr = addr + 1;                                                        //increment the eeprom address location by one
            if (addr == EEPROM.length()) {                                          //prevent overflow (should be impossible)
              addr = 0;
            }
            EEPROM.write(addr, zminSet);                                            //write plunger zminSet value to eeprom
            delay(100);
            push = 1;                                                               // set bit to leave the loop
          }
       }
     }
          
     OLD_Pos = NEW_Pos;    // update the reference position
     
    }

    for (int i=0; i<32; i++){
        Pincab_Controller_V2HB.setButton(i, LOW);         /// clear the indicator panel
    }

    Pincab_Controller_V2HB.setButton(1, HIGH);
    Pincab_Controller_V2HB.setButton(2, HIGH);
    Pincab_Controller_V2HB.setButton(3, HIGH);
    Pincab_Controller_V2HB.setButton(5, HIGH);
    Pincab_Controller_V2HB.setButton(7, HIGH);
    Pincab_Controller_V2HB.setButton(9, HIGH);
    Pincab_Controller_V2HB.setButton(11, HIGH);
    Pincab_Controller_V2HB.setButton(13, HIGH);
    Pincab_Controller_V2HB.setButton(14, HIGH);
    Pincab_Controller_V2HB.setButton(17, HIGH);           // display OK
    Pincab_Controller_V2HB.setButton(19, HIGH);
    Pincab_Controller_V2HB.setButton(21, HIGH);
    Pincab_Controller_V2HB.setButton(22, HIGH);
    Pincab_Controller_V2HB.setButton(25, HIGH);
    Pincab_Controller_V2HB.setButton(26, HIGH);
    Pincab_Controller_V2HB.setButton(27, HIGH);
    Pincab_Controller_V2HB.setButton(29, HIGH);
    Pincab_Controller_V2HB.setButton(31, HIGH);
    Pincab_Controller_V2HB.sendState();
    
    delay(3000);

    for (int i=0; i<32; i++){
              Pincab_Controller_V2HB.setButton(i, LOW);
    }
    Pincab_Controller_V2HB.sendState();
    
    Gamepadflag = ModeInit;                               // restore previous mode
    
    d1 = 0;
    d2 = 0;
    EXT_CAL = 0;                                          // exit loop
    
  }

  pull = 0;
  push = 0;
  
  SetBlinkMode(0);                                        // set Blink Speed for led
}



