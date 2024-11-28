void ReceiveDataOP(void) {

/*  //grab 1 byte at a time and process into output format

  SetBlinkMode(1);                              // set Blink Speed for led
  while(!Serial.available()) {};
  Blink();
  byte OPData=Serial.read();
  OPvalue[0]= OPData; 
  Serial.write(OPData); 
  for (int d = 1; d<32; d++){  
    while(!Serial.available()) {};
     OPData=(OPData<<8)|Serial.read();
     OPvalue[d]= OPData;
     //Serial.write(OPData);
  }  
  Ack();
  for (int e = 0; e < 32; e++){
    OPvalue[e] = map(OPvalue[e], 0, 255, 0, 4096);
  }
  Output(); */                                         // send out OPvalue to output chips
}

