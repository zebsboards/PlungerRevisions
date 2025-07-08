void Plunger(void){
  getPosition();                                                                                                //  get a reading from the plunger                                                                                      
  int valueToSend;                                                                                              //  the calculated value based on reading and type of event (push/pull)
  if (position != analogAtRest){                                                                                //  When the plunger is not at the resting point (moving)
    if (push == false && pull == false){                                                                        //  if this is the 1st time it has moved
      if (position < 1023 && position > 890){                                                                   //  if it is being pushed?
        push = true;                                                                                            //  set the push flag
        if (analogMode != true){                                                                                //  if in digital button mode
          ZBV7.setButton(LaunchBallButton, HIGH);                                                               //  press launch ball button
          if (GamepadFlag != true){ Keyboard.pressRaw(LBKey);}                                                  //  and if in keyboard mode press Launch Ball key
        }
      }
      else {pull = true;}                                                                                       //  otherwise, it is being pulled so set the pull flag
    }
    if (pull == true ){                                                                                         //  in a PULL operation:
      if (position > 900 && position < 1023) {                                                                  //    plunger is below rest point (bounce on release)  
        position = position - 1023;                                                                             //    adjust to scale below zero
        position = position * -1;                                                                               //    bring the reading back above zero
      }
      else if (position > pullMax){position = pullMax;}                                                         //    if the reading exceeds the calibrated maximum pull value use the calibrated value
      else if (position > 1022){position = 0;}                                                                  //    if it is at rest or at the rest position set the value to zero
      valueToSend = map(position,0,pullMax,0,127);                                                              //    remap the reading to the scale for sending to the plunger
    }
    else if (push == true){                                                                                     //  in a PUSH operation:
      if (position < pushMax){valueToSend = pushMax;}                                                           //    if the reading exceeds the calibrated maximum push value use the calibrated value
      valueToSend = map(valueToSend,1022,pushMax,0,127);                                                        //    remap the reading to the scale for sending to the plunger
      if(valueToSend >= MaxMoved){                                                                              //    if the plunger is being pushed in or is maxxed out
        MaxMoved = valueToSend;                                                                                 //    store that reading
      }
      else{                                                                                                     //    Plunger is moving back to rest
        if (analogMode != true){                                                                                //    if in digital button mode
          ZBV7.setButton(LaunchBallButton, LOW);                                                                //    release launch ball button
          if (GamepadFlag != true){                                                                             //    if in keyboard mode
            Keyboard.releaseRaw(LBKey);                                                                         //    release Launch Ball key
          }
          else{ valueToSend = 0;}                                                                               //    otherwise send the at rest point (zero)
        }
      }
    }
    timer = 0;                                                                                                  // in either instance the plunger is in use so reset the timer to zero
  }
  else {                                                                                                        // When the plunger is at resting point
    valueToSend = 0;                                                                                            // the reading to send is zero
    if (timer == 0 && (push == true || pull == true)){timer = millis();}                                        // if the timer hasn't been started and the plunger has been in use start the timer
    else if (timer > 250){ push = false; pull = false; MaxMoved = 0; timer = 0;}                                // if it has been at rest long enough, consider it stopped and reset the flags and variables
  }
  ZBV7.setZAxis(valueToSend);                                                                                   // report the position over usb
}


void getPosition(void) {
  position = analogRead(plungerPotPin);                                                                        //  Current position of plunger - Potentiometer
}