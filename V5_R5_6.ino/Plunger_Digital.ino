void DigitalballLaunch (void) {  // ********************** Plunger as Enter Key sub routine ********************

  

  pl_z = map((NEW_Pos), zminSet, z_centered, 0,-127); // scale plunger actual reading (-) to full scale reading (+) due to smaller travel distance
  constrain(pl_z, 0, -127);

  pl_zrot = pl_z * -1;

  if ((NEW_Pos < 0)&&(NEW_Pos <= z_min)) {                    // if plunger is pressed

    if (Gamepadflag > 0) {
      Pincab_Controller_V2HB.setButton(LB_Pos, HIGH);
      z_min = NEW_Pos;
    }
    if (Gamepadflag < 1) {
      Keyboard.press (LB_VAL);            // Press Enter Key
      z_min = NEW_Pos;
    }
  }
  if (NEW_Pos > z_min)  {              // if plunger released

    if (Gamepadflag > 0) {
      Pincab_Controller_V2HB.setButton(LB_Pos, LOW);
      rel_timer = 0;
      z_released = 1;
    }
    if (Gamepadflag < 1) {
      Keyboard.release (LB_VAL);         // Release Enter Key
      rel_timer = 0;
      z_released = 1;
    }
    if (NEW_Pos > -1) {
      z_min = 0;
    }
  }
}
