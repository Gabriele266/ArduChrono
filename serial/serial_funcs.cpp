#ifndef SERIAL_FUNCS
#define SERIAL_FUNCS

#include "../ArduChrono/costants.h"
#include "../stove/stove_controls.cpp"
#include "../stove/stove_info.cpp"
#include "../ArduChrono/objects.h"
#include "../temperatures/temperatures_fun.cpp"
#include "../time/time_funcs.cpp"
#include "../interface/interface_fun.cpp"


/// Invia un insieme di 2 stringhe come nome: valore
void sendParameter(String name, String val){
    Serial.print("\n");
    Serial.print(name);
    Serial.print(":");
    Serial.println(val);
}

/// Controlla i comandi seriali
void checkSerialCommands(){
    //li valuta e esegue le operazioni relative
    //modalità selezionata dall' utente

  //modalita di ciclo e comando inviato
  String command;

  //leggo tutti i dati in entrata dal modulo
  mywifi.updateGivenCommands(&command);

  //controllo della stufa
  if (command.equals(stoveSwitch_on)) {
    // Accendo la stufa
    turn_stove_on();
  }
  else if (command.equals(stoveSwitch_off)) {
    // Spengo  la stufa
    STOVE.openCircuit();
    updateStatusLed();
    pwr = 0;
    vent = 0;
  }
  else if (command.equals(buttonPower_press)){
    // E' stato premuto il pulsante per incrementare la potenza della stufa
    PWR.closeCircuit();
    delay(200);
    PWR.openCircuit();
    delay(200);
    pwr ++;
    // Correggo i valori
    if(pwr == 6){
      pwr = 1;
    }
  }
  else if(command.equals("#B-1P")){
    mywifi.sendSplashPanel();
  }
  else if(command.equals("#B2P")){
    mywifi.sendMainPanel();
  }
  else if (command.equals(buttonVent_press)){
    // Aumento di 1 la ventola
    VENT.closeCircuit();
    delay(200);
    VENT.openCircuit();
    delay(200);
    vent ++;
    // Correggo i valori
    if (vent == 4){
      vent = 1;
    }
  }

  //selezione modalita di ciclo
  //###########################################################################
  else if (command.equals(dailySwitch_off)) {
    // E' stato deselezionato il ciclo bigiornaliero
    daily_selected = false;
  }
  else if (command.equals(dailySwitch_on)) {
    daily_selected = true;  //l' utente ha selezionato il modello giornaliero
    bi_daily_selected = false;
    mywifi.sendCommand(biDailySwitch_off);
  }
  else if (command.equals(biDailySwitch_on)) {
    // E' stato selezionato il modello bigiornaliero
    bi_daily_selected = true;
    daily_selected = false;
    mywifi.sendCommand(dailySwitch_off);
  }
  else if (command.equals(biDailySwitch_off)) {
    // E' stato deselezionato il modello bigiornaliero
    bi_daily_selected = false;
  }
  else if (command.equals(settingsButton_press)) {
    // L' utente vuole passare alla pagina impostazioni
    mywifi.sendSecondPanel();  //pagina impostazioni
    saveAllTemperatures();
    mywifi.sendCommand(alarm_description);

    //controllo le modalità e metto su on lo switch giusto
    if (check_cicle_daily){
      //l'utente aveva impostato il ciclo giornaliero
      mywifi.sendCommand(startCicle_on);
      mywifi.sendCommand(dailySwitch_on);
      mywifi.sendCommand(biDailySwitch_off);
    }
    else if (check_cicle_bi_daily){
    //imposto lo switch per il bidaily su on e quello per il daily su off
      mywifi.sendCommand(startCicle_off);
      mywifi.sendCommand(biDailySwitch_on);
      mywifi.sendCommand(dailySwitch_off);
    }
    else{
      // Errore le variabili sono nella posizione scorretta
      Serial.println(F("Error!!!! the cicle control vars aren't in the correct position!!!!!"));
      // invio un piccolo log
      sendParameter("Modello bi-giornaliero",String(check_cicle_bi_daily ));
      sendParameter("Modello giornaliero",String(check_cicle_daily ));
    }
  }
  else if (command.equals(mainButton_press)) {
    // Passa alla pagina principale
    mywifi.sendMainPanel();  //pagina principale
  }
  else if (command.equals(online)) {
    //qualcuno si è connesso al modulo
    module_connected = true;
  }
  else if (command.equals(cloud_connecting_error)){
    module_connected = false;
  }
  else if (command.equals(ap_not_found_error)){
    module_connected = false;
  }
  else if (command.equals(connected)){
    module_connected = true;
  }
  else if (command.substring(4, 0).equals("#EA:")) {
    //l'utente ha impostato l' orario di accensione della stufa per il ciclo daily
    process_time(command.substring(4),'.',&acc_time,&acc_time_minute);
    // invio il log
    sendParameter("Ora accensione", String(acc_time));
    sendParameter("Minuti accensione", String(acc_time_minute));
  }
  else if (command.substring(4, 0).equals("#ED:")) {
    //l 'utente ha impostato l'orario di spegnimento
    process_time(command.substring(4),'.',&spegn_time,&spegn_time_minute);
    // invio il log
    sendParameter("Ora spegnimento", String(spegn_time));
    sendParameter("Minuti spegnimento", String(spegn_time_minute));
  }
  else if (command.substring(4, 0).equals("#E0:")) {
    //orario accensione mattino
    process_time(command.substring(4),'.',&bi_m_acc_time,&bi_m_acc_time_minute);
    // invio il log
    sendParameter("Ora accensione mattino", String(bi_m_acc_time));
    sendParameter("Minuti accensione mattino", String(bi_m_acc_time_minute));
  }
  else if (command.substring(4, 0).equals("#E4:")) {
    //orario spegnimento mattino
    process_time(command.substring(4),'.',&bi_m_spegn_time,&bi_m_spegn_time_minute);
    sendParameter("Ora spegnimento mattino", String(bi_m_spegn_time));
    sendParameter("Minuti spegnimento mattino", String(bi_m_spegn_time_minute));
  }
  else if (command.substring(4, 0).equals("#E2:")) {
    //orario accensione pomeriggio
    process_time(command.substring(4),'.',&bi_p_acc_time,&bi_p_acc_time_minute);
    sendParameter("Ora accensione pomeriggio", String(bi_p_acc_time));
    sendParameter("Minuti accensione pomeriggio", String(bi_p_acc_time_minute));
  }
  else if (command.substring(4, 0).equals("#E3:")) {
    //orario spegnimento pomeriggio
    process_time(command.substring(4),'.',&bi_p_spegn_time,&bi_p_spegn_time_minute);
    sendParameter("Ora spegnimento pomeriggio", String(bi_p_spegn_time));
    sendParameter("Minuti spegnimento pomeriggio", String(bi_p_spegn_time_minute));
  }
  else if(command.substring(4,0).equals("#E5:")){
    // correggo le ore e invio la notifica
    sendCondNotify("Impostazione ore avvenuta con successo", "Errore: parametro ore non corretto.",correctHour(command.substring(4)));
  }
  else if(command.substring(4,0).equals("#E6:")){
    // correggo le ore e invio la notifica
    sendCondNotify("Impostazione minuti avvenuta con successo", "Errore: parametro minuti non corretto.",correctMinute(command.substring(4)));
  }
  else if(command.substring(4,0).equals("#E7:")){
    // correggo le ore e invio la notifica
    sendCondNotify("Impostazione giorno avvenuta con successo", "Errore: parametro giorno non corretto.",correctDay(command.substring(4)));
  }
  else if(command.substring(4,0).equals("#E8:")){
    // correggo le ore e invio la notifica
    sendCondNotify("Impostazione mese avvenuta con successo", "Errore: parametro mese non corretto.",correctMonth(command.substring(4)));
  }
  else if(command.substring(4,0).equals("#E9:")){
    // correggo le ore e invio la notifica
    sendCondNotify("Impostazione anno avvenuta con successo", "Errore: parametro anno non corretto.",correctYear(command.substring(4)));
  }
  else if (command.equals(startCicle_on)) {
    //avvio del ciclo
    //in base alla modalità selezionata lo avvio
    if (bi_daily_selected) {
      //modello da due parti per mattino e pomeriggio
      //imposto i controlli giusti
      //aggiorno l'utente
      check_cicle_bi_daily = true;
      check_cicle_daily = false;
      mywifi.sendPushNotify(("Controllo del ciclo bi-giornaliero avviato con: accM ore= " + bi_m_acc_time + " minuti: " + bi_m_acc_time_minute + " spegnM ore= " + bi_m_spegn_time + " minuti: " + bi_m_spegn_time_minute + "acP ore= " + bi_p_acc_time + " minuti:  " + bi_p_acc_time_minute +"speP ore= " + bi_p_spegn_time+" minuti: "+bi_p_spegn_time_minute));
      formatAlarmInfo();
    }
    else if (daily_selected) {
      //modello unico per tutto il giorno
      check_cicle_daily = true;
      check_cicle_bi_daily = false;
      //aggiorno l'utente con una notifica e il messaggio
      mywifi.sendPushNotify(("Controllo del ciclo giornaliero avviato con: a ore= " + acc_time +" minuti: "+acc_time_minute+ " spe ore = " + spegn_time + " minuti: " + spegn_time_minute));
      formatAlarmInfo();
    }
  }
  else if (command.equals(startCicle_off)) {
    //l'utente vuole fermare il ciclo
    check_cicle_daily = false;
    check_cicle_bi_daily = false;
    daily_selected = false;
    bi_daily_selected = false;
    //resetto l'interfaccia
    mywifi.sendCommand(F("#W20"));
    mywifi.sendCommand(F("#W30"));
    //fermo il controllo
    mywifi.sendPushNotify(F("Controllo del ciclo fermato"));
    Serial.println(F("CK-CLG-STOP"));
    formatAlarmInfo();
  }
}

/// Controlla i messaggi seriali del computer
void checkComputerCommands() {
  //controlla i comandi provenienti dall' utente e li interpreta
  int c;
  String Msg1;
  if (Serial.available() > 0){
    while ((c = Serial.read()) > '\n') Msg1 += (char) c;  // legge i caratteri in arrivo fino alla fine della linea
    if (c == '\n')                                       // quando è comapleto:
    {
      //avvia l'interpretazione dei comandi
      if (interactive_mode){
        //la modalità interattiva riscrive il comando inserito dall' utente in modo formattato per aiutare a correggere gli errori
        Serial.println(("Computer --> " + Msg1));
      }
      else if (!interactive_mode){
        //non lo riscrive
      }
      else if (Msg1.equals("COPYRIGHT")){
        //mostra il copyright del termostato
        Serial.println(F("          Copyright C 2018 di Gabriele Cavallo       "));
        Serial.println(F("  Il seguente prodotto e' soggetto da copyright, non lo si puo'"));
        Serial.println(F("copiare,modificare, ridistribuire o riutilizzare per fini non consoni al"));
        Serial.println(F("suo uso predefinito da proprietario.."));
        Serial.println(F("    Per maggiori informazioni contatare il 3317573441"));
      }
      else if (Msg1.equals("LCD-BACK-ON")){
        //accende la retroilluminazione del display lcd
        lcd.backlight();
      }
      else if (Msg1.equals("LCD-BACK-OFF")){
        //accende la retroilluminazione del display lcd
        lcd.noBacklight();
      }
      else if (Msg1.equals("INT-MODE-TRUE")){
        //passa alla modalità interattiva
        interactive_mode = true;
      }
      else if (Msg1.equals("INT-MODE-FALSE")){
        //passa alla modalità di default
        interactive_mode = false;
      }
      else if (Msg1.equals("DHT-TEMP-GET")){
        //restituisce la temperatura della casa scrivendola sul seriale
        Serial.println(DHT22TEMP.temperature);
      }
      else if (Msg1.equals("DHT-HUM-GET")){
        //restituisce l' umidità della casa
        Serial.println(DHT22TEMP.humidity);
      }
      else if (Msg1.equals("EST-TEMP-GET")){
        //restituisce la temperatura esterna
        Serial.println(esternal_temperature.temperature);
      }
      else if (Msg1.equals("STV-TEMP-GET")){
        //restituisce la temperatura della stufa
        Serial.println(stove_temperature.temperature);
      }
      else if (Msg1.equals("CLS")){
        //pulisce lo schermo
        for(int times = 0;times <= 20;times ++){
          //scrive un messaggio di ritorno a capo
          Serial.println("/n");
        }
      }
      else if (Msg1.equals("STV-TURN-ON")){
        //accende la stufa
        STOVE.closeCircuit();
        stove = ON;
        mywifi.sendCommand(F("#W11"));
      }
      else if (Msg1.equals("STV-TURN-OFF")){
        //spegne la stufa
        STOVE.openCircuit();
        stove = OFF;
        mywifi.sendCommand(F("#W10"));
      }
      else if (Msg1.equals("VENT-REL-ON")){
        // Chiude il circuito della ventola
        VENT.closeCircuit();
      }
      else if (Msg1.equals("VENT-REL-OFF")){
        // Chiude il circuito della ventola
        VENT.openCircuit();
      }
      else if (Msg1.equals("VENT-INCREMENT")){
        // Chiude il circuito della ventola
        VENT.closeCircuit();
        delay(200);
        VENT.openCircuit();
        vent ++;
      }
      else if (Msg1.equals("PWR-REL-ON")){
        // Chiude il circuito della ventola
        PWR.closeCircuit();
      }
      else if (Msg1.equals("PWR-REL-OFF")){
        // Chiude il circuito della ventola
        PWR.openCircuit();
      }
      else if (Msg1.equals("PWR-INCREMENT")){
        // Chiude il circuito della ventola
        PWR.closeCircuit();
        delay(200);
        PWR.openCircuit();
        pwr ++;
      }
      else if (Msg1.equals("PWR-GET")){
        Serial.println(pwr);
      }
      else if (Msg1.equals("VENT-GET")){
        Serial.println(vent);
      }
      else if (Msg1.equals("STV-STATE-GET")){
        //restituisce lo stato della stufa
        Serial.println(STOVE.getState());
      }
      else if (Msg1.equals("WM-CON-GET")){
        //restituisce lo stato di connessione del modulo
        Serial.println(module_connected);
      }
      else if (Msg1.equals("P-SER-LINE")){
        //restituisce lo stato della linea seriale del pannello

      }
      else if (Msg1.equals("MAIN-PANEL")){
        //mostra il pannello principale e lo scrive sul seriale
        mywifi.sendMainPanel();
        Serial.println((mywifi.main_panel));
      }
      else if (Msg1.equals("SECOND-PANEL")){
        //mostra il secondo pannello e lo scrive sul seriale
        mywifi.sendSecondPanel();
        Serial.println((mywifi.second_panel));
      }
      else if (Msg1.equals("SHOW-MAX")){
        //restituisce le massime in ordine di importanza
        Serial.print(F("ESTMAX: "));
        Serial.println(esternal_max_temp);
        Serial.print(F("HOMEMAX: "));
        Serial.println((home_max_temp));
        Serial.print(F("STVMAX: "));
        Serial.println((stove_max_temp));
      }
      else if (Msg1.equals("SHOW-MIN")){
        //restituisce le minime in ordine di importanza
        Serial.print(F("ESTMIN: "));
        Serial.println((esternal_min_temp));
        Serial.print(F("HOMEMIN: "));
        Serial.println((home_min_temp));
        Serial.print(F("STVMIN: "));
        Serial.println((stove_min_temp));
      }
      else if (Msg1.equals("REBOOT")){
        //effettua un reboot del termostato
        setup();
      }
      else if (Msg1.equals("HELP")){
        // Mostra una lista di comandi
        Serial.println(F("-------------------------------------------------------------------------------------------------------------------"));
        Serial.println(F("Lista comandi utilizzabili:"));
        Serial.println(F("                               "));
        Serial.println(F("Comando         Parametri     Sintassi         Effetto"));
        Serial.println(F("COPYRIGHT       nessuno       COPYRIGHT        Mostra il copyright a cui è soggetto il software"));
        Serial.println(F("VERSION         nessuno       VERSION          Mostra laversione corrente del software"));
        Serial.println(F("LCD-BACK-ON     nessuno       LCD-BACK-ON      Accende la retroilluminazione del display lcd"));
        Serial.println(F("LCD-BACK-OFF    nessuno       LCD-BACK-OFF     Spegne la retroilluminazione del display lcd"));
        Serial.println(F("INT-MODE-TRUE   nessuno       INT-MODE-TRUE    Passa alla modalità interattiva del seriale"));
        Serial.println(F("INT-MODE-FALSE  nessuno       INT-MODE-FALSE   Passa alla modalità di default del termostato"));
        Serial.println(F("HELP            nessuno       HELP             Visualizza una lista di comandi"));
        Serial.println(F("DHT-TEMP-GET    nessuno       DHT-TEMP-GET     Mostra la temperatura del dht"));
        Serial.println(F("DHT-HUM-GET     nessuno       DHT-HUM-GET      Mostra l'umidità del dht"));
        Serial.println(F("EST-TEMP-GET    nessuno       EST-TEMP-GET     Mostra la temperatura esterna"));
        Serial.println(F("STV-TEMP-GET    nessuno       STV-TEMP-GET     Mostra la temperatura della stufa"));
        Serial.println(F("CLS             nessuno       CLS              Pulisce la schermata"));
        Serial.println(F("STV-TURN-ON     nessuno       STV-TURN-ON      Accende la stufa"));
        Serial.println(F("STV-TURN-OFF    nessuno       STV-TURN-OFF     Spegne la stufa"));
        Serial.println(F("STV-STATE-GET   nessuno       STV-STATE-GET    Ottiene lo stato della stufa"));
        Serial.println(F("WM-CON-GET      nessuno       WM-CON-GET       Restituisce lo stato della connessione"));
        Serial.println(F("P-SER-LINE      nessuno       P-SER-LINE       Restituisce lo stato del pannello seriale"));
        Serial.println(F("MAIN-PANEL      nessuno       MAIN-PANEL       Visualizza sul pannello e sul seriale il pannello principale"));
        Serial.println(F("SECOND-PANEL    nessuno       SECOND-PANEL     Visualizza sul pannello e sul seriale il pannello secondario"));
        Serial.println(F("SHOW-MAX        nessuno       SHOW-MAX         Visualizza la massima di tutte le temperature"));
        Serial.println(F("SHOW-MIN        nessuno       SHOW-MIN         Visualizza la minima di tutte le temperature"));
        Serial.println(F("REBOOT          nessuno       REBOOT           Riavvia il termostato"));
        Serial.println(F("VENT-REL-ON     nessuno       VENT-REL-ON      Mette il relay della ventola su on"));
        Serial.println(F("VENT-REL-OFF    nessuno       VENT-REL-ON      Mette il relay della ventola su OFF"));
        Serial.println(F("VENT-INCREMENT  nessuno       VENT-REL-ON      Incrementa di 1 la ventola della stufa"));
        Serial.println(F("PWR-REL-ON      nessuno       VENT-REL-ON      Mette il relay della potenza su on"));
        Serial.println(F("PWR-REL-OFF     nessuno       VENT-REL-ON      Mette il relay della potenza su OFF"));
        Serial.println(F("PWR-INCREMENT   nessuno       VENT-REL-ON      Incrementa di 1 la potenza della stufa"));
        Serial.println(F("SET-HOUR        ore (int)     SET-HOUR-5       Imposta l'ora al valore indicato dal parametro ore"));
        Serial.println(F("SET-MINUTE      minuti(int)   SET-MINUTE-5     Imposta i minuti al valore indicato dal parametro minuti"));
        Serial.println(F("SET-DAY         day (int)     SET-DAY-5        Imposta il giorno al valore dato"));
        Serial.println(F("SET-MONTH       month(int)    SET-MONTH-5      Imposta i mesi al valore indicato dal parametro minuti"));
        Serial.println(F("SHOW-TIME       nessuno       SHOW_TIME        Mostra l'ora e la data locale"));
        Serial.println(F("GET-TIME-FROM-CLOUD           /////////        Richiede la data e l'ora alweb"));
        Serial.println(F("------------------------------------------------------------------------------------------------------------------------"));
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      }
      else if(Msg1.equals("VERSION")){
        // Mostra la versione
        Serial.println("Versione N: " + VERSION);
      }
      else if(Msg1.equals("SHOW-TIME")){
        // Mostro la data e l'ora
        Serial.println(F("Ora e data locale: "));
        Serial.print(F("ORE: "));
        Serial.println(mydata.hour);
        Serial.print(F("MINUTI: "));
        Serial.println(mydata.minute);

        Serial.print(F("GIORNO: "));
        Serial.println(mydata.day);

        Serial.print(F("MESE: "));
        Serial.println(mydata.month);

        Serial.print(F("ANNO: "));
        Serial.println(mydata.year);
      }
      else if((Msg1.substring(0,9)).equals("SET-HOUR-")){
        // Salvo il resto della stringa
        String param = Msg1.substring(9,Msg1.length());
        // Converto la stringa in un numero
        int newhour = atoi(param.c_str());
        Serial.println("Salvataggio nuova ora: " + param);
        // Imposto la nuova ora
        rtc.adjust(DateTime(
          atoi((mydata.year).c_str()),
          atoi((mydata.month).c_str()),
          atoi((mydata.day).c_str()),
          newhour,
          atoi((mydata.minute).c_str())));
      }
      else if((Msg1.substring(0,11)).equals("SET-MINUTE-")){
        // Salvo il resto della stringa
        String param = Msg1.substring(11,Msg1.length());
        // Converto la stringa in un numero
        int newminute = atoi(param.c_str());
        Serial.println("Salvataggio nuovi minuti: " + param);
        // Imposto la nuova ora
        rtc.adjust(DateTime(
          atoi((mydata.year).c_str()),
          atoi((mydata.month).c_str()),
          atoi((mydata.day).c_str()),
          atoi((mydata.hour).c_str()),
          newminute));
      }
      else if((Msg1.substring(0,8)).equals("SET-DAY-")){
        // Salvo il resto della stringa
        String param = Msg1.substring(8,Msg1.length());
        // Converto la stringa in un numero
        int newday = atoi(param.c_str());
        Serial.println("Salvataggio nuovo giorno: " + param);
        // Imposto la nuova ora
        rtc.adjust(DateTime(
          atoi((mydata.year).c_str()),
          atoi((mydata.month).c_str()),
          newday,
          atoi((mydata.hour).c_str()),
          atoi((mydata.minute).c_str())));
      }
      else if(Msg1.substring(0,10).equals("SET-MONTH-")){
        // Salvo il resto della stringa
        String param = Msg1.substring(10,Msg1.length());
        // Converto la stringa in un numero
        int newmonth = atoi(param.c_str());
        Serial.println("Salvataggio nuovo mese: " + param);
        // Imposto la nuova ora
        rtc.adjust(DateTime(
          atoi((mydata.year).c_str()),
          newmonth,
          atoi((mydata.day).c_str()),
          atoi((mydata.hour).c_str()),
          atoi((mydata.minute).c_str())));
      }
      else if((Msg1.substring(0,9)).equals("SET-YEAR-")){
        // Salvo il resto della stringa
        String param = Msg1.substring(9,Msg1.length());
        // Converto la stringa in un numero
        int newyear = atoi(param.c_str());
        Serial.println("Salvataggio nuovo anno: " + param);
        // Imposto la nuova ora
        rtc.adjust(DateTime(
          newyear,
          atoi((mydata.month).c_str()),
          atoi((mydata.day).c_str()),
          atoi((mydata.hour).c_str()),
          atoi((mydata.minute).c_str())));
      }
      //svuoto la variabile per il messaggio
      Msg1 = "";
    }
    else{
      // Il comando non è stato riconosciuto
      if (interactive_mode){
        Serial.print(F("Comando "));
        Serial.print(Msg1);
        Serial.println(F(" non riconosciuto. Utilizzare HELP per maggiori informazioni"));
      }
      if (!interactive_mode){
        // Non dico niente all' utente
      }
    }
  }
}
#endif // SERIAL_FUNCS
