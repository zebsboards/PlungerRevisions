//************************************************** Cabinet button  In-Game sub routine *****************************

void Button(void) { 
    //******************************* i2c Button Read  - GAMEPAD - Gamepadflag = 1  *******************************
    
  if (Gamepadflag == 1){
   for (int bcount = 0; bcount < pincount; bcount++){
    if(digitalRead(pl_orig[bcount])== LOW ){
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
      else{
         if (keys[bcount] != 10){
            Pincab_Controller_V2HB.setButton(bcount, HIGH);
         }
      }
    }         
    if(digitalRead(pl_orig[bcount])== HIGH ){
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
      else{
         if (keys[bcount] != 10){
            Pincab_Controller_V2HB.setButton(bcount, LOW);
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
  E,    // q       Exit                       Button 4
  0x32, // 2       Extra Buy-in               Button 5
  0x35, // 5       Coin                       Button 6
  C,    // END     Coin Door Open/Close       Button 7  
  0x3D, // =       Volume DOWN                Button 8
  0x2D, // -       Volume UP                  Button 9
  0x38, // 8       SMenu Down                 Button 10
  0x39, // 9       SMenu UP                   Button 11
  0x30, // 0       SMenu Enter                Button 12
  0x37, // 7       SMenu Cancel               Button 13
  0x85, // RShift  Right Flipper              Button 14
  0x84, // RCTRL   Right MSave                Button 15
  B,    // Enter   Launch/Fire                Button 16
  A,    // Tilt Button Assignment             Button 17
*/
  for (int bcount = 0; bcount < pincount; bcount++){
      if(digitalRead(pl_orig[bcount])== LOW ){
        if (bcount > lengthofkeys){
           Pincab_Controller_V2HB.setButton(bcount, HIGH);  
        }
        else if(keys[bcount] == 12) {
           if(Coin_Door == 0){
             Keyboard.press(CD_VAL);Pincab_Controller_V2HB.setButton(bcount, HIGH);
             delay(50);
             Keyboard.release(CD_VAL);Pincab_Controller_V2HB.setButton(bcount, LOW);
             Coin_Door = 1;
           }
        }
        else if(keys[bcount] == 10) {
          Pincab_Controller_V2HB.setButton(tilt_button, HIGH);
        }
        else if(keys[bcount] == 11) {
          Keyboard.press(LB_VAL);Pincab_Controller_V2HB.setButton(bcount, HIGH);
        }
        else if(keys[bcount] == 14) {
          Keyboard.press(EX_VAL);Pincab_Controller_V2HB.setButton(bcount, HIGH);
        }
        else {
          if (keys[bcount] != 10){
            Keyboard.press(keys[bcount]);Pincab_Controller_V2HB.setButton(bcount, HIGH); 
          } 
        }
           
      }      
      if(digitalRead(pl_orig[bcount])== HIGH ){
        if (bcount > lengthofkeys){
           Pincab_Controller_V2HB.setButton(bcount, LOW);  
        }
        else if(keys[bcount] == 12) {
         if(Coin_Door == 1){
           Keyboard.press(CD_VAL);Pincab_Controller_V2HB.setButton(bcount, HIGH);
           delay(50);
           Keyboard.release(CD_VAL);Pincab_Controller_V2HB.setButton(bcount, LOW);
           Coin_Door = 0;
         }
        }
        else if(keys[bcount] == 10) {
          Pincab_Controller_V2HB.setButton(tilt_button, LOW);
        }
        else if(keys[bcount] == 11) {
          Keyboard.release(LB_VAL);Pincab_Controller_V2HB.setButton(bcount, LOW);
        }
        else if(keys[bcount] == 14) {
          Keyboard.release(EX_VAL);Pincab_Controller_V2HB.setButton(bcount, LOW);
        }
        else {
          if (keys[bcount] != 10){
            Keyboard.release(keys[bcount]);Pincab_Controller_V2HB.setButton(bcount, LOW); 
          }  
      }
     }
  }
 
} 
}
