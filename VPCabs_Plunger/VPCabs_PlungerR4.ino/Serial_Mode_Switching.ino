void SerCTRL(void) {

  while (Serial.available()) {
    delay (30);
    char c = Serial.read();
    readString += c ;
  }
  if (readString.length() > 0) {
    if (readString == "ZBGP") {
      Gamepadflag = 1;          // set to gamepad mode
    }
    if (readString == "ZBKB") {
      Gamepadflag = 0;          // set to keyboard mode
    }
    if (readString == "ZBLA") {
      zblaunchflag = 0;          // set plunger mode to analog
    }
    if (readString == "ZBLD") {
      zblaunchflag = 1;          // set plunger mode to digital ball launch
    }
    else {
      readString = "";
    }
    readString = "";
  }

}
