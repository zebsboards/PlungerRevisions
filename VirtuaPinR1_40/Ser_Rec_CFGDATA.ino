void ReceiveDataConfigSettings(void)
{
  eeprom.init();                                                                         //    set i2c frequency to 400000hz
  byte numBytesReceived;
  byte ConfigArray[25];
  while (!Serial.available())
    delay(500);
  if (Serial.available() > 0)
  {
    {
      byte n = Serial.available(); //to check that a character has come from Serial Monitor
      if (n != 0)
      {
        numBytesReceived = Serial.readBytesUntil('\n', buf, BUFFER_SIZE);
        buf[numBytesReceived] = '\0';
      }
    }
    char*next;               //pointer to substring (token)
    next = strtok(buf, ","); //splitting string by "," and get 1st substring token
    if (receivedByte == 'B')
    {
      ConfigArray[0] = atol(next);
      for (int a = 1; a < 25; a++)
      {
        next = strtok(NULL, ","); // print next string  and remove previous string
        ConfigArray[a] = atol(next);
      }
      for (int a = 0; a < 25; a++)
      {
        eeprom.write(a + 100, ConfigArray[a]); // write to EEPROM
        delay(10);
      }
    }
  }
  eeprom.close();                                                                         //    Reset i2c frequency to 1700000hz
}

void ReceiveDataKeys(void)
{
  byte numBytesReceived;
  int KeysTmp[KEY_SIZE];
  eeprom.init();                                                                          //    set i2c frequency to 400000hz
  if (Serial.available() > 0) {
    {
      byte n = Serial.available(); //to check that a character has come from Serial Monitor
      if (n != 0)
      {
        numBytesReceived = Serial.readBytesUntil('\n', buf, BUFFER_SIZE);
        buf[numBytesReceived] = '\0';
      }
    }
    char*next;               //pointer to substring (token)
    next = strtok(buf, ","); //splitting string by "," and get 1st substring token
    if (receivedByte == 'C') {
      KeysTmp[0] = atol(next);
      for (int a = 1; a < KEY_SIZE; a++) {
        next = strtok(NULL, ","); // print next string  and remove previous string
        KeysTmp[a] = atol(next);
      }
      for (int a = 0; a < KEY_SIZE; a++) {
        KeyCommands[a] = KeysTmp[a];
        eeprom.write(a + 125, KeyCommands[a]); // write to EEPROM
        delay(10);
      }
    }
  }
  eeprom.close();                                                                        //    Reset i2c frequency to 1700000hz
}

void ReceiveDataLabels()
{
  byte numBytesReceived;
  int KeysTmp[KEY_SIZE];
  eeprom.init();                                                                          //    set i2c frequency to 400000hz  
  if (Serial.available() > 0) {
    {
      byte n = Serial.available(); //to check that a character has come from Serial Monitor
      if (n != 0)
      {
        numBytesReceived = Serial.readBytesUntil('\n', buf, BUFFER_SIZE);
        buf[numBytesReceived] = '\0';
      }
    }
    char*next;               //pointer to substring (token)
    next = strtok(buf, ","); //splitting string by "," and get 1st substring token
    if (receivedByte == 'L') {
      KeysTmp[0] = atol(next);
      for (int a = 1; a < KEY_SIZE; a++) {
        next = strtok(NULL, ","); // print next string  and remove previous string
        KeysTmp[a] = atol(next);
      }
      for (int a = 0; a < KEY_SIZE; a++) {
        LabelCodes[a] = KeysTmp[a];
        eeprom.write(a + 200, LabelCodes[a]); // write to EEPROM
        delay(10);
      }
    }
  }
  eeprom.close();                                                                        //    Reset i2c frequency to 1700000hz
}

void ReceiveLargeDataVals (void)
{
  eeprom.init();                                                                         //    set i2c frequency to 400000hz
  byte numBytesReceived;
  int16_t ConfigArray[6];
  int count = 0;
  while (!Serial.available())
    delay(500);
  if (Serial.available() > 0) {
    {
      byte n = Serial.available(); //to check that a character has come from Serial Monitor
      if (n != 0)
      {
        numBytesReceived = Serial.readBytesUntil('\n', buf, BUFFER_SIZE);
        buf[numBytesReceived] = '\0';
      }
    }
    char*next;               //pointer to substring (token)
    next = strtok(buf, ","); //splitting string by "," and get 1st substring token
    if (receivedByte == 'I')
    {
      ConfigArray[0] = atol(next);
      for (int a = 1; a < 6; a++)
      {
        next = strtok(NULL, ","); // print next string  and remove previous string
        ConfigArray[a] = atol(next);
      }
      for (int a = 0; a < 6; a++)
      {
        eeprom.write(157 + count, ConfigArray[a] & 0xff);
        eeprom.write(158 + count, ConfigArray[a] >> 8);
        count = count + 2;
        delay(10);  
      }
    }
  }
  eeprom.close();                                                                         //   Reset i2c frequency to 1700000hz
}

/*
 //************************DEBUG Received********************************************
    Serial.print("Received(HEX).....   ");
    for (int a = 0; a < numBytesReceived; a++)
    {
      if (buf[a] == ',')
      {
        Serial.print(",");
      }
      else
      {
        Serial.print(buf[a], HEX);
      }
    }
    Serial.println("");
    //************************END DEBUG*********************************************
    

    //************************DEBUG********************************************
      Serial.print("Stored  (HEX).....   ");
      for (int a = 0; a < 5; a++)
      {
        Serial.print(ConfigArray[a], HEX);
        Serial.print(",");
        delay(5);
      }
      Serial.println("");
      Serial.print("Stored  (DEC).....   ");
      for (int a = 0; a < 5; a++)
      {
        Serial.print(ConfigArray[a], DEC);
        Serial.print(",");
      }
      Serial.println("");
      Serial.print("SizeOf   INT .....   ");
      for (int a = 0; a < 5; a++)
      {
        Serial.print(sizeof(ConfigArray[a]));
        Serial.print(",");
      }
      Serial.println("");
      //********************END DEBUG*************************************************
 */
