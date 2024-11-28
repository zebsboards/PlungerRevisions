void KeyAssign (void){
  if (EEPROM.read(0) != 1){                     // no eeprom values present, write default values
   if (i2c_present == 1){ 
    for (int k=0; k < 32; k++){                   // routine should be run only once ever in lifespan of firmware
      int addr = k+1;
      if (addr == EEPROM.length()) {              // prevent overflow (should be impossible)
        addr = 0;
      }
      EEPROM.write(addr, keysNew[k]);
      keys[k] = keysNew[k];
      delay(50);
      if (keysNew[k] == 11){
         LB_Pos = k;
      }
      if (keysNew[k] == 10){
         tilt_button = k;
      }
      if (keysNew[k] == 0){
        count++;
      }
     } 
   }
   else {
    for (int k=0; k < 32; k++){                   // routine should be run only once ever in lifespan of firmware
      int addr = k+1;
      if (addr == EEPROM.length()) {              // prevent overflow (should be impossible)
        addr = 0;
      }
      EEPROM.write(addr, keys[k]);
      delay(50);
      if (keys[k] == 11){
         LB_Pos = k;
      }
      if (keys[k] == 10){
         tilt_button = k;
      }
      if (keys[k] == 0){
        count++;
      }
     } 
    }
    
    EEPROM.write(0,1);                           // flag keys array as written in memory
    EEPROM.write(65,tilt_enable);                // TILT Enabled and written in memory
    EEPROM.write(66,LB_VAL);                       // LB_VAL written in memory
    EEPROM.write(67,zmaxSet);                    // write default value of 182                  
    EEPROM.write(68,zminSet);                    // write default value of 0
    EEPROM.write(69,CD_VAL);                     // write default Coin Door Button Value
    EEPROM.write(70,EX_VAL);                     // write default Exit Button Value 
    
     
  }  
  else {
    for (int k=0; k <32; k++){
      int addr = k+1;
      keys[k] = EEPROM.read(addr);
      if (keys[k] == 11){
         LB_Pos = k;
      }
      if (keys[k] == 10){
         tilt_button = k;
      }
      if (keys[k] == 0){
         count++;
      } 
    }  
  }
  lengthofkeys = 32 - count;                  // calculate number of keystrokes in array
  delay(1500);
}

