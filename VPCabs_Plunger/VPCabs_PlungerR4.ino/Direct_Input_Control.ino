void Direct_Input_Control (void){

  if ((digitalRead(5) == LOW) && (digitalRead(8) == LOW)) {      //Right flipper and Start
    
    if (Gamepadflag < 1){
       Gamepadflag = 1;                                           // set to gamepad mode
       delay(10);
    }
    else {
       Gamepadflag = 0;                                           // set to keyboard mode
       delay(10);
    }
  }
    
  if ((digitalRead(4) == LOW) && (digitalRead(8) == LOW)) {       //Left Flipper and Start
    
    Ext_Calib_Plunger();                                          // goto calibration mode
  }
  
  else{
    EXT_CAL = 0;
  }
  
}

