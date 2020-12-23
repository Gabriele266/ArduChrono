/*
* Autore:
* Versione:
* Descrizione:
* Progetto:
*
*/
#ifndef TEMPERATURES_CPP
#define TEMPERATURES_CPP


#include <Arduino.h>
#include <SimpleDHT.h>                  // Controllo del sensore DHT22
#include "../ArduChrono/objects.h"
#include "../ArduChrono/costants.h"

void maxMinPrinter(String syntax_divider, float home_max, float home_min, float est_max, float est_min, float stove_max, float stove_min) {
  //funzione chiamata dal metodo calcMaxTemp
  //scrive tutte le informazioni sul pannello e sul seriale
  //per il pannello:
  //M7 per la massima della casa
  //M8 per la minima della casa
  //M9 per la massima esterna
  //M0 per la minima esterna
  //variabile che conterrà tutti i messaggi seriali
  String maxminmsg;

  //identificatore del comando e tipologia di azione
  //la tipologia di comando che invierà sarà ad esempio:
  //per ogni oggetti viene modificato il nome
  //TEMP-MAX-HOME-valore
  mywifi.sendCommand(String("#M7" + String(home_max) + "/" + String(home_min)));
  mywifi.sendCommand(String("#M8" + String(est_max) + "/" + String(est_min)));


  //--------------------------------------------------------------------------
}

/// calcola la temperatura massima tra quelle inserite
void calcMaxTemp(void (*print_provider) (String syntax_divider, float home_max, float home_min, float est_max, float est_min, float stove_max, float stove_min)) {
  //questo metodo, per garantire il corretto funzionamento, deve essere chiamato al salvataggio della temperatura
  //le temperature che utilizza sono: temp. casa temp. esterna e temp. stufa
  //restituisce anche i valori medi
  //per capire il funzionamento osservate l'immagine algoritmo_min_max.png
  //parte per la temperatura della casa
  //--------------------------------------------------------------------------------------------------
  if (DHT22TEMP.temperature > home_max_temp) {
    //la temperatura corrente(DHT22TEMP.teperature) e' maggiore della massima
    //allora la salvo come nuova massima
    home_max_temp = DHT22TEMP.temperature;
    //questa e le altre massime e minime verranno scritte sul pannello e sul seriale
    //ma solo se l'utente vuole
  }
  if (DHT22TEMP.temperature < home_min_temp) {
    //la temperatura è minore della minima assoluta
    //la imposto come nuova minima
    home_min_temp = DHT22TEMP.temperature;
  }

  if (esternal_temperature.temperature > esternal_max_temp) {
    //la temperatura è maggiore della massima
    // Controllo che sia plausibile come temperatura
    if(esternal_temperature.temperature > 50){
      // Mostro l'errore sul seriale
      Serial.println(temperature_not_plausible);
    }
    else{
      // La temperatura è plausibile
      // La imposto come massima
      esternal_max_temp = esternal_temperature.temperature;
    }
  }
  if (esternal_temperature.temperature < esternal_min_temp) {
    //è minore della minima
    //la imposto come minima
    esternal_min_temp = esternal_temperature.temperature;
  }

  //---------------------------------------------------------------------------------------------------
  //controllo le temperature della stufa
  //l'oggetto è stove_temperature
  if (stove_temperature.temperature > stove_max_temp) {
    //la temperatura è maggiore della massima
    //la imposto come massima
    stove_max_temp = stove_temperature.temperature;
  }
  if (stove_temperature.temperature < stove_min_temp) {
    //è minore della minima
    //la imposto come minima
    stove_min_temp = stove_temperature.temperature;
  }

  //adesso richiamo il provider per la scrittura dei valori
  //i parametri che gli passo sono le massime e le minime
  //il syntax_divider è il divisore dei comandi
  print_provider("-", home_max_temp, home_min_temp, esternal_max_temp, esternal_min_temp, stove_max_temp, stove_min_temp);
}

void saveAllTemperatures() {
  //salva tutte le temperature che trova
  //poi le confronta e ottiene le massime e le minime
  //e le scrive sul pannello
  //-------------------------------------------------------------------------
  //controllo temperature esterne dai sensori dallas
  //richiedo le temperature ai sensori
  ESTERNAL_SENSOR.requestTemperatures();
  STOVE_SENSOR.requestTemperatures();

  //le salvo nelle variabili
  esternal_temperature.temperature = ESTERNAL_SENSOR.getTempCByIndex(0);
  stove_temperature.temperature = STOVE_SENSOR.getTempCByIndex(0);
  //--------------------------------------------------------------------------
  //salvo la temperatura del dht22
  //salva la temperatura nei contenitori

  //variabile per gli errori
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(DHT22PIN, &DHT22TEMP.temperature, &DHT22TEMP.humidity, NULL)) != SimpleDHTErrSuccess) {
    //calcolo massima e minima
    calcMaxTemp(maxMinPrinter);
  }
}


#endif // TEMPERATURES_CPP
