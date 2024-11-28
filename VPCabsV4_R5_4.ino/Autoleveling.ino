void autoleveling(void) { //************************************************ Accelerometer Calibration **********************************************

        

       if ((i2c_present == 0)||(i2c_present == 2)){

       analog_X.update();    //sample the x axis
       analog_Y.update();    //sample the y axis
       int x_centered = analog_X.getValue();   // get the average reading
       int y_centered = analog_Y.getValue();
       analog_Z.update();
       z_centered = map(analog_Z.getValue(), 0, 1024, 0, 255);
       


       x_calib = (x_centered*-1); // create an offset constant for the axes
       y_calib = (y_centered*-1);
       z_calib = (z_centered*-1);

       x_rest = (x_centered + x_calib);        // x axis at 0
       y_rest = (y_centered + y_calib);        // y axis at 0
       z_rest = round(z_centered + z_calib);        // z axis at 0
       }

       else{
         while (counter < counter_end) {
          i2c_sense();
          counter = counter + 1;             //advance the counter
         }
         int x_centered = (x_read / counter);   // get the average reading
         int y_centered = (y_read / counter);
         analog_Z.update();
         z_centered = map(analog_Z.getValue(), 0, 1024, 0, 255);
       
         x_calib = (x_centered*-1); // create an offset constant for the axes
         y_calib = (y_centered*-1);
         z_calib = (z_centered*-1);

         x_rest = (x_centered + x_calib);        // x axis at 0
         y_rest = (y_centered + y_calib);        // y axis at 0
         z_rest = round(z_centered + z_calib);        // z axis at 0
       } 
 
}



void i2c_sense (void){

           accelX = (accel.getRawX()*2);
           accelY = (accel.getRawY()*2);
             
           x_read = x_read + accelX;          //accumulate the totals
           y_read = y_read + accelY;
          
  
}




