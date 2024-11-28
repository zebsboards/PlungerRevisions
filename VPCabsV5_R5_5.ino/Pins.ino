void Pins (void) {

  switch (i2c_present) {

    case 0:

      pinMode(0, INPUT_PULLUP);
      pinMode(1, INPUT_PULLUP);
      pinMode(2, INPUT_PULLUP);
      pinMode(3, INPUT_PULLUP);
      pinMode(4, INPUT_PULLUP);
      pinMode(5, INPUT_PULLUP);
      pinMode(6, INPUT_PULLUP);
      pinMode(7, INPUT_PULLUP);
      pinMode(8, INPUT_PULLUP);
      pinMode(9, INPUT_PULLUP);
      pinMode(10, INPUT_PULLUP);
      pinMode(11, INPUT_PULLUP);
      pinMode(12, INPUT_PULLUP);
      pinMode(13, INPUT_PULLUP);
      pinMode(14, INPUT_PULLUP);
      pinMode(15, INPUT_PULLUP);
      pinMode(16, INPUT_PULLUP);
      pinMode(17, INPUT_PULLUP);
      pinMode(A0, INPUT);
      pinMode(A1, INPUT);
      pinMode(A2, INPUT);
      pinMode(A3, INPUT);
      pinMode(A4, INPUT);
      pinMode(A5, INPUT_PULLUP);
      pinMode(30, INPUT_PULLUP);

      
      break;
    
    
    case 1:

      pinMode(0, INPUT_PULLUP);
      pinMode(1, INPUT_PULLUP);             // Expansion Power OUTPUT Disabled
      pinMode(4, INPUT_PULLUP);
      pinMode(5, INPUT_PULLUP);
      pinMode(6, INPUT_PULLUP);
      pinMode(7, INPUT_PULLUP);
      pinMode(8, INPUT_PULLUP);
      pinMode(9, INPUT_PULLUP);
      pinMode(10, INPUT_PULLUP);
      pinMode(11, INPUT_PULLUP);
      pinMode(12, INPUT_PULLUP);
      pinMode(13, INPUT_PULLUP);
      pinMode(14, INPUT_PULLUP);
      pinMode(15, INPUT_PULLUP);
      pinMode(16, INPUT_PULLUP);
      pinMode(17, INPUT_PULLUP);
      pinMode(A0, OUTPUT);             // LED OUTPUT
      pinMode(A1, INPUT_PULLUP);
      pinMode(A2, INPUT);
      pinMode(A3, INPUT);
      pinMode(A4, INPUT);
      pinMode(A5, INPUT_PULLUP);
      pinMode(30, INPUT_PULLUP); 

      // pinMode(2, INPUT_PULLUP);      // SDA channel i2c bus
      // pinMode(3, INPUT_PULLUP);      // SCL channel i2c bus
      break;

    case 2:

      pinMode(0, INPUT_PULLUP);
      pinMode(1, OUTPUT);              // Expansion Power OUTPUT
      pinMode(4, INPUT_PULLUP);
      pinMode(5, INPUT_PULLUP);
      pinMode(6, INPUT_PULLUP);
      pinMode(7, INPUT_PULLUP);
      pinMode(8, INPUT_PULLUP);
      pinMode(9, INPUT_PULLUP);
      pinMode(10, INPUT_PULLUP);
      pinMode(11, INPUT_PULLUP);
      pinMode(12, INPUT_PULLUP);
      pinMode(13, INPUT_PULLUP);
      pinMode(14, INPUT_PULLUP);
      pinMode(15, INPUT_PULLUP);
      pinMode(16, INPUT_PULLUP);
      pinMode(17, INPUT_PULLUP);
      pinMode(A0, INPUT);
      pinMode(A1, INPUT);
      pinMode(A2, INPUT);
      pinMode(A3, INPUT);
      pinMode(A4, INPUT);
      pinMode(A5, INPUT_PULLUP);
      pinMode(30, INPUT_PULLUP); 

      // pinMode(2, INPUT_PULLUP);      // SDA channel i2c bus
      // pinMode(3, INPUT_PULLUP);      // SCL channel i2c bus
      break;  

    case 3:

      pinMode(0, INPUT_PULLUP);
      pinMode(1, OUTPUT);              // Expansion Power OUTPUT
      pinMode(4, INPUT_PULLUP);
      pinMode(5, INPUT_PULLUP);
      pinMode(6, INPUT_PULLUP);
      pinMode(7, INPUT_PULLUP);
      pinMode(8, INPUT_PULLUP);
      pinMode(9, INPUT_PULLUP);
      pinMode(10, INPUT_PULLUP);
      pinMode(11, INPUT_PULLUP);
      pinMode(12, INPUT_PULLUP);
      pinMode(13, INPUT_PULLUP);
      pinMode(14, INPUT_PULLUP);
      pinMode(15, INPUT_PULLUP);
      pinMode(16, INPUT_PULLUP);
      pinMode(17, INPUT_PULLUP);
      pinMode(A0, OUTPUT);             // LED OUTPUT
      pinMode(A1, INPUT_PULLUP);
      pinMode(A2, INPUT);
      pinMode(A3, INPUT);
      pinMode(A4, INPUT);
      pinMode(A5, INPUT_PULLUP);
      pinMode(30, INPUT_PULLUP); 

      // pinMode(2, INPUT_PULLUP);      // SDA channel i2c bus
      // pinMode(3, INPUT_PULLUP);      // SCL channel i2c bus
      break;

    

  }
  
}

