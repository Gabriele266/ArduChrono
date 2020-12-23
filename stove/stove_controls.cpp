#ifndef STOVE_CONTROL
#define STOVE_CONTROL

#include "../ArduChrono/objects.h"
#include "../ArduChrono/costants.h"

void increment_pwr(int num){
  // num è la quantità di volte di cui si vuole incrementare la potenza
  for (int count = 0; count <= num; count ++){
    // Incremento la potenza di tante volte quante ne vuole l'utente
    PWR.closeCircuit();     // |
    delay(200);             // |  Cambia di 1
    PWR.openCircuit();      // |
    delay(200);             // |
    pwr += 1;               // Incremento di 1 la potenza ogni volta
  }
}
void increment_vent(int num){
  // num è la quantità di volte di cui si vuole incrementare la potenza
  for (int count = 0; count <= num; count ++){
    // Incremento la potenza di tante volte quante ne vuole l'utente
    VENT.closeCircuit();     // |
    delay(200);             // |  Cambia di 1
    VENT.openCircuit();      // |
    delay(200);             // |
    vent += 1;               // Incremento di 1 la potenza ogni volta
  }
}

void turn_stove_on(){
  // Accende la stufa con una potenza di 5 e la ventola a 3
  STOVE.closeCircuit();                 // Accendo la stufa
  stove = ON;                           // Imposto la stufa su on
  updateStatusLed();                    // Aggiorno i led
  // Parte a 2
  pwr = 2;
  vent = 3;
}



#endif // STOVE_CONTROL
