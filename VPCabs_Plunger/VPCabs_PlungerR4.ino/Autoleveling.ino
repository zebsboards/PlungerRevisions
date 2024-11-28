void autoleveling(void) { //************************************************ Analog Calibration **********************************************

  while (buffer < buffer_end) {      //initialization routine for accelerometer
    analogRead(0);                     //dummy values that are not used anywhere else
    analogRead(1);                     //
    buffer = buffer + 1;               //advance the count
  }

  while (counter < counter_end) {

    analog_X.update();                 //sample the axis value
    analog_Y.update();                 //sample the axis value
    analog_Z.update();                 //sample the axis value

    x_init = (analog_X.getValue()/4);    //assign the smoothed axis value
    x_read = x_read + x_init;          //accumulate the totals
    y_init = (analog_Y.getValue()/4);
    y_read = y_read + y_init;
    z_init = (analog_Z.getValue()/4);
    z_read = z_read + z_init;

    counter = counter + 1;             //advance the counter
  }

  x_centered = (x_read / 30);   // get the average reading
  y_centered = (y_read / 30);
  z_centered = (z_read / 30);


  x_calib = (0 - x_centered); // create an offset constant for the axes
  y_calib = (0 - y_centered);
  z_calib = (0 - z_centered);

  x_rest = round(x_centered + x_calib);        // x axis at 0
  y_rest = round(y_centered + y_calib);        // y axis at 0
  z_rest = round(z_centered + z_calib);        // z axis at 0

} 

