#include "at24c256.h"
#include <Wire.h>
at24c256 eeprom(0x50);  

int readval;



//---------------------------------------------------------------------------------------------------------
void setup() {
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  //while(!Serial);
  byte eightbitsettings[25]=
  {
    1,                               // Gamepad/Keyboard Mode Switch  0 = Keyboard Mode / 1 = Gamepad Mode
    1,                               // Use Coindoor (0 = Not Used, 1 = Use Coin Door routine - Single Click)
    5,                               // button number assigned to Coin Door Input
    16,                              // Launch Ball Button Assignment (position in Keys array)
    28,                              // Launch Ball Keypress Assignment
    0,                               // (0 = No Hatswitch, 1 = Use Hatswitch)
    1,                               // nudging enabled = 1, OFF = 0
    4,                               // nudging sensitivity +-2g,4g,8g,16g
    7,                               // nudge sensor orientation (LScableDown = 0, CFcableDown = 1, RScableDown = 2, LScableFlat = 3, CFcableFrontFlat = 4, RScableFlat = 5, CBcableDown = 6, CFcableBackFlat = Default)
    100,                    		 // tilt debounce value
   	0,                 				 // n/a
    1,                               // nudge X axis amplification
    1,                               // nudge Y axis amplification
    8,                               // nudge X axis deadzone value
    8,                               // nudge Y axis deadzone value
    0,                               // (1 = Enable Tilt routines, 0 = Disable tilt routines)
    17,                              // button number assigned to Tilt input
    147,                             // XTilt value received
    147,                             // YTilt value received
    0,                               // n/a
    0,                               // n/a
    0,                               // n/a
    0,                               // n/a
    0,                               // n/a 
    0                                // n/a
  };
  byte keys[32] = {                                                                     //    default original ZB plunger keystroke assignment and order
    0xE1,   // LShift  Left Flipper               Button 1
    0xE0,   // LCTRL   Left MSave                 Button 2  
    0x1E,   // 1       Start                      Button 3
    0x14,   // q       Exit                       Button 4
    0x1F,   // 2       Extra Buy-in               Button 5
    0x22,   // 5       Coin                       Button 6
    0x4D,   // END     Coin Door Open/Close       Button 7  
    0x2E,   // =       Volume DOWN                Button 8
    0x2D,   // -       Volume UP                  Button 9
    0x25,   // 8       SMenu Down                 Button 10
    0x26,   // 9       SMenu UP                   Button 11
    0x27,   // 0       SMenu Enter                Button 12
    0x24,   // 7       SMenu Cancel               Button 13
    0xE5,   // RShift  Right Flipper              Button 14
    0xE4,   // RCTRL   Right MSave                Button 15
    0x28,   // Enter   Launch/Fire   '28'         Button 16
    0x17,   // Tilt Button Assignment   t         Button 17
    0x00,   // null 'n'                           Button 18
    0x00,   // null 'n'                           Button 19
    0x00,   // null 'n'                           Button 20
    0x00,   // null 'n'                           Button 21
    0x00,   // null 'n'                           Button 22
    0x00,   // null 'n'                           Button 23
    0x00,   // null 'n'                           Button 24
    0x00,   // null 'n'                           Button 25
    0x00,   // null 'n'                           Button 26
    0x00,   // null 'n'                           Button 27
    0x00,   // null 'n'                           Button 28
    0x00,   // null 'n'                           Button 29
    0x00,   // null 'n'                           Button 30
    0x00,   // null 'n'                           Button 31
    0x00    // null 'n'                           Button 32
  };
  int16_t sixteenbitsettings[] = {
    870,    // plungerDrawLength
    955     // plungerPushLength
  };
  byte LabelCodes[32] = {
    0x01,0x03,0x05,0x06,0x08,0x09,0x0C,0x0F,0x0E,0x11,0x10,0x12,0x13,0x02,0x04,0x0A,0x17,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
  };

  eeprom.init();
  eeprom.write(0, 1);                //************************************** EEPROM Written Flag ******** 
  for (int8_t a = 0; a < 25; a++)    //**************************************  8 bit write ***************
  {
   eeprom.write(a + 100, eightbitsettings[a]); 
   delay(20);
  }
  for (int8_t a = 0; a < 32; a++)   //**************************************  8 bit write ***************
  {
   eeprom.write(a + 125, keys[a]);
   delay(10);
  }
  for (int8_t a = 0; a < 32; a++)   //**************************************  8 bit write ****************
  {
   eeprom.write(a + 200, LabelCodes[a]);
   delay(10);  
  }
  int count = 0;                    //*************************************** 16 bit write ***************
  for (int a = 0; a < 2; a++)
  {
    eeprom.write(157 + count, sixteenbitsettings[a] & 0xff);
    eeprom.write(158 + count, sixteenbitsettings[a] >> 8);
    count = count +2;
    delay(10);                                
  }
  /*for (int8_t a = 0; a < 25; a++)  //**************************************  8 bit read ***************
  {
   Serial.println(eeprom.read(a + 100)); 
   delay(20);
  }
  for (int8_t a = 0; a < 32; a++)   //**************************************  8 bit read ***************
  {
   Serial.println(eeprom.read(a + 125)); 
   delay(20);
  }
  count = 0;                        //*************************************** 16 bit read **************                          
  int16_t readVal;
  for (int a = 0; a < 6; a++)
  {
    readVal = eeprom.read(157 + count)+(eeprom.read(158 + count)<<8);
    Serial.println(readVal);
    count = count +2;
    delay(10);                                
  }
  Serial.println("");Serial.println("");Serial.println("COMPLETE"); Serial.println("");*/
  
  //eeprom.close();                                                                // Reset i2c frequency to 1700000hz
  digitalWrite(LED_BUILTIN, LOW);
  
}


void loop() {
}



