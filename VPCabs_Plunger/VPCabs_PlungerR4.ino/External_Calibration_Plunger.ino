void Ext_Calib_Plunger(void){

  EXT_CAL = 1;                                  // set the flag to keep us in the calibration loop

  int ModeInit = Gamepadflag;                   // Store mode state **************** for future use if buttons are used for nudge gain and tilt calibration***************
  
  if(Gamepadflag > 0){                          // if in gamepad mode coming into the routine
    for (int i=0; i<10; i++){
     Pincab_ControllerVPC.setButton(i, LOW);  // turn off all button states
    }
    Pincab_ControllerVPC.sendState();
    delay(10);
  }
 
  else {                                        // if in Keyboard mode coming into the routine
    for (int i=0; i<10; i++){
     Pincab_ControllerVPC.setButton(i, LOW);  // turn off all button states
    }
    Keyboard.release(KEY_LEFT_ARROW);
    Keyboard.release(KEY_LEFT_CTRL);
    Keyboard.release('1');
    Keyboard.release(KEY_ESC);
    Keyboard.release('5');
    Keyboard.release(KEY_RIGHT_ARROW);
    Keyboard.release(KEY_RIGHT_CTRL);
    Keyboard.release(0xB0);
    
    //Pincab_ControllerVPC.setButton(16, LOW);  // Release Gamepad Burron 1
    //Pincab_ControllerVPC.setButton(17, LOW);  // Release Gamepad Button 2
    
    Gamepadflag = 1;                            // switch to gamepad mode
  }

  int pull = 0;
  int push = 0;

  zmaxSet = 0;                          //plunger calibration max (pull length) value
  zminSet = z_centered;                 //plunger calibration min (push length) value set to rest position by default

  int addr = 0;                         // eeprom starting address
  
  
  while(EXT_CAL > 0){

    for(int i= 0; i<4; i++){
    Pincab_ControllerVPC.setButton((i), HIGH);
    }
    /*for(int i= 9; i<15; i++){
    Pincab_ControllerVPC.setButton((i), HIGH);
    }
    for(int i= 18; i<22; i++){
    Pincab_ControllerVPC.setButton((i), HIGH);
    }
    for(int i= 27; i<29; i++){
    Pincab_ControllerVPC.setButton((i), HIGH);
    }*/
        
    Pincab_ControllerVPC.sendState();
    
    while (pull == 0){  //Calibrate plunger MAX (Pull)

     Pincab_ControllerVPC.sendState();
      
     analog_Z.update();  // read the raw plunger data

     NEW_Pos = (analog_Z.getValue()/4);    // use the smoothed value 0-1024 (10bit) brought down to 0-255 scale (8bit) and apply calibration
     
     if (NEW_Pos > (z_centered + 5) || NEW_Pos < (z_centered - 5)) {    //deadzone to accomodate plunger fluctuation at static rest

         NEW_Pos = NEW_Pos;                                 //send the realtime readings
     }
     else  {
         NEW_Pos = z_rest;                                //send the center position
     }
     if (NEW_Pos != OLD_Pos) {                            // if plunger position has changed

         if (NEW_Pos < z_centered) {                          // if plunger being pressed

           for (int f = 0; f < 3; f++) {
            for (int i=0; i<10; i++){
             Pincab_ControllerVPC.setButton(i, LOW);
            }
            Pincab_ControllerVPC.sendState();
            delay(250);
                    
            for(int i= 0; i<4; i++){
             Pincab_ControllerVPC.setButton((i), HIGH);         //ERROR
            }
            /*for(int i= 9; i<15; i++){
             Pincab_ControllerVPC.setButton((i), HIGH);
            }                                                     // flash UP arrow
            for(int i= 18; i<22; i++){
             Pincab_ControllerVPC.setButton((i), HIGH);
            }
            for(int i= 27; i<29; i++){
             Pincab_ControllerVPC.setButton((i), HIGH);
            }*/
            Pincab_ControllerVPC.sendState();               
            delay(250);
         } 
        }
        if ((NEW_Pos > z_centered)&&(NEW_Pos > zmaxSet)) {                              // if plunger being drawn back
             zmaxSet = NEW_Pos;
        }
        if ((NEW_Pos > z_centered)&&(NEW_Pos < zmaxSet)){                               // if plunger is maxxed out
             EEPROM.write(addr, zmaxSet);                                                  //write plunger zmaxSet value to eeprom
             delay(100);
             pull = 1;                                                                     // set bit to leave the loop
        }
       }
     
     OLD_Pos = NEW_Pos;    // update the reference position
          
    }

    for (int i=0; i<10; i++){
        Pincab_ControllerVPC.setButton(i, LOW);               // clear the indicator lights
    }

    delay(1000);

          //Pincab_ControllerVPC.setButton(3, HIGH);
          //Pincab_ControllerVPC.setButton(4, HIGH);
          for(int i= 4; i<8; i++){
             Pincab_ControllerVPC.setButton((i), HIGH);
          }
          /*for(int i= 17; i<23; i++){
             Pincab_ControllerVPC.setButton((i), HIGH);
          }
          for(int i= 27; i<29; i++){
             Pincab_ControllerVPC.setButton((i), HIGH);
          } */                
          

    delay(1000);

    while (push == 0){  //Calibrate plunger MIN (Push)**************************************************************************************************
      
      
     Pincab_ControllerVPC.sendState();
      
     analog_Z.update();  // read the raw plunger data

     NEW_Pos = (analog_Z.getValue()/4);    // use the smoothed value 0-1024 (10bit) brought down to 0-255 scale (8bit) and apply the offset
          
     if (NEW_Pos != OLD_Pos) {                            // if plunger position has changed

         if (NEW_Pos > z_centered) {                          // if plunger being pulled

          for (int f = 0; f < 3; f++) {
            for (int i=0; i<10; i++){
              Pincab_ControllerVPC.setButton(i, LOW);
            }
            Pincab_ControllerVPC.sendState();
            delay(250);
          
            //Pincab_ControllerVPC.setButton(3, HIGH);
            //Pincab_ControllerVPC.setButton(4, HIGH);
            for(int i= 4; i<8; i++){
             Pincab_ControllerVPC.setButton((i), HIGH);
            }                                                     // flash direction arrow
            /*for(int i= 17; i<23; i++){
             Pincab_ControllerVPC.setButton((i), HIGH);
            }
            for(int i= 27; i<29; i++){
             Pincab_ControllerVPC.setButton((i), HIGH);
            }*/
            Pincab_ControllerVPC.sendState();               
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

    for (int z=0; z < 2; z++){
    for (int i=0; i<11; i++){
        Pincab_ControllerVPC.setButton(i, LOW);         /// clear the indicator panel
    }

    delay(500);

    for (int i=0; i<8; i++){
        Pincab_ControllerVPC.setButton(i, HIGH);         /// indicate 1 - 8 (flash
    }

    delay(500);
    }

    

    /*Pincab_ControllerVPC.setButton(1, HIGH);
    Pincab_ControllerVPC.setButton(2, HIGH);
    Pincab_ControllerVPC.setButton(3, HIGH);
    Pincab_ControllerVPC.setButton(5, HIGH);
    Pincab_ControllerVPC.setButton(7, HIGH);
    Pincab_ControllerVPC.setButton(9, HIGH);
    Pincab_ControllerVPC.setButton(11, HIGH);
    Pincab_ControllerVPC.setButton(13, HIGH);
    Pincab_ControllerVPC.setButton(14, HIGH);
    Pincab_ControllerVPC.setButton(17, HIGH);           // display OK
    Pincab_ControllerVPC.setButton(19, HIGH);
    Pincab_ControllerVPC.setButton(21, HIGH);
    Pincab_ControllerVPC.setButton(22, HIGH);
    Pincab_ControllerVPC.setButton(25, HIGH);
    Pincab_ControllerVPC.setButton(26, HIGH);
    Pincab_ControllerVPC.setButton(27, HIGH);
    Pincab_ControllerVPC.setButton(29, HIGH);
    Pincab_ControllerVPC.setButton(31, HIGH);*/
    
    Pincab_ControllerVPC.sendState();
    
    delay(500);

    for (int i=0; i<11; i++){
              Pincab_ControllerVPC.setButton(i, LOW);
    }
    Pincab_ControllerVPC.sendState();
    
    Gamepadflag = ModeInit;                               // restore previous mode
    
    EXT_CAL = 0;                                          // exit loop
    
  }
}



