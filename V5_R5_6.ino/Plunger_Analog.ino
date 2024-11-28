

void AnalogPlunger (void) {  // *************************** Analog Plunger sub Routine *************************


  //  1st step - determine push or pull event

  if ((NEW_Pos > OLD_Pos) && (Pull_Flag < 1) && (Push_Flag < 1)) {

    Push_Flag = 0;
    Pull_Flag = 1;

  }
  if (Pull_Flag > 0) {                                                           // Pull Flag = 1

    //  2nd step - factor plunger movement to axis scale
        
    if (NEW_Pos > 0){
      pl_z = map((NEW_Pos - z_calib), z_centered, zmaxSet, 0, 127);

      if ((NEW_Pos > OLD_Pos) && (z_released < 1)) {
        z_released = 0;
        rel_timer = 0;                                                            // reset release timer variables and flags
        counter = 0;
        
        z_max = NEW_Pos;                                                           // set max pullback value
        constrain(z_max, 0, 127);
      }
      if ((NEW_Pos < round(z_max * .25)) && (z_released < 1)) {                 // Set the release flag as plunger is released
        z_released = 1;   // plunger released, set flag
        rel_timer = 0;  // start timer
      }
    }

    if (NEW_Pos < 0) {                                                         // release has gone below 0
      pl_z =  NEW_Pos;
    }

    pl_zrot = pl_z;
    
    
    
  }

  if ((NEW_Pos < OLD_Pos) && (Pull_Flag < 1) && (Push_Flag < 1)) {

    Push_Flag = 1;
    Pull_Flag = 0;

  }
  if (Push_Flag > 0) {    // Push Flag = 1

    pl_z = map((NEW_Pos), zminSet, z_centered, 0,127)*-1;
    constrain(pl_z, 0,127);
            
    if (NEW_Pos < OLD_Pos) {
      z_max = NEW_Pos;  // set max push travel value
      constrain(z_max, zminSet, z_centered);
    }
    if ((NEW_Pos > z_max) && (z_released < 1)) {
      z_released = 1;   // plunger released, set flag
      rel_timer = 0;  // start timer
    }

    pl_zrot = pl_z;
  }
  return;
}
