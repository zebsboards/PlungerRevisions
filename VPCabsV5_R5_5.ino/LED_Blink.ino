//Sets the mode for the blinking of the led

void SetBlinkMode(int Mode) {
  BlinkMode=Mode;
  BlinkModeTimeoutTimer=0;
}

//Controls the blinking of the led

void Blink() {

  if ((i2c_present == 1)||(i2c_present == 3)){
  
  switch(BlinkMode) {
    case 0:
      //Blinkmode 0 
      if(BlinkTimer<1500) {
        digitalWrite(LEDOUT,0);
      } else if(BlinkTimer<1600) {
        digitalWrite(LEDOUT,1);
      } else {
        BlinkTimer=0;
        digitalWrite(LEDOUT,0);
      }
    break;
    case 1:
      //Blinkmode 1 is activated when the plunger serial port is active
      //Mode expires 500ms after the last byte has been received resp. mode has been set
      if(BlinkTimer>30) {
        BlinkTimer=0;
        digitalWrite(LEDOUT,!digitalRead(LEDOUT));
      }
      if(BlinkModeTimeoutTimer>500) {
        SetBlinkMode(2);
      }
    break;   
    case 2:
      //Blinkmode 2 is active while the plunger is waiting for more commands
      if(BlinkTimer<1500) {
        digitalWrite(LEDOUT,0);
      } else if(BlinkTimer<1600) {
        digitalWrite(LEDOUT,1);
      } else if(BlinkTimer<1700) {
        digitalWrite(LEDOUT,0);
      } else if(BlinkTimer<1800) {
        digitalWrite(LEDOUT,1);
      }else {
        BlinkTimer=0;
        digitalWrite(LEDOUT,0);
      }
    break;  
    default:
      //This should never be active
      //The code is only here to make it easier to determine if a wrong Blinkcode has been set
      if(BlinkTimer>2000) {
        BlinkTimer=0;
        digitalWrite(LEDOUT,!digitalRead(LEDOUT));
      }
      break;
  }
  }
  else {
  }
}
