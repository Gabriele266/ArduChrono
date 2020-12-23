#ifndef CICLE_FUN
#define CICLE_FUN

#include "../ArduChrono/objects.h"
#include "../ArduChrono/costants.h"

void checkCicle() {
  //controllo la modalità da utilizzare
  //le modalità vengono scaturite dalle variabili globali booleane check_cicle_daily e check_cicle_bi_daily
  //se la modalità è la daily
  bool acc = true; bool spegn = true;
  //variabili per il controllo dello stato
  bool a_acc = true; bool p_acc = true;
  bool a_spegn = true; bool p_spegn = true;
  if (check_cicle_daily) {
    //variabili per il controllo dello stato

    //controllo lora corrente
    saveTime();

    if (mydata.hour == acc_time && mydata.minute == acc_time_minute && acc == true) {
      //accendo la stufa
      STOVE.closeCircuit();
      stove = ON;
      //imposto come accesa
      acc = false;
      //aggiorno il pannello
      updateStatusLed();
      pwr = 2;
      vent = 3;
    }
    if (mydata.hour == spegn_time && mydata.minute == spegn_time_minute && spegn == true) {
      //spengo la stufa
      STOVE.openCircuit();
      stove = OFF;
      //imposto come spenta
      spegn = false;
      //aggiorno il pannello
      updateStatusLed();
      vent = 0;
      pwr = 0;
    }
  }
  if (check_cicle_bi_daily){



    //l'utente ha selezionato la modalità doppia
    //permette di impostare un modello per la mattina
    //e un modello per il pomeriggio

    if (mydata.hour == bi_m_acc_time && mydata.minute == bi_m_acc_time_minute && a_acc == true){
      //è l'ora di accendere
      STOVE.closeCircuit();
      stove = ON;
      //loa stufa è accesa e la imposto come da non accendere
      a_acc = false;
      //aggiorno il pannello
      updateStatusLed();
      pwr = 2;
      vent = 3;
    }
    if (mydata.hour == bi_m_spegn_time && mydata.minute == bi_m_spegn_time_minute && a_spegn == true){
      //spengo il relay
      STOVE.openCircuit();
      stove = OFF;
      //imposto come spenta
      a_spegn = false;
      //aggiorno il pannello
      updateStatusLed();
      pwr = 0;
      vent = 0;
    }
    if (mydata.hour == bi_p_acc_time && mydata.minute == bi_p_acc_time_minute && p_acc == true){
      //accendo il relay
      STOVE.closeCircuit();
      stove = ON;
      //imposto come accesa
      p_acc = false;
      //aggiorno il pannello
      updateStatusLed();
      pwr = 2;
      vent = 3;
    }
    if (mydata.hour == bi_p_spegn_time && mydata.minute == bi_p_spegn_time_minute && p_spegn == true){
      //spengo il relay
      STOVE.openCircuit();
      stove = OFF;
      //imposto come spenta
      p_spegn = false;
      //aggiorno il pannello
      updateStatusLed();
      pwr = 0;
      vent = 0;
    }
   }
}

#endif // CICLE_FUN
