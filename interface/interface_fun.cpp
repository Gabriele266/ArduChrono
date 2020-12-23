#ifndef INTERFACE_FUN
#define INTERFACE_FUN

#include "../ArduChrono/objects.h"
#include "../ArduChrono/costants.h"
#include <Arduino.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void startUserInterface() {
  //avvia l'interfaccia utente-computer
  lcd.backlight();
  lcd.setCursor(0, 0);

  //pulizia e invio del pannello
  mywifi.discardOldMessages();
  mywifi.sendMainPanel();

  //questa stringa avverte l'utente che il termostato si sta avviando
  Serial.println(F("STING-TERM"));

  //scritte di avvio sul display
  lcd.print(F("Avvio termostato"));
  lcd.setCursor(0, 1);
  lcd.print(F("by Gabri C."));
  delay(3000);
  lcd.clear();

  //spengo la backlight per risparmiare energia e anche per non produrre la luce di notte
  //la backlight è riattivabile tramite l'apposito pulsante nero o dal seriale con i comandi
  lcd.noBacklight();

  mywifi.sendCommand(F("#WC0"));
}

void updateStatusLed(String arg = "-base"){
  //questa funzione aggiorna i led della interfaccia in base alla temperatura
  //attraverso la modifica del parametro si ottengono effetti diversi
  //la temperatura della stufa è STOVE_TEMPERATURE.temperature
  //i led di stato sono
  //L1 = verde  L2 = giallo L2 = rosso
  //aggiorna l'icona dello stato di connessione
  if (module_connected){
    //qunado il modulo si connette
    lcd.setCursor(12,1);
    lcd.write(byte(2));
  }
  if (!module_connected){
    //quando non è connesso
    lcd.setCursor(12,1);
    lcd.write(byte(3));
  }

  lcd.setCursor(2, 0);                                //scrivo il tutto sul display lcd
  lcd.print(((String(DHT22TEMP.temperature) + "C")));   //con un po' di formattazione
  lcd.setCursor(9, 0);
  lcd.print(((String(DHT22TEMP.humidity) + "%")));
  lcd.setCursor(5, 1);
  lcd.print(((mydata.hour + ":" + mydata.minute)));
  lcd.setCursor(15,1);
  lcd.print((String(STOVE.getState())));
  if (arg.equals("-base")){
    //modalità base
    //legge lo stato del relay
    //in base alla temperatura della stufa decide se è accesa, in accensione o spenta
    if (STOVE.getState() == 0){
      //quando la stufa è su 0 la imposto come spenta(rosso)
      mywifi.sendCommand(F("#W10"));
      mywifi.sendCommand(F("#L20"));
      //disattivo tutti gli altri
      mywifi.sendCommand(F("#L10"));
      mywifi.sendCommand(F("#L31"));
    }
    if (STOVE.getState() == 1){
      //la stufa si sta accendendo
      mywifi.sendCommand(F("#L31"));
      mywifi.sendCommand(F("#W11"));
      if (stove_temperature.temperature > DHT22TEMP.temperature && stove_temperature.temperature < 30.0){
        //la stufa è in accensione quindi imposto il led giallo su on, il rosso su off e il verde su off
        mywifi.sendCommand(F("#L10"));
        mywifi.sendCommand(F("#L21"));
        mywifi.sendCommand(F("#L30"));
      }
      if (stove_temperature.temperature > 30.00){
        //la stufa è accesa!!!!
        mywifi.sendCommand(F("#L11"));
        mywifi.sendCommand(F("#L20"));
        mywifi.sendCommand(F("#L30"));
      }
      if (stove_temperature.temperature == 127.00){
        // la temperatura è molto sbgaliata
        // Non la interpreto e lascio la situazione uguale a quella di prima
      }
    }
  }
  if (arg.equals("-onlystat")){
    //aggiorna i led senza l'uso della temperatura
    //utilizza solo il led rosso e quello verde
    if (STOVE.getState() == 1){
      //la stufa è accesa
      mywifi.sendCommand(F("#W11"));
      mywifi.sendCommand(F("#L11"));
      mywifi.sendCommand(F("#L20"));
      mywifi.sendCommand(F("#L30"));
    }
    if (STOVE.getState() == 0){
      //la stufa è spenta
      mywifi.sendCommand(F("#W10"));
      mywifi.sendCommand(F("#L10"));
      mywifi.sendCommand(F("#L20"));
      mywifi.sendCommand(F("#L31"));
    }
  }
  if (arg.equals("-onlylcd")){
    //aggiorna lo stato della stufa mostrato sul display lcd scegliendo tra le due icone create in precedenza
    if (STOVE.getState() == 1){
      //la stufa è spenta
      //aggiorno l'icona (spento = carattere 0 acceso = carattere 1)
      lcd.setCursor(15,1);
      lcd.write(byte(1));
    }
    if (STOVE.getState() == 0){
      //la stufa è accesa
      //aggiorno l'cona
      lcd.setCursor(15,1);
      lcd.write(byte(0));
    }
   }
}

void updatePowerIndicators(){
  // Aggiorna gli indicatori di potenza della stufa
  if (pwr == 6){
    // La potenza non è in una scala giusta
    pwr = 1;
  }
  else {
    mywifi.sendCommand(("#MB" + String(pwr)));
  }
  // Aggiornamento degli indicatori di ventola
  if (vent == 4){
    // La ventola non è in un range giusto
   vent = 1;
  }
  else {
    // Ottengo il valore vero
    mywifi.sendCommand(("#MC" + String(vent)));
  }
}

/// Invia una notifica condizionale all' utente
void sendCondNotify(String trueText, String falseText, bool cond){
    if(cond){
        // invio truetext
        mywifi.sendPushNotify(trueText);
    }
    else{
        mywifi.sendPushNotify(falseText);
    }
}

#endif // INTERFACE_FUN
