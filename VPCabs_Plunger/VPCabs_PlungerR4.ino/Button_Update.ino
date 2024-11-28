void Buttoninput(void) {  //************************************************** Cabinet button  In-Game sub routine *****************************

  // Read and set the button states

  if (Gamepadflag > 0) {

    button0.update();    // Left Flipper

    if (button0.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(0, HIGH);
    }
    if (button0.risingEdge())
    {
      Pincab_ControllerVPC.setButton(0, LOW);
    }

    button1.update();    // LEFT MAGNASAVE

    if (button1.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(1, HIGH);
    }
    if (button1.risingEdge())
    {
      Pincab_ControllerVPC.setButton(1, LOW);
    }

    button2.update();    //START

    if (button2.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(2, HIGH);
    }
    if (button2.risingEdge())
    {
      Pincab_ControllerVPC.setButton(2, LOW);
    }

    button3.update();   //EXIT

    if (button3.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(3, HIGH);
    }
    if (button3.risingEdge())
    {
      Pincab_ControllerVPC.setButton(3, LOW);
    }

    button4.update();    //EXTRA BALL / BUY IN

    if (button4.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(4, HIGH);
    }
    if (button4.risingEdge())
    {
      Pincab_ControllerVPC.setButton(4, LOW);
    }

    button5.update();   // COIN

    if (button5.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(5, HIGH);
    }
    if (button5.risingEdge())
    {
      Pincab_ControllerVPC.setButton(5, LOW);
    }

    button6.update();    // COIN DOOR OPEN

    if (button6.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(6, HIGH);
      delay (10);
      Pincab_ControllerVPC.setButton(6, LOW);
    }
    if (button6.risingEdge())
    {
      Pincab_ControllerVPC.setButton(6, HIGH);
      delay (10);
      Pincab_ControllerVPC.setButton(6, LOW);
    }

    button7.update();    // VOLUME DOWN

    if (button7.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(10, HIGH);
    }
    if (button7.risingEdge())
    {
      Pincab_ControllerVPC.setButton(10, LOW);
    }

    button8.update();    // VOLUME UP

    if (button8.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(9, HIGH);
    }
    if (button8.risingEdge())
    {
      Pincab_ControllerVPC.setButton(9, LOW);
    }

    button9.update();    // SERVICE MENU DOWN

    if (button9.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(8, HIGH);
    }
    if (button9.risingEdge())
    {
      Pincab_ControllerVPC.setButton(8, LOW);
    }

    button10.update();   // SERVICE MENU UP

    if (button10.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(7, HIGH);
    }
    if (button10.risingEdge())
    {
      Pincab_ControllerVPC.setButton(7, LOW);
    }

    button11.update();    // SERVICE MENU ENTER

    if (button11.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(13, HIGH);
    }
    if (button11.risingEdge())
    {
      Pincab_ControllerVPC.setButton(13, LOW);
    }

    button12.update();    // SERVICE MENU CANCEL

    if (button12.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(14, HIGH);
    }
    if (button12.risingEdge())
    {
      Pincab_ControllerVPC.setButton(14, LOW);
    }

    button13.update();   // RIGHT FLIPPER

    if (button13.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(15, HIGH);
    }
    if (button13.risingEdge())
    {
      Pincab_ControllerVPC.setButton(15, LOW);
    }

    button14.update();    // RIGHT MAGNASAVE

    if (button14.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(16, HIGH);
    }
    if (button14.risingEdge())
    {
      Pincab_ControllerVPC.setButton(16, LOW);
    }

    button15.update();    // LAUNCH BALL

    if (button15.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(17, HIGH);
    }
    if (button15.risingEdge())
    {
      Pincab_ControllerVPC.setButton(17, LOW);
    }

    button16.update();    // GAMEPAD BUTTON 1

    if (button16.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(12, HIGH);
    }
    if (button16.risingEdge())
    {
      Pincab_ControllerVPC.setButton(12, LOW);
    }

    button17.update();    // GAMEPAD BUTTON 2

    if (button17.fallingEdge())
    {
      Pincab_ControllerVPC.setButton(11, HIGH);
    }
    if (button17.risingEdge())
    {
      Pincab_ControllerVPC.setButton(11, LOW);
    }
  }
  else {

    // Read and set the button states

  button0.update();    // Left Flipper

  if (button0.fallingEdge())
  {
    Keyboard.press(KEY_LEFT_ARROW);         // LEFT_ARROW
  }  
  if (button0.risingEdge())
  {
    Keyboard.release(KEY_LEFT_ARROW);
  }

  button1.update();    // LEFT MAGNASAVE

  if (button1.fallingEdge())
  {
    Keyboard.press(KEY_LEFT_CTRL);           // LEFT_CTRL
  }
  if (button1.risingEdge())
  {
    Keyboard.release(KEY_LEFT_CTRL);
  }

  button2.update();    //START

  if (button2.fallingEdge())
  {
    Keyboard.press('1');             // 1
  }
  if (button2.risingEdge())
  {
    Keyboard.release('1');
  }

  button3.update();   //EXIT

  if (button3.fallingEdge())
  {
    Keyboard.press(KEY_ESC);         // Esc 
  }
  if (button3.risingEdge())
  {
    Keyboard.release(KEY_ESC);
  }

  button4.update();    //Coin

  if (button4.fallingEdge())
  {
    Keyboard.press('5');            // 5
  }
  if (button4.risingEdge())
  {
    Keyboard.release('5');
  }

  button5.update();   // R_FLIPPER

  if (button5.fallingEdge())
  {
    Keyboard.press(KEY_RIGHT_ARROW);         // R_ARROW
  }
  if (button5.risingEdge())
  {
    Keyboard.release(KEY_RIGHT_ARROW);
  }

  button6.update();    // R_MAGNASAVE

  if (button6.fallingEdge())
  {
    Keyboard.press(KEY_RIGHT_CTRL);        // R_CTRL
  }
  if (button6.risingEdge())
  {
    Keyboard.release(KEY_RIGHT_CTRL);
  }

  button7.update();    // LAUNCH

  if (button7.fallingEdge())
  {
    Keyboard.press(0xB0);        // ENTER
  }
  if (button7.risingEdge())
  {
    Keyboard.release(0xB0);
  }

  button8.update();    //GP1

  if (button8.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(0, HIGH);
  }
  if (button8.risingEdge())
  {
    Pincab_ControllerVPC.setButton(0, LOW);
  }

  button9.update();    //GP2

  if (button9.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(1, HIGH);
  }
  if (button9.risingEdge())
  {
    Pincab_ControllerVPC.setButton(1, LOW);
  }

  button10.update();   //GP3

  if (button10.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(2, HIGH);
  }  
  if (button10.risingEdge())
  {
    Pincab_ControllerVPC.setButton(2, LOW);
  }

  button11.update();    //GP4

  if (button11.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(3, HIGH);
  }
  if (button11.risingEdge())
  {
    Pincab_ControllerVPC.setButton(3, LOW);
  }

  button12.update();    //GP5

  if (button12.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(4, HIGH);
  }
  if (button12.risingEdge())
  {
    Pincab_ControllerVPC.setButton(4, LOW);
  }

  button13.update();   //GP6

  if (button13.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(5, HIGH);
  }
  if (button13.risingEdge())
  {
    Pincab_ControllerVPC.setButton(5, LOW);
  }

  button14.update();    //GP7

  if (button14.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(6, HIGH);
  }
  if (button14.risingEdge())
  {
    Pincab_ControllerVPC.setButton(6, LOW);
  }

  button15.update();    //GP8

  if (button15.fallingEdge())
  {
   Pincab_ControllerVPC.setButton(7, HIGH);
  }
  if (button15.risingEdge())
  {
   Pincab_ControllerVPC.setButton(7, LOW);
  }

  button16.update();    //GP9

  if (button16.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(8, HIGH);
  }
  if (button16.risingEdge())
  {
    Pincab_ControllerVPC.setButton(8, LOW);
  }

  button17.update();    //GP10

  if (button17.fallingEdge())
  {
    Pincab_ControllerVPC.setButton(9, HIGH);
  }
  if (button17.risingEdge())
  {
    Pincab_ControllerVPC.setButton(9, LOW);
  }
  }
}
