  /********************************** EEPROM WRITE/READ EXAMPLES **********************************
  
  clear();
  int16_t testval_1 = 12500;
  int16_t testval_2 = -1350;
  long    testval_3 = 128541;

  Serial.println(sizeof(testval_3));Serial.println("");

  eeprom.write(1, testval_1 & 0xff);
  eeprom.write(2, testval_1 >> 8);

  eeprom.write(3, testval_2 & 0xff);
  eeprom.write(4, testval_2 >> 8);

  eeprom.write(5, testval_3 & 0xff);
  eeprom.write(6, testval_3 >> 8);
  eeprom.write(7, testval_3 >> 16);
  eeprom.write(8, testval_3 >> 24);
  
  int16_t readval_1 = eeprom.read(1)+(eeprom.read(1+1)<<8);
  int16_t readval_2 = eeprom.read(3)+(eeprom.read(3+1)<<8);
  long    readval_3 = eeprom.read(5)+(eeprom.read(5+1)<<8)+(eeprom.read(5+2)<<16)+(eeprom.read(5+3)<<24);
  

  Serial.print("testval_1  "); Serial.print(readval_1); Serial.print("  testval_2  "); Serial.print(readval_2);Serial.print("  testval_3  "); Serial.println(readval_3);
  
  **********************************************************************************************************/

void BootConfig (void)
{
  eeprom.init();                                                                          //     set i2c frequency to 400000hz 
  GamepadFlag = eeprom.read(100);                                                         //  -  8bit EEPROM VALUE - Boot Mode
  delay(5);
  plungerIndicatorMode = eeprom.read(101);                                                //  -  8bit EEPROM VALUE - Boot Mode
  delay(5);
  plungerDeadzone = eeprom.read(102);                                                     //  -  8bit EEPROM VALUE
  delay(5);
  accelOrientation = eeprom.read(103);                                                    //  -  8bit EEPROM VALUE
  delay(5);
  XGain = eeprom.read(104);                                                               //  -  8bit EEPROM VALUE
  delay(5);
  YGain = eeprom.read(105);                                                               //  -  8bit EEPROM VALUE
  delay(5);
  tilt_enable = eeprom.read(106);                                                         //  -  8bit EEPROM VALUE - Boot Mode
  delay(5);
  LaunchBallButton = eeprom.read(107);                                                    //  -  8bit EEPROM VALUE
  delay(5);
  useHatSwitch = eeprom.read(108);                                                        //  -  8bit EEPROM VALUE - Boot Mode
  delay(5);
  CD_VAL = eeprom.read(109);                                                              //  -  8bit EEPROM VALUE
  delay(5);
  LBkey = eeprom.read(110);                                                               //  -  8bit EEPROM VALUE
  delay(5);
  plungerSenseType = eeprom.read(111);                                                    //  -  8bit EEPROM VALUE - Boot Mode
  delay(5); 
  tilt_button = eeprom.read(112);                                                         //  -  8bit EEPROM VALUE
  delay(5);
  accelScale = eeprom.read(113);                                                          //  -  8bit EEPROM VALUE
  delay(5);
  indBrightness = eeprom.read(114);                                                       //  -  8bit EEPROM VALUE
  delay(5);
  trackballFlag = eeprom.read(115);                                                       //  -  8bit EEPROM VALUE
  delay(5);
  CD_Pos = eeprom.read(116);                                                              // -   8bit EEPROM VALUE
  delay(5);
  for (int a = 0; a < 32; a++){KeyCommands[a] = eeprom.read(a + 125);delay(5);}           //  -  8bit EEPROM VALUE (x 32)
  for (int a = 0; a < 32; a++){LabelCodes[a] = eeprom.read(a + 200);delay(5);}            //  -  8bit EEPROM VALUE (x 32)
  plungerDrawLength = (eeprom.read(157)+(eeprom.read(158)<<8)) * -1;                      //  - 16bit EEPROM VALUE                  - converted to negative int after read
  delay(5);
  plungerPushLength = eeprom.read(159)+(eeprom.read(160)<<8);                             //  - 16bit EEPROM VALUE
  delay(5);
  Xdeadzone = eeprom.read(161)+(eeprom.read(162)<<8);                                     //  - 16bit EEPROM VALUE
  delay(5);
  Ydeadzone = eeprom.read(163)+(eeprom.read(164)<<8);                                     //  - 16bit EEPROM VALUE
  delay(5);
  tilt_VALX = eeprom.read(165)+(eeprom.read(166)<<8);                                      //  - 16bit EEPROM VALUE
  delay(5);
  tilt_VALY = eeprom.read(167)+(eeprom.read(168)<<8);                                      //  - 16bit EEPROM VALUE
  eeprom.close();                                                                          //    Reset i2c frequency to 1700000hz
  if (plungerIndicatorMode != 0)
  {
    plungerIndicator.setBrightness(indBrightness); 
    ClearIndicator();
  }                                                                       
}

void FactoryReset (void)
{
  eeprom.init();                                                                          //    set i2c frequency to 400000hz 
  byte eightbitsettings[25]=
  {
    1,      // GamepadFlag
    2,      // PlungerIndicatorMode
    50,     // Plunger Deadzone
    6,      // Board Orientation (Accelerometer)
    1,      // X Gain Factor
    1,      // Y Gain Factor
    1,      // Tilt Enable
    15,     // Launch Ball Button Location (actual use value)
    1,      // Use Hat Switch
    0x4D,    // CoinDoor Keystroke  (0xD5)
    0x28,    // LB Keystroke        (0xB0)
    1,      // Plunger Sensor Type (IR(1)/POT(2)/Disabled(0)
    17,     // Tilt Button Location
    4,      // accelScale          (2/4/8/16)
    10,     // indicator brightness
    0,      // trackball enable
    6,     // Coin Door Button Assignment (actual use value)
  };
  byte keys[32] = {                                                                     //    default original ZB plunger keystroke assignment and order
    0xE1,   // LShift  Left Flipper               Button 1
    0xE0,   // LCTRL   Left MSave                 Button 2  
    0x1E,   // 1       Start                      Button 3
    0x14,   // q       Exit                       Button 4
    0x1F,   // 2       Extra Buy-in               Button 5
    0x22,   // 5       Coin                       Button 6
    0x4D,   // END     Coin Door Open/Close       Button 7  
    0x2E,   // =       Volume DOWN                Button 8
    0x2D,   // -       Volume UP                  Button 9
    0x25,   // 8       SMenu Down                 Button 10
    0x26,   // 9       SMenu UP                   Button 11
    0x27,   // 0       SMenu Enter                Button 12
    0x24,   // 7       SMenu Cancel               Button 13
    0xE5,   // RShift  Right Flipper              Button 14
    0xE4,   // RCTRL   Right MSave                Button 15
    0x28,   // Enter   Launch/Fire                Button 16
    0x17,   // Tilt Button Assignment   t         Button 17
    0x00,   // null 'n'                           Button 18
    0x00,   // null 'n'                           Button 19
    0x00,   // null 'n'                           Button 20
    0x00,   // null 'n'                           Button 21
    0x00,   // null 'n'                           Button 22
    0x00,   // null 'n'                           Button 23
    0x00,   // null 'n'                           Button 24
    0x52,   // null 'n'                           Button 25
    0x4F,   // null 'n'                           Button 26
    0x51,   // null 'n'                           Button 27
    0x50,   // null 'n'                           Button 28
    0x1A,   // null 'n'                           Button 29
    0x07,   // null 'n'                           Button 30
    0x16,   // null 'n'                           Button 31
    0x04    // null 'n'                           Button 32
  };
  int16_t sixteenbitsettings[] = {
    320,    // plungerDrawLength
    790,    // plungerPushLength
    2000,   // X axis Deadzone
    2000,   // Y axis Deadzone
    12500,   // X Tilt Threshold
    12500   // Y Tilt Threshold
  };
  byte LabelCodes[] = {
    0x01,0x03,0x05,0x06,0x08,0x09,0x0C,0x0F,0x0E,0x11,0x10,0x12,0x13,0x02,0x04,0x0A,0x17,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F
  };
   
  for (int8_t a = 0; a < 25; a++)    //**************************************  8 bit write ***************
  {
   eeprom.write(a + 100, eightbitsettings[a]); 
   delay(20);
  }
  for (int8_t a = 0; a < 32; a++)   //**************************************  8 bit write ***************
  {
   eeprom.write(a + 125, keys[a]);
   delay(10);
  }
  for (int8_t a = 0; a < 32; a++)   //**************************************  8 bit write ****************
  {
   eeprom.write(a + 200, LabelCodes[a]);
   delay(10);  
  }
  int count = 0;                    //*************************************** 16 bit write ***************
  for (int a = 0; a < 6; a++)
  {
    eeprom.write(157 + count, sixteenbitsettings[a] & 0xff);
    eeprom.write(158 + count, sixteenbitsettings[a] >> 8);
    count = count +2;
    delay(10);                                
  }
  /*for (int8_t a = 0; a < 25; a++)  //**************************************  8 bit read ***************
  {
   Serial.println(eeprom.read(a + 100)); 
   delay(20);
  }
  for (int8_t a = 0; a < 32; a++)   //**************************************  8 bit read ***************
  {
   Serial.println(eeprom.read(a + 125)); 
   delay(20);
  }
  count = 0;                        //*************************************** 16 bit read **************                          
  int16_t readVal;
  for (int a = 0; a < 6; a++)
  {
    readVal = eeprom.read(157 + count)+(eeprom.read(158 + count)<<8);
    Serial.println(readVal);
    count = count +2;
    delay(10);                                
  }
  Serial.println("");Serial.println("");Serial.println("COMPLETE"); Serial.println("");*/
  BootConfig();                                                                  // Read stored values and assign to Control Variables
  eeprom.close();                                                                // Reset i2c frequency to 1700000hz
}

void DisplayMem (void)
{
  eeprom.init();                                                                          //    set i2c frequency to 400000hz 
  int counter = 0;
  Serial.print("8bit Config Values     ");
  for (int j = 100; j < 157; j++)
  {
    if(j == 125)
    {
      Serial.println("");Serial.println("");
      Serial.print("8bit Key Commands   ");
    }
    Serial.print(eeprom.read(j));Serial.print(",");
  }
  Serial.println("");Serial.println("");
  Serial.print("8bit Label Codes       ");
  for (int j = 200; j < 232; j++)
  {
    Serial.print(eeprom.read(j));Serial.print(",");
  }
  Serial.println("");Serial.println("");
  Serial.print("16bit Config Values   ");
  for (int j = 0; j < 6; j++)
  {  
     Serial.print(eeprom.read(157 + counter)+(eeprom.read(158 + counter)<<8));Serial.print(",");
     counter = counter + 2;
  }
  Serial.println("");Serial.println("");
  Serial.print("plungerDrawLength  ");Serial.print(plungerDrawLength);Serial.print("   plungerPushLength  ");Serial.print(plungerPushLength);Serial.print("   plungerAtRest  ");Serial.print(plungerAtRest);Serial.print("   plungerDeadzone  ");Serial.println(plungerDeadzone);
  Serial.println("");
  Serial.print("accelOrientation  ");Serial.print(accelOrientation);Serial.print("   Sensitivity  ");Serial.print(accelScale);Serial.print("g");Serial.print("   X_offset  ");Serial.print(x_offset);Serial.print("   Y_offset  ");Serial.print(y_offset);Serial.print("   Z_offset  ");Serial.println(z_offset);
  Serial.println("");
  Serial.print("X_Deadzone  ");Serial.print(Xdeadzone);Serial.print("   X_Gain  ");Serial.print(XGain);Serial.print("   Y_Deadzone  ");Serial.print(Ydeadzone);Serial.print("   Y_Gain  ");Serial.println(YGain);
  Serial.println("");
  Serial.print("GamepadFlag  ");Serial.print(GamepadFlag);Serial.print("   plungerIndicatorMode  ");Serial.print(plungerIndicatorMode);Serial.print("   Indicator Brightness  ");Serial.print(indBrightness);Serial.print("   TiltEnable  ");Serial.print(tilt_enable);Serial.print("   UseHatSwitch  ");Serial.println(useHatSwitch);
  Serial.println("");
  Serial.println("REPORT END");
  eeprom.close();                                                                          //    reset i2c frequency to 1700000hz 
}
  



