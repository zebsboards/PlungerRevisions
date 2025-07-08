void SerCTRL (void)
{
  String Revision = "1.40";
    
  receivedByte = Serial.read();
  
  if (receivedByte == 'O')                          // if output data is received don't pause routines
  {
    pauseOnSerial = 0;
  }
  else                                              // otherwise pause plunger routines to avoid memory collisions when writing to eeprom
  {
    pauseOnSerial = 1;
  }
  
  switch (receivedByte) 
  {
    case 'A':
      zblaunchflag = 0;                             // set plunger mode to analog - OTG by bat file
      receivedByte = NULL;
      pauseOnSerial = 0;
      break;
    case 'B':
      ReceiveDataConfigSettings();                  // call to function that receives Configuration Settings (in array) from Software Program and writes to eeprom
      receivedByte = NULL;
      BootConfig();
      Serial.println("A");                          // Acknowledge Config Complete 
      pauseOnSerial = 0;
      break;
    case 'C':                                       // call function to receive 32 bytes of key data for KeyCommands[] array    
      ReceiveDataKeys();
      receivedByte = NULL;
      BootConfig();
      Serial.println("A");                          // Acknowledge Config Complete 
      pauseOnSerial = 0;
      break;      
    case 'D':
      zblaunchflag = 1;                             // set plunger mode to digital ball launch - OTG by bat file
      receivedByte = NULL;
      pauseOnSerial = 0;
      break;
    case 'E':                                       // Send current Configuration Settings to software program
      SendConfigData();
      receivedByte = NULL;
      pauseOnSerial = 0;
      break;
    case 'F':                                       // factory reset
      FactoryReset();
      pauseOnSerial = 0;
      break;     
    case 'G':
      GamepadFlag = 1;                              // set to gamepad mode - OTG by bat file
      receivedByte = NULL;
      pauseOnSerial = 0;
      break;
    case 'H':                                       // Toggle hatswitch mode - OTG by bat file
      useHatSwitch = !useHatSwitch;
      receivedByte = NULL;
      pauseOnSerial = 0;
      break;  
    case 'I':                                       // call function to recieve 5 x 16bit settings for accelerometer deazones, tilt threshold, plunger push/pull lengths
      ReceiveLargeDataVals();
      receivedByte = NULL;
      BootConfig();                                 // Read stored values and assign to Control Variables
      Serial.println("A");                          // Acknowledge Config Complete 
      pauseOnSerial = 0;
      break;  
    case 'J':
      DisplayMem();                                 // Read Memory/Config settings and display contents
      receivedByte = NULL;
      pauseOnSerial = 0;
      break;
    case 'K':
      GamepadFlag = 0;                              // set to keyboard mode - OTG by bat file
      receivedByte = NULL;
      pauseOnSerial = 0;
      break;
    case 'L':                                       // call function to receive 32 bytes of key label data for LabelCodes[] array    
      ReceiveDataLabels();
      receivedByte = NULL;
      BootConfig();
      Serial.println("A");                          // Acknowledge Config Complete 
      pauseOnSerial = 0;
      break;       
    case 'O':                                       // Reserved for eventual OUTPUT usage
      receivedByte = NULL;
      pauseOnSerial = 0;
      break;
    case 'P':                                       // Calibration - PLUNGER
      Calibration(); 
      receivedByte = NULL;
      BootConfig();                                 // Read stored values and assign to Control Variables
      pauseOnSerial = 0;
      break;         
    case 'R':
      delay(3500);
      ESP.restart();                                // Reboot Controller
    case 'T':                                       // Test Buttons
      ButtonTest();
      receivedByte = NULL;
      pauseOnSerial = 0;
      break;  
    case 'V':                      
      Serial.println("VirtuaPin,20A0,41B9," + Revision);  // Software Requests Firmware Version
      receivedByte = NULL;
      pauseOnSerial = 0;
      break;        
    default:
      if (Serial.available() > 0) 
      {
        Serial.readBytesUntil('\n',buf,BUFFER_SIZE);
      }
      receivedByte = NULL;
      pauseOnSerial = 0;
      break;
  }
}

