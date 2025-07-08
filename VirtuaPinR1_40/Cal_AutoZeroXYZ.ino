void autozeroing(void) 
{ 
  /************************************************* ACCELEROMETER *****************************************************************************************************/
  if (accelEnabled != 0)
  {
    int16_t x_centered;
    int16_t y_centered;
    int16_t z_centered;
    for (int8_t a=1; a < 31; a++)
    {
       x_centered = x_centered + (accel.getRawX()*2);                                                                                         // accumulate the totals
       delay(10);
       y_centered = y_centered + (accel.getRawY()*2);
       delay(10);
       z_centered = z_centered + (accel.getRawZ()*2);
       delay(10);
    } 
    x_offset = (x_centered / 30) * -1;                                                                                                      // get the average reading
    y_offset = (y_centered / 30) * -1;                                                                                                      // and create an offset from it
    z_offset = (z_centered / 30) * -1;
  }
  
  /************************************************ PLUNGER SENSOR *****************************************************************************************************/
  switch (plungerSenseType)
  {
    int pl_centered;
    case 1:
        plungerAtRest = plungerSensor.getProximity();
        delay(10);
        plungerAtRest = plungerSensor.getProximity();                                                                                  // Initial starting point of plunger on IR SENSOR
        break;
    case 2:
        for (int8_t a = 0; a<31; a++)
        {
          pl_centered = pl_centered + analogRead(plungerPotPin);                                                                            // Initial starting point of plunger on ANALOG POT SENSOR
          delay(10); 
        }
        plungerAtRest = (pl_centered / 29) * -1;                                                                                            // determine the average reading
        break;
    default:
        break;  
  }
}

void Calibration (void)
{
  if (plungerSenseType != 0)
  {
    eeprom.init(); 
    int drawComplete = 0;
    plungerDrawLength = 0;        // number of counts read from resting point to maximum pull
    plungerPushLength = 0;        // number of counts read from resting point to maximum push
    elapsedMillis calTimer;
    if (plungerSenseType == 1)
    {
      plungerAtRest = plungerSensor.getProximity();                           // starting position PULL test
    }
      else
    {
      plungerAtRest = analogRead(plungerPotPin);
    }
    Serial.println("A");
    while(receivedByte != 'A')
    {
      receivedByte = Serial.read();
      delay(20);
    }
    receivedByte = NULL;
    calTimer = 0;
    while (drawComplete < 1)
    {
      if (plungerSenseType == 1)
      {
         plungerRead = plungerSensor.getProximity();                          // current raw reading
      }
      else
      {
        plungerRead = analogRead(plungerPotPin) + (plungerAtRest * -1);
      }
      if (plungerRead == plungerDrawLength && plungerRead < plungerAtRest-100)    // if plunger is stopped and is being pulled
      {
          drawComplete = 1;
          calTimer = 8001;
      }
      else if (plungerRead != plungerDrawLength)                              // if plunger is moving
      {
         plungerDrawLength = plungerRead;
         calTimer = 0;
      }
      if (calTimer > 8000)
      {
        drawComplete = 1;
      }
      delay(50);
    }
    plungerDrawLength = (plungerAtRest - plungerDrawLength + 3);      // subtract 3 counts as a buffer on the maximum number of counts moved on the pull
    Serial.println(plungerDrawLength);
    eeprom.write(157, plungerDrawLength & 0xff);
    eeprom.write(158, plungerDrawLength >> 8);
    delay(50);
    calTimer = 0;
    /***************************************************** Release and Settle *************************************************************/
    Serial.println("B");                              // start 3500ms delay on other end to allow plunger to settle
    while(receivedByte != 'A')                        // watch for indicator to proceed after delay completes 
    {
      receivedByte = Serial.read();
      delay(20);
    }
    receivedByte = NULL;
    drawComplete = 0;
    if (plungerSenseType == 1)
    {
      plungerAtRest = plungerSensor.getProximity();                                       // current raw reading ... PULL TEST starting position 
    }
      else
    {
      plungerAtRest = analogRead(plungerPotPin);
    }
    calTimer = 0;
    /**************************************************  Send Push Data *****************************************************************/
    Serial.println("C");
    while(receivedByte != 'A')
    {
      receivedByte = Serial.read();
      delay(20);
    }
    receivedByte = NULL;
    calTimer = 0;
    while (drawComplete < 1)
    {
      if (plungerSenseType == 1)
      {
         plungerRead = plungerSensor.getProximity();                                        // current raw reading - PUSH
      }
      else
      {
        plungerRead = analogRead(plungerPotPin) + (plungerAtRest * -1);
      }
      if (plungerRead == plungerPushLength && plungerRead > plungerAtRest+1000)                  // maxxed out on push and above initial reading at rest
      { 
        drawComplete = 1;
        calTimer = 8001;
      }
      else if (plungerRead != plungerPushLength)                                            // still pushing
      {
         plungerPushLength = plungerRead;                                                   // raw value of current location
         calTimer = 0;
      }
      if (calTimer > 8000)                                                                  // failsafe timer
      {
         drawComplete = 1;
      }
      delay(50);
    }
    /**************************************************  Send Release *****************************************************************/
    plungerPushLength = plungerRead - plungerAtRest -3;
    Serial.println(plungerPushLength);
    eeprom.write(159, plungerPushLength & 0xff);
    eeprom.write(160, plungerPushLength >> 8);
    delay(50);
    eeprom.close(); 
    Serial.println("D");
    calTimer = 0;
    /**************************************************  wait for ack *****************************************************************/
    while(receivedByte != 'A')
    {
      receivedByte = Serial.read();
      delay(20);
    }
    receivedByte = NULL;
    /**************************************************  Send complete *****************************************************************/
    Serial.println("F");
    delay(100);
    plungerAtRest = plungerSensor.getProximity();
  }
} 
