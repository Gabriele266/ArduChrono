/*
* Autore: cavallo gabriele
* Versione:
* Descrizione:
* Progetto:
*
*/
// inclusioni
// Sorgente con le funzioni dei pulsanti
#include "../buttons/generic_funcs.cpp"
// Sorgente con le funzioni per l'interfaccia
#include "../interface/interface_fun.cpp"
// Sorgente con le funzioni per il seriale
#include "../serial/serial_funcs.cpp"
// Sorgente con le funzioni per la stufa
#include "../stove/stove_controls.cpp"
// Sorgente con le funzioni per le temperature
#include "../temperatures/temperatures_fun.cpp"
// Sorgente con le funzioni per il tempo
#include "../time/time_funcs.cpp"
// Sorgente con le funzioni per i cicli
#include "../time/cicle_funcs.cpp"

/// Iniziazillazione della scheda
static void core_setup(){
    // Setup del display
      lcd.init();
      lcd.noBacklight();
      lcd.createChar(0, stove_off);
      lcd.createChar(1, stove_on);
      lcd.createChar(2, connected_ok);
      lcd.createChar(3, connected_no);
      lcd.clear();

      // Velocita di comunicazione del modulo e del computer
      mywifi.computer_baudrate = 115200;
      mywifi.module_baudrate = 115200;

      // Setup del modulo
      mywifi.autoreport = true;
      mywifi.initComWithComputer();
      mywifi.initComWithModule();

      // Assegnamento pannelli per il modulo
      mywifi.main_panel =   "$P:D!00;*20/#F00T*9:Stufa Morinesio;/=/T*9:Stato stufa:;/L1G:0:;&&L2Y:0:;&&L3R:1:;/*9W1:0;T*7:T. Stufa: ;MA*7:0;//T*9:Regolazioni;{-r30!228,114*12{T*8:Pot: ;#0F0MB*8:0;&&#F00BV*8:+;}|{#F00T*8:Ven: ;#0F0MC*8:0;&&#F00BP*8:+;}}/=/T1*9:T. amb = ;#fffM1*9: 0;/#F00T2*9:U. amb = ;#fffM2*9: 0;/T*9:Massima/minima casa: ;&&M7*9:0.0;/=#F00T*9:T.esterna= ;#fffM3*9:0;/T*9:Massima/minima esterna: ;M8*9:0;=//#F00M5*9:0/0/0;/M9:0;/=/BI*9:Imposta azione;/B*10:Settaggi;";
      mywifi.second_panel=F("$P:D!00A-k;==//#F00T*20:Impostazione azione programmata;/=#fffT*15:Modello da applicare:;/==/#0f0T*15:Modello giornaliero;&&W2*10;/#fffT*13:Orario accensione (es 12 o 12.30):;#F00EA*10:;#fff/T*13:Orario spegnimento(es 12 o 12.30):;#F00ED*10:;/==/#0f0T*15:Modello bi-giornaliero:;&&W3*10;/#fffT*17:Mattino:;/#fffT*15:Orario accensione(es 12 o 12.30);#F00E0*10:;/#fffT*15:Orario spegnimento(es 12 o 12.30):;#F00E4*10:;//=#fffT*17:Pomeriggio:;/#fffT*15:Orario accensione(es 12 o 12.30);#F00E2*10:;/#fffT*15:Orario spegnimento(es 12 o 12.30);#F00E3*10:;/=/#fffT*15:Allarme impostato:;/#F00M9*15:Nessun allarme impostato;/=/#F00T*15:Avvia ciclo;WC*15;/#F00BM*15:Pagina principale;");
      // terzo pannello
      mywifi.splash_panel = F("$P:D!00;=#F00T*20:Impostazioni generali;//#fffT*15:Orario modulo;/#F00T*10:Ore;&&E5*11;/T*10:Minuti;&&E6*11;/T*10:Giorno;&&E7*11;/T*10:Mese;&&E8*11;/T*10:Anno;&&E9*11;//=T*15:Invia comando modulo.;EB*15;//#F00B2*15:Indietro;");
      // Prepara il modulo ad eseguire i comandi inviati
      mywifi.prepareModule();

      // Imposto i parametri per la scrittura sul display (coordinate)
      DHT22TEMP.lcdHumidityX = 0;
      DHT22TEMP.lcdHumidityY = 1;
      DHT22TEMP.lcdTempX = 0;
      DHT22TEMP.lcdTempY = 0;

      // Collegamento elementi intefaccia
      esternal_temperature.temp_panel_object = F("#M3");
      stove_temperature.temp_panel_object = F("#MA");
      DHT22TEMP.temp_panel_object = F("#M1");
      DHT22TEMP.hum_panel_object = F("#M2");
      mydata.panel_object = F("#M5");

      // Setup rtc
      rtc.begin();
      if (!rtc.begin()) {
        //l'rtc non è stato trovato
        Serial.println(F("RTC-ERR-NOT-FOUND"));
      }
      if (!rtc.isrunning()) {
        //imposto l'rtc sull' ora e la data giusta
        Serial.println(F("RTC-ERR-NOT-CALCULATING"));
      }

      // Avvio interfaccia utente
      startUserInterface();
      // Salvo tutte le temperature
      saveAllTemperatures();
      // Assegno alle minime i valori appena letti.
      esternal_min_temp = esternal_temperature.temperature;
      home_min_temp = DHT22TEMP.temperature;
      stove_min_temp = stove_temperature.temperature;
      // Imposto la velocità di risposta del modulo
      mywifi.sendCommand("$PING 400");
}

/// Aggiornamento della scheda
static void core_loop(){
      //controllo input dell' utente e dei sensori
  checkComputerCommands();
  // Controllo i messaggi in arrivo dal modulo seriale
  checkSerialCommands();
  // Controllo se bisogna applicare un ciclo
  checkCicle();
  // Controllo il
  checkButton();
  updateStatusLed();
  updateStatusLed("-onlylcd");
  //OUTPUT SU PANNELLO E DISPLAY
  //invia tutte le temperature al modulo
  saveAllTemperatures();
  saveTime();

  esternal_temperature.sendTo(mywifi);
  stove_temperature.sendTo(mywifi);
  mywifi.sendCommand(alarm_description);              //invio la descrizione dell' allarme
  DHT22TEMP.sendTo(mywifi);                           // ""    ""   ""
  mydata.sendTo(mywifi);
  mywifi.sendCommand(alarm_description);
  // Aggiornamento degli indicatori di potenza
  updatePowerIndicators();
}
