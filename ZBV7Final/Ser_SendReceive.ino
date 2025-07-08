void ClearFlashMemory (void) {
  Serial.println("M,ERASING FLASH MEMORY...Please Stand By");
  eeprom.init();
  for (int a = 0; a < 256; a++) {
    eeprom.write(a,0x00);                           // write OXFF into all blocks
    delay(2);
  }
  eeprom.write(0,0);                                // write eeprom empty flag into 1st block
  delay(2);
  Serial.println("A,D");
  delay(100);
  Serial.println("M,FLASH MEMORY CLEARED");
  eeprom.close();
}

void SendDataConfigSettings(void) {
  eeprom.init();                                                                                                   // set i2c frequency to 400000hz
  if (eeprom.read(0) != 0){
    Serial.print("S,");                                                                                               // Send current 8bit settings to software program
    for (int j = 100; j < 119; j++) {
      if (j < 118) {
        Serial.print(eeprom.read(j));Serial.print(",");
      }
      else {
        Serial.println(eeprom.read(j));
      }
      delay(1);
    }
    receivedByte = NULL;
    while(receivedByte != 'A') {
      receivedByte = Serial.read();
      delay(5);
    }
    Serial.print("K,");                                                                                            // Send current KeyCommands to software program
    for (int a = 0; a < 31; a++) {                                               // KeyCodes
      Serial.print(eeprom.read(a+125));Serial.print(",");
      delay(1);
    }
    Serial.println(eeprom.read(156));
    receivedByte = NULL;
    while(receivedByte != 'A') {
      receivedByte = Serial.read();
      delay(5);
    }
    Serial.print("L,"); 
    for (int a = 0; a < 31; a++) {                                               // LabelCodes
      Serial.print(eeprom.read(a+200));Serial.print(",");
      delay(1);
    }
    Serial.println(eeprom.read(231));
    receivedByte = NULL;
    while(receivedByte != 'A') {
      receivedByte = Serial.read();
      delay(5);
    }
    Serial.print("C,");                                                                                           // Send current 16bit settings to software program
    Serial.print((eeprom.read(157)+(eeprom.read(158)<<8)));Serial.print(",");
    delay(1);                                
    Serial.println(eeprom.read(159)+(eeprom.read(160)<<8));
    //delay(100);
    //Serial.println("M,Settings Received Successfully");
  }
  else {
    Serial.println("M,!!! Nothing to Send. Flash Memory Empty !!!");
  }
  eeprom.close();                                                                                              // Reset i2c frequency to 1700000hz
}

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
    if (receivedByte == 'R')
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
      Serial.println("A,R");
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
    if (receivedByte == 'K') {
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
      Serial.println("A,K");
    }
  }
  eeprom.close();                                                                        //    Reset i2c frequency to 1700000hz
}

void ReceiveDataLabels(void)
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
      Serial.println("A,L");
    }
  }
  eeprom.close();                                                                        //    Reset i2c frequency to 1700000hz
}

void ReceiveLargeDataVals (void)
{
  eeprom.init();                                                                         //    set i2c frequency to 400000hz
  byte numBytesReceived;
  int16_t ConfigArray[2];
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
    if (receivedByte == 'P')
    {
      ConfigArray[0] = atol(next);
      for (int a = 1; a < 2; a++)
      {
        next = strtok(NULL, ","); // print next string  and remove previous string
        ConfigArray[a] = atol(next);
      }
      for (int a = 0; a < 2; a++)
      {
        eeprom.write(157 + count, ConfigArray[a] & 0xff);
        eeprom.write(158 + count, ConfigArray[a] >> 8);
        count = count + 2;
        delay(10);  
      }
      eeprom.write(0,1);                                                                      //  set flag for eeprom being written
      initializeVariables();                                                                  //  reassign variables to newly uploaded values
      Serial.println("A,P");
    }
  }
  eeprom.close();                                                                         //   Reset i2c frequency to 1700000hz
}
