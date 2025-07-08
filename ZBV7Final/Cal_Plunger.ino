void Calibration(void) {
  /********************************************************************************************************************************************
  *
  *   Plunger calibration determines the readings that can be applied at either end of the range (PULL, PUSH) and stores them in memory on the eeprom chip.
  *   Plunger operation will use these values to map the range of physical movement to the gamepad scale of resting point (-127, 0, 127)
  *
  ********************************************************************************************************************************************/
  int drawComplete = 0;                                                                                               // changing to 1 breaks out of the loop
  int moveMax;                                                                                                        // maximum reading on push or pull
  elapsedMillis calTimer = 0;                                                                                         // failsafe timeout timer in case the routine locks in a loop
  eeprom.init();
  analogAtRest = analogRead(3);                                                                                       // get starting point reading (should be 1023)
  Serial.print("Q,"); Serial.println(analogAtRest);
  while(receivedByte != 'A')  {
    receivedByte = Serial.read();
    delay(10);
  }
  Serial.println("M,PULL & RELEASE");
  receivedByte = NULL;
  while (receivedByte != 'A') {
    receivedByte = Serial.read();
    delay(10);
  }
  while (drawComplete < 1) {
    position = analogRead(3);
    if (position < 1023 && position > 890) {                                                                          // Plunger is being pushed instead of pulled
      Serial.println("M,Do Not Push the Plunger, PULL it");
      calTimer = 0;
    } 
    else if (position > 1022) { position = 0;}                                                                        // Plunger is at resting point so zero out the reading 
    else if (moveMax <= position) { moveMax = position; }                                                             // Plunger being pulled back store the reading
    else if (moveMax > position) { Serial.print("P,"); Serial.println(position); drawComplete = 1; calTimer = 0;}     // Plunger starts moving back to resting point so break the loop
    else if (calTimer > 6000) { Serial.println("M,Failsafe Timer Timed Out"); drawComplete = 1;}                      // If the timer runs out then break as a failsafe
    delay(50);
  }  
  pullMax = moveMax;
  
  /***************************************************** Release and Settle *************************************************************/
  eeprom.write(157, pullMax & 0xff);
  eeprom.write(158, pullMax >> 8);
  delay(1000);                                                                                                        // start 1000ms delay on other end to allow plunger to settle
  receivedByte = NULL;
  /**************************************************  Send Push Data *****************************************************************/
  drawComplete = 0;
  moveMax = analogRead(3);                                                                                            // movement will be in the reverse so start at high number and move low
  Serial.println("M,PUSH ALL THE WAY IN & RELEASE");
  while (receivedByte != 'A')                                                                                         // watch for indicator to proceed after delay completes
  {
    receivedByte = Serial.read();
    delay(10);
  }
  while (drawComplete < 1) {
    position = analogRead(3);                                                                                         // get Current position of plunger
    if (position > 0 && position < 700) {                                                                             // Plunger being pulled instead of pushed
      Serial.println("M,Do Not Pull the Plunger, PUSH it");
      calTimer = 0;
    } 
    else if (moveMax > position) { moveMax = position; }                                                              // Plunger being pushed in store the reading
    else if (moveMax < position) { Serial.print("R,"); Serial.println(moveMax); drawComplete = 1; calTimer = 0;}      // Plunger starts moving back to resting point so break the loop
    if (calTimer > 6000) { Serial.println("M,Failsafe Timer Timed Out"); drawComplete = 1;}                           // If the timer runs out then break as a failsafe
    delay(50);
  }  
  /**************************************************  Send Release *****************************************************************/
  pushMax = moveMax ;
  eeprom.write(159, pushMax & 0xff);
  eeprom.write(160, pushMax >> 8);
  delay(5);
  eeprom.close();
  
  /**************************************************  Send complete *****************************************************************/
  Serial.println("M,Calibration Complete");
  delay(100);
  receivedByte = NULL;
}

void AlignRack (void)
{
  bool Done = false;
  delay(100);
  while (Done != true) {
    if (Serial.available()) { receivedByte = Serial.read(); if (receivedByte == 'B') { Done = true;}}
    Serial.print("B,"); Serial.println(analogRead(3));
    delay(50);
  }
  Serial.println("M, Alignment Complete");
}