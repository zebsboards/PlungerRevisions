void NudgingRoutine (void) {  // ************************* Nudging sub routine ************************************

  analog_G.update();                                    // read analog gain signal
  nGain = (analog_G.getValue()/4);                      // scale gain value to 0 - 102
  nGainFinal = (nGain * .01);                           // calculate final gain value to be applied to nudging output values

  // Read the nudge sensor and apply calibration to nudge position (x axis, y axis)

  analog_X.update();    //sample the x axis
  analog_Y.update();    //sample the y axis

  x_ref = (analog_X.getValue()/4);   //use the smoothed values
  y_ref = (analog_Y.getValue()/4);   // and rescale to 8bit (0-255)

  x_raw = round(x_ref + x_calib);
  y_raw = round(y_ref + y_calib);

  if (x_ref > (x_rest + 5) || x_ref < (x_rest - 5)) {    //deadzone to accomodate accelerometer fluctuation at static rest
    x_out = round(x_raw * nGainFinal);                                   //send the realtime readings
  }

  else  {
    x_out = x_rest;                                //send the 0 position
  }

  if (y_ref > (y_rest + 5) || y_ref < (y_rest - 5)) {
    y_out = round(y_raw * nGainFinal);
  }

  else  {
    y_out = y_rest;                                //send the 0 position
  }

  // read the TILT setting and verify that we aren't tilting the machine

  analog_T.update();                              // read analog TILT signal
  tilt_RAW = (analog_T.getValue());               // assign TILT variable signal to TILT threshold
  tilt_VAL = map(tilt_RAW, 0, 1024, 0, 128);      // map final reading to scale of 0 - 127

  if ((x_out > tilt_VAL) || (x_out < (tilt_VAL * -1)) || (y_out > tilt_VAL) || (y_out < (tilt_VAL * -1))) {     // if tilt has been activated

    if (millis() - tilt_time > 1000) {     // restricting the tilt alert to once every 1sec
      Pincab_ControllerVPC.setButton(10, HIGH);  // send button 30 on command as indicator
      tilt_time = millis();
      delay(100);
    }
    
  }
  else {
      Pincab_ControllerVPC.setButton(10, LOW);
  }

}
