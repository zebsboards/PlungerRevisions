void SerCTRL (void)
{
  String Revision = "2.0.0.0";
  receivedByte = Serial.read();
  switch (receivedByte)
  {
    case 'A':                                           // Plunger in analog mode - acts as a normal plunger
      analogMode = true;
      break;
    case 'B':                                           // Send Encoder Positional Loop for rack to pinion alignment                                                - Cal_Punger.ino file
      AlignRack();
      break;  
    case 'D':                                           // Plunger is in digital mode - pressing plunger acts as pressing a button switch
      analogMode = false;
      break;
    case 'E':                                           // EEPROM settings command received
      EEPROMcommand();
      break;
    case 'G':                                           // Button input is in Gamepad mode - gamepad button presses only - when flag is set to '1'
      GamepadFlag = 1;                                
      break;
    case 'K':                                           // Button input is in Keyboard mode - keypresses and gamepad button presses - when flag is set to '0'
      GamepadFlag = 0;
      break;      
    case 'P':                                           // Call to run the plunger calibration routine - command sent by utility                                     - Cal_Punger.ino file
      Calibration();
      break;
    case 'R':                                           // Reboot Controller
      delay(3500);
      ESP.restart();                                    
    case 'v':                                           // Response to call for info from utility on serial connection event  
      Serial.println("ZBV7,"+ Revision);
      break;  
  }
}

void EEPROMcommand (void) {
  receivedByte = Serial.read();
  if (receivedByte == 'E') {                          // Call to Erase eeprom - EE
    ClearFlashMemory();
  }                                           
  else if (receivedByte == 'R') {                     // Call to RECEIVE Option settings from configuration utility  - ER
    ReceiveDataConfigSettings();
  }  
  else if (receivedByte == 'K') {                     // Call to RECEIVE Key assignments from configuration utility  - EK
    ReceiveDataKeys();
  }
  else if (receivedByte == 'L') {                     // Call to RECEIVE Label Code Assignments from configuration utility - EL
    ReceiveDataLabels();
  }
  else if (receivedByte == 'P'){                      // Call to RECEIVE Plunger Count settings from configuration utility - EP 
    ReceiveLargeDataVals();
  }
  else if (receivedByte == 'S') {                     // Call to SEND stored settings to configuration utility  - ES
    SendDataConfigSettings();
  }
  else if (receivedByte == 'F') {                     // Call to Restore Factory Settings - EF
    FactoryReset();
  }
}  