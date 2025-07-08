void SendConfigData(void)
{
  eeprom.init();                                                                                                   // set i2c frequency to 400000hz
  Serial.print("E");                                                                                               // Send current 8bit settings to software program
  for (int j = 100; j < 125; j++)
  {
    if (j < 124)
    {
      Serial.print(eeprom.read(j));Serial.print(",");
    }
    else
    {
      Serial.println(eeprom.read(j));
    }
    delay(1);
  }
  receivedByte = NULL;
  while(receivedByte != 'A')
  {
    receivedByte = Serial.read();
    delay(5);
  }
  Serial.print("F");                                                                                            // Send current KeyCommands to software program
  for (int a = 0; a < 31; a++)                                                // KeyCodes
  {
     Serial.print(eeprom.read(a+125));Serial.print(",");
     delay(1);
  }
  Serial.println(eeprom.read(156));
  receivedByte = NULL;
  while(receivedByte != 'A')
  {
    receivedByte = Serial.read();
    delay(5);
  }
  Serial.print("H"); 
  for (int a = 0; a < 31; a++)                                                // LabelCodes
  {
     Serial.print(eeprom.read(a+200));Serial.print(",");
     delay(1);
  }
  Serial.println(eeprom.read(231));
  receivedByte = NULL;
  while(receivedByte != 'A')
  {
    receivedByte = Serial.read();
    delay(5);
  }
  int count = 0;
  Serial.print("G");                                                                                           // Send current 16bit settings to software program
  for (int a = 0; a < 5; a++)
  {
     Serial.print((eeprom.read(157 + count)+(eeprom.read(158 + count)<<8)));Serial.print(",");
     count = count +2;
     delay(1);                                
  }
  Serial.println(eeprom.read(157 + count)+(eeprom.read(158 + count)<<8));
  eeprom.close();                                                                                              // Reset i2c frequency to 1700000hz
}

void ButtonTest(void)
{
  byte buttonArray[32];
  byte currentState = GamepadFlag;
  GamepadFlag = 1;
  receivedByte = NULL;
  
  int buttonOffset;
  int bankOffset;
  
  while(receivedByte != 'T')
  {
    receivedByte = Serial.read();
    delay(5);
    volatile uint16_t val = mcp1.read16();
    volatile uint16_t vals = mcp2.read16();
    for (int b = 0; b < 2; b++)
    {
      if (b == 0)
      {
        val = val;
        bankOffset = 0;
      }
      else
      {
        val = vals;
        bankOffset = 16;
      }
      for (int pin = 0; pin < 16; pin++)
      {
        volatile int state = bitRead(val, pin);
        if (pin < 8)
        {
          buttonOffset = 8;
        }
        else
        {
          buttonOffset = -8;
        }
        int8_t currentButton = (pin + buttonOffset) + bankOffset;
        buttonArray[currentButton] = !state;
      }
    }
    Serial.print("T");
    for (int button = 0; button < 31; button++)
    {
      Serial.print(buttonArray[button]); Serial.print(",");
    }
    Serial.println(buttonArray[31]);
    delay(50);
  }
  GamepadFlag = currentState;
}

