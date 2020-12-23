/*
 * Autore: cavallo gabriele
 * versione: 1.3.0
 * File principale del progetto arduchrono
 */
#include "ArduChrono/CoreMain.cpp"

// inizializzazione della scheda
void setup(){
  // chiamo il setup
  core_setup();
}

// aggiornamento
void loop(){
  core_loop();
}
