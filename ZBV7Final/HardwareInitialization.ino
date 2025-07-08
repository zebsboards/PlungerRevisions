/*****************************************************************************************************************************************************************************************
*   Called during setup, this sketch initializes the hardware based upon settings set through the configuration utility ((use nudging, use plunger, gamepad/keyboard startup mode, etc)) *
*                                                                                                                                                                                        *
*****************************************************************************************************************************************************************************************/
void initializeVariables(void){
  GamepadFlag = eeprom.read(100);
  delay(2);
  CoinDoorInUse = eeprom.read(101);
  delay(2);
  CoinDoorButton = eeprom.read(102);
  delay(2);
  LaunchBallButton = eeprom.read(103);
  delay(2);
  LBKey = eeprom.read(104);
  delay(2);
  useHatSwitch = eeprom.read(105);
  delay(2);
  UseNudging = eeprom.read(106);
  delay(2);
  Sensitivity = eeprom.read(107);
  delay(2);
  NuOrientation = eeprom.read(108);
  delay(2);
  Debounce = eeprom.read(109);
  delay(2);
  //n/a
 // delay(2);                                             
  NuXGain = eeprom.read(111);                           
  delay(2);
  NuYGain = eeprom.read(112);
  delay(2);
  XDZ = eeprom.read(113);
  delay(2);
  YDZ = eeprom.read(114);
  delay(2);
  UseTilt = eeprom.read(115);
  delay(2);
  TiltButton = eeprom.read(116);
  delay(2);
  Xtilt = eeprom.read(117);
  delay(2);
  Ytilt = eeprom.read(118);
  delay(2);
  for (int a = 0; a < 32; a++){
    KeyCommands[a] = eeprom.read(a + 125);
    delay(2);
  }
  delay(2);
  pushMax = eeprom.read(157)+(eeprom.read(158)<<8);
  delay(2);
  pullMax = eeprom.read(159)+(eeprom.read(160)<<8);
  delay(2);
  for (int a = 0; a < 32; a++){
    LabelCodes[a] = eeprom.read(a + 200);
    delay(2);
  }
  NuXdeadzone = XDZ * 256;                                    // adjust Deadzone value by offset
  NuYdeadzone = YDZ * 256;
  TiltXval = Xtilt * 256;                                     // bring stored value to 16bit value
  TiltYval = Ytilt * 256;
  BounceTime = Debounce * 10;                                 // adjust stored value to 0-2550ms range
}

void initializeButtons (void) {
  mcp1.begin(true);                                          //  switch to mcp23017 library usage
  mcp2.begin(true);
  mcp1.pinMode16(0xFFFF);                                    //  set read mode to 16bit (button routine will read the bit mask)
  mcp2.pinMode16(0xFFFF);
  Keyboard.begin();                                          //  start the keyboard device
}

void initializeNudge (void)
{
  if (UseNudging != 0) {
    Nudge.begin();                                            // start accelerometer at defined address (0x18)
    switch(Sensitivity){                                      // set accelerometer tracking rate based on setting
      case 2:
        Nudge.setScale(LIS2DH12_2g);
      break;
      case 8:
        Nudge.setScale(LIS2DH12_8g);
      break;
      case 16:
        Nudge.setScale(LIS2DH12_16g);
      break;
      default:
        Nudge.setScale(LIS2DH12_4g);                              
      break;
    }
    Nudge.setMode(LIS2DH12_HR_12bit);                         // set accelerometer resolution
    Nudge.setDataRate(LIS2DH12_ODR_5kHz376_LP_1kHz344_NM_HP);
    NuXatRest = Nudge.getRawX();                                                                                    
    delay(10);
    NuYatRest = Nudge.getRawY();
    delay(10);
    NuZatRest = Nudge.getRawZ();
    delay(10);
  }
}

void initializePlunger(void){
  pinMode(plungerPotPin, INPUT);
  analogReadResolution(10);                    // set the analog resolution (10bit)
  getPosition();
  analogAtRest = position;                     // set the stationary (at rest) reading
}

void initializeController (void) {
  ZBV7.begin();
  ZBV7.setXAxisRange(-32767, 32767);
  ZBV7.setYAxisRange(-32767, 32767);
  ZBV7.setZAxisRange(-127, 127);
}