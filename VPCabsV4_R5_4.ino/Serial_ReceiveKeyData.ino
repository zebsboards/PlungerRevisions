void ReceiveDataKeys(void) {
  
  while(!Serial.available()) {Blink();};
  SetBlinkMode(1);                              // set Blink Speed for led
  delay(500);  
  if (Serial.available()> 0) {
     {
      byte n = Serial.available(); //to check that a charterer has come from Serial Monitor     
      if (n != 0){
        Blink();
        byte numBytesReceived = Serial.readBytesUntil('\n',buf,BUFFER_SIZE);
        buf[numBytesReceived] = '\0';
      }
     }    
     char*next;               //pointer to substring (token)
     next = strtok(buf, ","); //splitting string by "," and get 1st substring token
     
     if (receivedByte == 'I') {
       keys[0] = strtol(next, 0, 16);
       
       for (int a=1; a < KEY_SIZE; a++){
        next = strtok(NULL, ","); // print next string  and remove previous string
        keys[a] = strtol(next, 0, 16);
       }
       for (int a = 0; a < 32; a++){ 
        EEPROM.write(a+1, keys[a]);   // write to EEPROM 
       }
     }
     else if (receivedByte == 'C'){
       CD_VAL = strtol(next, 0, 16);
       EEPROM.write(69, CD_VAL);
     }
     else if (receivedByte == 'E'){
       EX_VAL = strtol(next, 0, 16);
       EEPROM.write(70, EX_VAL);
     }
     else if (receivedByte == 'L'){
       LB_VAL = strtol(next, 0, 16);
       EEPROM.write(66, LB_VAL);
     }
  } 
    
  EEPROM.write(0, 1);               // set keys array written flag

  SetBlinkMode(0);
   
}


 
