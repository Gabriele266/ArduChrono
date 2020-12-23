#ifndef TIME_FUN
#define TIME_FUN

#include "../ArduChrono/objects.h"
#include "../ArduChrono/costants.h"
#include <RTClib.h>

void correct_times(){
  // Corregge tutti gli orari
  const String zero = "0";

  if (acc_time_minute == "00" || acc_time_minute == ""){
    acc_time_minute = zero;
  }

  if (spegn_time_minute == "00" || spegn_time_minute == ""){
    spegn_time_minute = zero;
  }

  if (bi_m_acc_time_minute == "00" || bi_m_acc_time_minute == ""){
    bi_m_acc_time_minute = zero;
  }

  if (bi_m_spegn_time_minute == "00" || bi_m_spegn_time_minute == ""){
    bi_m_spegn_time_minute = zero;
  }

  if (bi_p_acc_time_minute == "00" || bi_p_acc_time_minute == ""){
    bi_p_acc_time_minute = zero;
  }

  if (bi_p_spegn_time_minute == "00" || bi_p_spegn_time_minute == ""){
    bi_p_spegn_time_minute = zero;
  }
}

void saveTime() {

  //salva l'ora nelle variabili contenitore convertendola
  DateTime now = rtc.now();
  mydata.hour = String(now.hour());
  mydata.minute = String(now.minute());
  mydata.second = String(now.second());

  mydata.day = String(now.day());
  mydata.month = String(now.month());
  mydata.year = String(now.year());
}

int process_time(String value,char s_divider,String *result_hour,String *result_minute){
  // Processa una informazione di tempo del tipo      7.50
  int divider_point;
  bool found_point = false;
  String h;
  String m;

  for (int curr = 0;curr <= value.length();curr ++){
      // Questa azione viene eseguita per ogni carattere contenuto nella stringa
      if (value[curr] == s_divider){
        // Questo simbolo definisce la divisione tra le ore e i minuti
        divider_point = curr;           // Mi salvo la posizione del punto

        found_point = true;

        // Inizio a splittare
        h = value.substring(curr,0);
        m = value.substring(curr + 1);

        // Correggo i valori errati
        if (m.equals("00")){
          m = "0";
        }
        if (h.equals("00")){
          h = "0";
        }
        *result_hour = h;               // Tutto il pezzo da destra a sinistra compone l'ora
        *result_minute = m;             // Tutto il pezzo da sinistra a destra compone i minuti
        // Controllo che sia un orario corretto
        if (h.toInt() < 23 && m.toInt() < 59 && h.toInt() > 0 && m.toInt() > 0){
          // L'orario è in un formato corretto
          return 0;
        }
        else{
          // L'orario non è in un formato corretto
          return 1;
        }
        break;
      }
      else{
        // Prosegui
      }
    }
    // Controllo se è stato trovato un punto
    if (found_point){
      // è stato trovato un punto
      return 0;
    }
    else{
      return 2;
    }
}

/// Corregge le ore del modulo
bool correctHour(String newval){
    // equivalente in intero
    int val = atoi(newval.c_str());
    // controllo il valore
    if(val >= 0 && val <= 23){
        // va bene
        rtc.adjust(DateTime(
          atoi((mydata.year).c_str()),
          atoi((mydata.month).c_str()),
          atoi((mydata.day).c_str()),
          val,
          atoi((mydata.minute).c_str())));
        return true;
    }
    else{
        return false;
    }
}

/// Corregge i minuti
bool correctMinute(String newval){
    // equivalente in intero
    int val = atoi(newval.c_str());
    // controllo il valore
    if(val >= 0 && val <= 59){
        // va bene
        rtc.adjust(DateTime(
          atoi((mydata.year).c_str()),
          atoi((mydata.month).c_str()),
          atoi((mydata.day).c_str()),
          atoi((mydata.hour).c_str()),
          val));
        return true;
    }
    else{
        return false;
    }
}

/// Corregge il giorno
bool correctDay(String newval){
    // equivalente in intero
    int val = atoi(newval.c_str());
    // controllo il valore
    if(val >= 0 && val <= 31){
        // va bene
        rtc.adjust(DateTime(
          atoi((mydata.year).c_str()),
          atoi((mydata.month).c_str()),
          val,
          atoi((mydata.hour).c_str()),
          atoi((mydata.minute).c_str())));
        return true;
    }
    else{
        return false;
    }
}

/// Corregge il mese
bool correctMonth(String newval){
    // equivalente in intero
    int val = atoi(newval.c_str());
    // controllo il valore
    if(val >= 1 && val <= 12){
        // va bene
        rtc.adjust(DateTime(
          atoi((mydata.year).c_str()),
          val,
          atoi((mydata.day).c_str()),
          atoi((mydata.hour).c_str()),
          atoi((mydata.minute).c_str())));
        return true;
    }
    else{
        return false;
    }
}

/// Corregge l'anno
bool correctYear(String newval){
    // equivalente in intero
    int val = atoi(newval.c_str());
    // controllo il valore
    if(val >= 0){
        // va bene
        rtc.adjust(DateTime(
          val,
          atoi((mydata.month).c_str()),
          atoi((mydata.day).c_str()),
          atoi((mydata.hour).c_str()),
          atoi((mydata.minute).c_str())));
        return true;
    }
    else{
        return false;
    }
}

#endif // TIME_FUN
