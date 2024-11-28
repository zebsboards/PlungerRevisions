void NudgingRoutine (void) {  // ************************* Nudging sub routine ************************************

  analog_G.update();                                    // read analog gain signal
  if ((i2c_present == 0)||(i2c_present == 2)){
   nGainFinal = map(analog_G.getValue(),0,1024,400,1);     // scale gain value to 10% to 130%
  }
  else {
   nGainFinal = map(analog_G.getValue(),0,1024,100,1);     // scale gain value to 10% to 100% 
  }
  nGainFinal = nGainFinal*.01;

  //***************** calculate nudging values from raw readings ********************************
  
  if ((i2c_present == 0)||(i2c_present == 2)){
    analog_accel();  
  }
  else {
    i2c_accel();
  }
    
  if (tilt_enable == 1){
    // read the TILT setting and verify that we aren't tilting the machine
  
    analog_T.update();                              // read analog TILT signal
    tilt_RAW = (analog_T.getValue());               // assign TILT variable signal to TILT threshold
    tilt_VAL = map(tilt_RAW, 0, 1024, 0, 32768);      // map final reading to scale of 0 - 127

    if ((x_out > tilt_VAL) || (x_out < (tilt_VAL * -1)) || (y_out > tilt_VAL) || (y_out < (tilt_VAL * -1))) {     // if tilt has been activated

      if (tiltTime > 1000) {                          // restricting the tilt alert to once every 1sec
        Pincab_Controller_V2HB.setButton(tilt_button, HIGH);   // send button TB on command as indicator
        tiltTime = 0;
      }
    }
    else {
      Pincab_Controller_V2HB.setButton(tilt_button, LOW);
    }
  }  
}

void analog_accel (void) {

// Read the i2c nudge sensor and apply calibration to nudge position (x axis, y axis)
  
    analog_X.update();    //sample the x axis
    analog_Y.update();    //sample the y axis

    //accelX = analog_X.getValue()+ (x_rest*-1);
    //accelY = analog_Y.getValue()+ (y_rest*-1);
    accelX = analog_X.getValue()+ x_calib;
    accelY = analog_Y.getValue()+ y_calib;

    if (accelX > (x_rest+5) || accelX < (x_rest-5)) {    //deadzone to accomodate accelerometer fluctuation at static rest
        
        x_out = map(accelX,1024,0,-32768,32767);                   //send the realtime readings
        x_out = x_out*nGainFinal;
    }
    else  {
     x_out = 0;                                //send the 0 position
    }
    if (accelY > (y_rest+5) || accelY < (y_rest-5)) {    //deadzone to accomodate accelerometer fluctuation at static rest
        y_out = map(accelY,1024,0,-32768,32767);                 //send the realtime readings
        y_out = y_out*nGainFinal;
    }
    else  {
      y_out = 0;                          //send the 0 position
    }
  
}



void i2c_accel (void) {

// Read the i2c nudge sensor and apply calibration to nudge position (x axis, y axis)

    accelX = (accel.getRawX()*2);
    accelY = (accel.getRawY()*2);
    
    accelX = (accelX + x_calib);
    accelY = (accelY + y_calib);
         
    if (accelX > (x_rest + 2000) || accelX < (x_rest - 2000)) {    //deadzone to accomodate accelerometer fluctuation at static rest
      x_out = map(accelX,-16450,16450,-32768,32767);                 //send the realtime readings
      x_out = x_out*nGainFinal;
    }
    else  {
     x_out = 0;                                //send the 0 position
    }
    if (accelY > (y_rest + 2000) || accelY < (y_rest - 2000)) {    //deadzone to accomodate accelerometer fluctuation at static rest
       y_out = map(accelY,-16450,16450,-32768,32767);                 //send the realtime readings
       y_out = y_out*nGainFinal;
    }
    else  {
      y_out = 0;                                //send the 0 position
    }  
  
}

