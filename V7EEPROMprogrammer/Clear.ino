void clear (void){
  for (int a = 0; a < 1000; a++)
  {
   eeprom.write(a, 0XFF);
   delay(1);
  }
}

