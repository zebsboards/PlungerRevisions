void SerCTRL(void) {
  
  
  receivedByte = Serial.read();
  
  switch (receivedByte) {
    case 'G':
      Gamepadflag = 1;             // set to gamepad mode
      receivedByte = "";
      break;
    case 'K':
      Gamepadflag = 0;             // set to keyboard mode
      receivedByte = "";
      break;
    case 'A':
      zblaunchflag = 0;            // set plunger mode to analog
      receivedByte = "";
      break;
    case 'D':
      zblaunchflag = 1;            // set plunger mode to digital ball launch
      receivedByte = "";
      break;
    case 'R':
      EEPROM.write(0,0);           // Reset Keys Flag
      delay(100);
      receivedByte = "";
      break;
    case 'C':
      ReceiveDataKeys();                   // call for the function that receives the CDVAL (coin door keystroke) data
      receivedByte = ""; 
      break;
    case 'T':
     tilt_enable = !tilt_enable;  // set TILT enable/disable flag to opposite state
     EEPROM.write(65,tilt_enable);
     delay(100);
     receivedByte = ""; 
     break;
    case 'L':                    // call function that assigns Launch Button Value
      ReceiveDataKeys();
      receivedByte = "";  
      break;
    case 'E':                    // call function that assigns Launch Button Value
      ReceiveDataKeys();
      receivedByte = "";  
      break;  
    case 'I':                      // call function to recieve 32 bytes of key data for keys[] array    
      ReceiveDataKeys();
      receivedByte = "";  
      break;
    case 'W':                      // call function to recieve 32 bytes of key data for keys[] array    
      delay(5000);
      for (int a = 0; a < 32; a++) {
        Serial.print("Gamepad Button ");
        Serial.print(a+1);
        Serial.print("   HEX   ");
        Serial.print(keys[a],HEX);
        Serial.print("   DEC   ");
        Serial.println(keys[a],DEC);
      }
      Serial.println("");
      Serial.print("Launch Ball Key   ");
      Serial.println(LB_VAL, HEX);
      Serial.print("Coin Door Key     ");
      Serial.println(CD_VAL, HEX);
      Serial.print("Exit Key          ");
      Serial.println(EX_VAL, HEX);
      Serial.print("TILT Button       ");
      Serial.println(tilt_button + 1);
      Serial.print("I2C Devices       ");
      Serial.println(i2c_present);
      receivedByte = "";  
      break; 
    case 'V':
      for (int a = 0; a <32; a++){
        for (int b = 0; b < 2; b++){
          Pincab_Controller_V2HB.setButton(a, HIGH);
          delay (250);
          Pincab_Controller_V2HB.setButton(a, LOW); 
          delay (250);   
        }
      }
      receivedByte = "";  
      break;
    default:
      if (Serial.available() > 0) {
         Serial.readBytesUntil('\n',buf,BUFFER_SIZE);
      }
      receivedByte = "";  
      break;
  }
}


