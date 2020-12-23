#ifndef GENERIC_BUTTON
#define GENERIC_BUTTON

#include "../ArduChrono/objects.h"
#include "../ArduChrono/costants.h"
#include <Arduino.h>

void checkButton() {
  // controlla lo stato del pulsante
  String st;
  BACK_SWITCH.updateState(&st);

  if (BACK_SWITCH.pressed) {
    //quando viene premuto
    if (BACK_SWITCH.first_pressed) {
      //se è la pria volta accende la back
      lcd.backlight();
    }
    else {
      lcd.noBacklight();
    }
  }
}

#endif // GENERIC_BUTTON
