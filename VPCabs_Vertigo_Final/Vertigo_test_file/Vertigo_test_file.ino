

int inputs[]{3,2,0,1,4,12,6,8,9,10,5,13}; // input pins used

void setup() {
  
  for (int a = 0; a < 12; a++) {
    pinMode(inputs[a], OUTPUT);
    digitalWrite(inputs[a], LOW);
  }
  

}

void loop() {

for (int a = 0; a < 12; a++) {
    digitalWrite(inputs[a], HIGH);
    delay(300);
    digitalWrite(inputs[a], LOW);
  }

}
