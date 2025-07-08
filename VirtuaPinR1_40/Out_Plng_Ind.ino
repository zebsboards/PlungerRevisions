void ShowIndicator(void) {
  int invertDirection;
  switch (plungerIndicatorMode) {
    case 1:                                                   // ON at Rest
      numberOfLeds = 15 - numberOfLeds;
      invertDirection = 0;
      break;
    case 2:                                                   // OFF at Rest
      numberOfLeds = numberOfLeds;
      invertDirection = 0;
      break;
    case 3:                                                   // ON Inverted
      numberOfLeds = 15 - numberOfLeds;
      invertDirection = 1;
      break;
    case 4:                                                   // OFF Inverted
      numberOfLeds = numberOfLeds;
      invertDirection = 1;
      break;
    default:
      break;
  }
  if (invertDirection == 0) {
    for (int a = 0; a < 15; a++) {
      if ((a < numberOfLeds) && (a < 5)) {
        plungerIndicator.setPixelColor(a, plungerIndicator.Color(0, 255, 0));
      } else if ((a < numberOfLeds) && (a > 4) && (a < 10)) {
        plungerIndicator.setPixelColor(a, plungerIndicator.Color(255, 255, 0));
      } else if ((a < numberOfLeds) && (a > 9)) {
       plungerIndicator.setPixelColor(a, plungerIndicator.Color(255, 0, 0));
      } else {
        plungerIndicator.setPixelColor(a, plungerIndicator.Color(0, 0, 0));
      }
      plungerIndicator.show();                                                  // Light the indicator lights
    }
  }
  else {
    for (int a = 0; a < 15; a++) {
      int8_t LedToLight = 14 - a;
      if ((a < numberOfLeds) && (a < 5)) {
        plungerIndicator.setPixelColor(LedToLight, plungerIndicator.Color(0, 255, 0));
      } else if ((a < numberOfLeds) && (a > 4) && (a < 10)) {
        plungerIndicator.setPixelColor(LedToLight, plungerIndicator.Color(255, 255, 0));
      } else if ((a < numberOfLeds) && (a > 9)) {
        plungerIndicator.setPixelColor(LedToLight, plungerIndicator.Color(255, 0, 0));
      } else {
        plungerIndicator.setPixelColor(LedToLight, plungerIndicator.Color(0, 0, 0));
      }
      plungerIndicator.show();                                                  // Light the indicator lights
    }
  }
}

void ClearIndicator(void) {
  switch (plungerIndicatorMode) {
    case 1:
      for (int a = 0; a < 15; a++) {
        if (a < 5) {
          plungerIndicator.setPixelColor(a, plungerIndicator.Color(0, 255, 0));
        } else if (a > 4 && a < 10) {
          plungerIndicator.setPixelColor(a, plungerIndicator.Color(255, 255, 0));
        } else {
          plungerIndicator.setPixelColor(a, plungerIndicator.Color(255, 0, 0));
        }
        plungerIndicator.show();
      }
    break;
    case 3:
      for (int a = 0; a < 15; a++) {
        int8_t LedToLight = 14 - a;
        if (a < 5) {
          plungerIndicator.setPixelColor(LedToLight, plungerIndicator.Color(0, 255, 0));
        } else if (a > 4 && a < 10) {
          plungerIndicator.setPixelColor(LedToLight, plungerIndicator.Color(255, 255, 0));
        } else {
          plungerIndicator.setPixelColor(LedToLight, plungerIndicator.Color(255, 0, 0));
        }
        plungerIndicator.show();
      }
    break;
    default:
      plungerIndicator.clear();
      plungerIndicator.show();
    break;
  }
}
