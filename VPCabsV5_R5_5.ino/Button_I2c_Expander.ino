//************************************************** Cabinet button  In-Game sub routine *****************************

void ButtonI2c(void) { 
    //******************************* i2c Button Read  - GAMEPAD - Gamepadflag = 1  *******************************

if (Gamepadflag > 0) {
  
    for (int bcount = 0; bcount < 16; bcount++){ 
     if(mcp1.digitalRead(bcount)== LOW ){
       if(keys[bcount] == 12) {
         if(Coin_Door == 0){
           Pincab_Controller_V2HB.setButton((bcount), HIGH);
           delay (50);
           Pincab_Controller_V2HB.setButton((bcount), LOW);
           Coin_Door = 1;
         }
       }
       else if(keys[bcount] == 10) {
          Pincab_Controller_V2HB.setButton(tilt_button, HIGH);
       } 
       else {
          if (keys[bcount] != 10){
            Pincab_Controller_V2HB.setButton((bcount), HIGH);
          }  
       }
     }
     if(mcp1.digitalRead(bcount)== HIGH ){
       if(keys[bcount] == 12) {
         if(Coin_Door == 1){
           Pincab_Controller_V2HB.setButton((bcount), HIGH);
           delay (50);
           Pincab_Controller_V2HB.setButton((bcount), LOW);
           Coin_Door = 0;
         }
       }
       else if(keys[bcount] == 10) {
          Pincab_Controller_V2HB.setButton(tilt_button, LOW);
       } 
       else {
          if (keys[bcount] != 10){
            Pincab_Controller_V2HB.setButton((bcount), LOW);
          }  
       }
     }
  
     if(mcp2.digitalRead(bcount)== LOW ){
       if(keys[bcount+16] == 12) {
         if(Coin_Door == 0){
           Pincab_Controller_V2HB.setButton((bcount+16), HIGH);
           delay (50);
           Pincab_Controller_V2HB.setButton((bcount+16), LOW);
           Coin_Door = 1;
         }
       }
       else if(keys[bcount+16] == 10) {
          Pincab_Controller_V2HB.setButton(tilt_button, HIGH);
       } 
       else {
          if (keys[bcount+16] != 10){
            Pincab_Controller_V2HB.setButton((bcount+16), HIGH);
          }  
       }
     }
     if(mcp2.digitalRead(bcount)== HIGH ){
       if(keys[bcount+16] == 12) {
         if(Coin_Door == 1){
           Pincab_Controller_V2HB.setButton((bcount+16), HIGH);
           delay (50);
           Pincab_Controller_V2HB.setButton((bcount+16), LOW);
           Coin_Door = 0;
         }
       }
       else if(keys[bcount+16] == 10) {
          Pincab_Controller_V2HB.setButton(tilt_button, LOW);
       } 
       else {
          if (keys[bcount+16] != 10){
            Pincab_Controller_V2HB.setButton((bcount+16), LOW);
          }  
       }
     }
   } 
  
  }  
  else {   // ******************* i2c Button Read - Keyboard - Gamepadflag = 0  *******************************************************      

     
  /*
  default keys array - reference only
  
  0x81, // LShift  Left Flipper               Button 1
  0x80, // LCTRL   Left MSave                 Button 2  
  0x31, // 1       Start                      Button 3
  0x71, // q       Exit                       Button 4
  0x32, // 2       Extra Buy-in               Button 5
  0x35, // 5       Coin                       Button 6
  END,  // END     Coin Door Open/Close       Button 7  
  0x3D, // =       Volume DOWN                Button 8
  0x2D, // -       Volume UP                  Button 9
  0x38, // 8       SMenu Down                 Button 10
  0x39, // 9       SMenu UP                   Button 11
  0x30, // 0       SMenu Enter                Button 12
  0x37, // 7       SMenu Cancel               Button 13
  0x85, // RShift  Right Flipper              Button 14
  0x84, // RCTRL   Right MSave                Button 15
  0xB0, // Enter   Launch/Fire                Button 16
  TB,   // Tilt Button Assignment             Button 17
*/

    if (lengthofkeys == 16){
      full_1st();
      full_2nd();
    }

    else if (lengthofkeys > 16){
      full_1st();
      part_2nd();
    }

    else{
      part_1st();
      full_2nd();
    }
  }
}  
  
//*******************************1st expander Keyboard Only**************************************  
void full_1st (void){
  for (int bcount = 0; bcount < 16; bcount++){
      if(mcp1.digitalRead(bcount)== LOW ){
        if(keys[bcount] == 12) {
           if(Coin_Door == 0){
             Keyboard.press(CD_VAL);
             delay(50);
             Keyboard.release(CD_VAL);
             Coin_Door = 1;
           }
        }
        else if(keys[bcount] == 10) {
          Pincab_Controller_V2HB.setButton(tilt_button, HIGH);
        }
        else if(keys[bcount] == 11) {
          Keyboard.press(LB_VAL);
        }
        else if(keys[bcount] == 14) {
          Keyboard.press(EX_VAL);
        }
        else {
          if (keys[bcount] != 10){
            Keyboard.press(keys[bcount]); 
          } 
        }   
      }      
      if(mcp1.digitalRead(bcount)== HIGH ){
        if(keys[bcount] == 12) {
         if(Coin_Door == 1){
           Keyboard.press(CD_VAL);
           delay(50);
           Keyboard.release(CD_VAL);
           Coin_Door = 0;
         }
        }
        else if(keys[bcount] == 10) {
          Pincab_Controller_V2HB.setButton(tilt_button, LOW);
        }
        else if(keys[bcount] == 11) {
          Keyboard.release(LB_VAL);
        }
        else if(keys[bcount] == 14) {
          Keyboard.release(EX_VAL);
        }
        else {
          if (keys[bcount] != 10){
            Keyboard.release(keys[bcount]); 
          }  
      }
     }
  }
}


//*******************************2nd expander Gamepad Only**************************************
void full_2nd (void){
  for (int bcount = 0; bcount < 16; bcount++){ 
     if(mcp2.digitalRead(bcount)== LOW ){
       Pincab_Controller_V2HB.setButton((bcount+16), HIGH);
     }   
     else {
      Pincab_Controller_V2HB.setButton((bcount+16), LOW);
     }
   }
}
//******************************1st partial Keyboard********************************
void part_1st (void){  
   for (int bcount = 0; bcount < (lengthofkeys); bcount++){
      if(mcp1.digitalRead(bcount)== LOW ){
        if(keys[bcount] == 12) {
           if(Coin_Door == 0){
             SingleClickKB();
             Coin_Door = 1;
           }
        }
        else if(keys[bcount] == 10) {
          Pincab_Controller_V2HB.setButton(tilt_button, HIGH);
        }
        else if(keys[bcount] == 11) {
          Keyboard.press(LB_VAL);
        }
        else if(keys[bcount] == 14) {
          Keyboard.press(EX_VAL);
        }
        else {
          if (keys[bcount] != 10){
            Keyboard.press(keys[bcount]); 
          } 
        }   
      }      
      if(mcp1.digitalRead(bcount)== HIGH ){
        if(keys[bcount] == 12) {
         if(Coin_Door == 1){
           SingleClickKB();
           Coin_Door = 0;
         }
        }
        if(keys[bcount] == 10) {
          Pincab_Controller_V2HB.setButton(tilt_button, LOW);
        }
        if(keys[bcount] == 11) {
          Keyboard.release(LB_VAL);
        }
        else if(keys[bcount] == 14) {
          Keyboard.release(EX_VAL);
        }
        else {
          if (keys[bcount] != 10){
            Keyboard.release(keys[bcount]); 
          }  
      }
     }
  for (int bcount = (lengthofkeys); bcount < 16; bcount++){ 
     if(mcp1.digitalRead(bcount)== LOW ){
       Pincab_Controller_V2HB.setButton((bcount), HIGH);
     }
     else {
       Pincab_Controller_V2HB.setButton((bcount), LOW);
     }
  }
   }  
}
//******************************2nd partial Keyboard***************************************  
  
void part_2nd (void){
   for (int bcount = 0; bcount < (lengthofkeys - 16); bcount++){
      if(mcp2.digitalRead(bcount)== LOW ){
        if(keys[bcount] == 12) {
           if(Coin_Door == 0){
             SingleClickKB();
             Coin_Door = 1;
           }
        }
        if(keys[bcount+16] == 10) {
          Pincab_Controller_V2HB.setButton(tilt_button, HIGH);
        }
        else if(keys[bcount] == 11) {
          Keyboard.press(LB_VAL);
        }
        else if(keys[bcount] == 14) {
          Keyboard.press(EX_VAL);
        }
        else {
          if (keys[bcount+16] != 10){
            Keyboard.press(keys[bcount+16]);
          }  
        } 
      }      
      if(mcp2.digitalRead(bcount)== HIGH ){
        if(keys[bcount] == 12) {
         if(Coin_Door == 1){
           SingleClickKB();
           Coin_Door = 0;
         }
        }
        if(keys[bcount+16] == 10) {
          Pincab_Controller_V2HB.setButton(tilt_button, LOW);
        }
        else if(keys[bcount] == 11) {
          Keyboard.release(LB_VAL);
        }
        else if(keys[bcount] == 14) {
          Keyboard.release(EX_VAL);
        }
        else {
          if (keys[bcount+16] != 10){
            Keyboard.release(keys[bcount+16]); 
          }  
        }
      }
     }
  for (int bcount = (lengthofkeys - 16); bcount < 16; bcount++){ 
     if(mcp2.digitalRead(bcount)== LOW ){
       Pincab_Controller_V2HB.setButton((bcount+16), HIGH);
     }
     else {
       Pincab_Controller_V2HB.setButton((bcount+16), LOW);
     }
  } 
}
  
void SingleClickKB(void){
      Keyboard.press(CD_VAL);
      delay(50);
      Keyboard.release(CD_VAL);
  
}




 
