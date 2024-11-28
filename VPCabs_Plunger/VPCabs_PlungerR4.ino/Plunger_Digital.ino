void DigitalballLaunch (void) {  // ********************** Plunger as Enter Key sub routine ********************

  

  pl_z = map(NEW_Pos, zminSet, z_centered, 0,-127); // scale plunger actual reading (-) to full scale reading (+) due to smaller travel distance
  constrain(pl_z, 0, -127);

  pl_zrot = pl_z * -1;

  if ((NEW_Pos < 0)&&(NEW_Pos < z_max)) {                    // if plunger is pressed

    //Serial.print(NEW_Pos), Serial.print("          "), Serial.println(z_max);

    if (Gamepadflag > 0) {
      //Keyboard.press (0xB0);          // Press Enter Key
      Pincab_ControllerVPC.setButton(31, HIGH);
      z_max = NEW_Pos;
    }
    if (Gamepadflag < 1) {
      Keyboard.press (0xB0);            // Press Enter Key
      //Pincab_ControllerVPC.setButton(31, HIGH);
      z_max = NEW_Pos;
    }
  }
  if (NEW_Pos > z_max)  {              // if plunger released

    //Serial.print(NEW_Pos), Serial.print("          "), Serial.println(z_max);

    if (Gamepadflag > 0) {
      //Keyboard.release (0xB0);       // Release Enter Key
      Pincab_ControllerVPC.setButton(31, LOW);
      rel_timer = 0;
      z_released = 1;
    }
    if (Gamepadflag < 1) {
      Keyboard.release (0xB0);         // Release Enter Key
      //Pincab_ControllerVPC.setButton(31, LOW);
      rel_timer = 0;
      z_released = 1;
    }
    if (NEW_Pos > -1) {
      z_max = 0;
    }
  }
}
