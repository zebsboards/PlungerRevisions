void Direct_Input_Control (void){

   if (i2c_present > 0){
     if(i2c_present > 1){
      dpad();
     }         
     while (digitalRead(CALSW) == LOW){
      delay(5);
      if ((digitalRead(CALSW) == HIGH)&&((CswTimer > 5)&&(CALflag < 1))){     // switch between keyboard & gamepad based on cal switch quick press
        Gamepadflag = !Gamepadflag;
      }
      else if (CswTimer > 1500){                      // switch into calibration utility based on long button press
        delay(5);
        SetBlinkMode(1);                              // set Blink Speed for led
        CALflag = 1;
      }
     } 
     switch (CALflag){
      case 1:
       Ext_Calib_Plunger();
     }
      
     CALflag = 0;
     CswTimer = 0; 
         
     SetBlinkMode(0);
   } 
   else {
    if ((digitalRead(16) == LOW) && (digitalRead(8) == LOW)) {      //Right flipper and Start
    
      if (Gamepadflag < 1){
       Gamepadflag = 1;                                           // set to gamepad mode
       delay(20);
      }
      else {
       Gamepadflag = 0;                                           // set to keyboard mode
       delay(20);
      }
  }
    
  if ((digitalRead(4) == LOW) && (digitalRead(8) == LOW)) {       //Left Flipper and Start
    delay(20);
    Ext_Calib_Plunger();                                          // goto calibration mode
  }
  
  else{
    EXT_CAL = 0;
  }
   }

}
void dpad (void){

  dp_count=0;
  for(int x = 0; x < 4; x++){
   if (digitalRead(dp_in[x]) == LOW){                    // DPad1   A5 - UP, D7 - RIGHT, D13 - DOWN, D5 - LEFT
    dp_count = x+1;
   }
  }
  if (dp_count > 0){
    dp1 = dp_out[dp_count];
    Pincab_Controller_V2HB.setdpad1(dp1);
  }
  else{
    dp1 = 0;
    dp_count = 0;
    Pincab_Controller_V2HB.setdpad1(dp1);
  }
  dp_count = 0;
  for (int x = 4; x< 8; x++){
   if (digitalRead(dp_in[x]) == LOW){                    // DPad1   A5 - UP, D7 - RIGHT, D13 - DOWN, D5 - LEFT
    dp_count = x+1;
   }
  }
  if (dp_count > 0){
    dp2 = dp_out[dp_count];
    Pincab_Controller_V2HB.setdpad2(dp2);
  }
  else{
    dp2 = 0;
    dp_count = 0;
    Pincab_Controller_V2HB.setdpad2(dp2);
  }
  
  
  
    
}

