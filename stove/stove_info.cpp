#ifndef STOVE_INFO
#define STOVE_INFO

#include "../ArduChrono/costants.h"

/// Formatta la stringa di informazione allarme
void formatAlarmInfo(){
    // controllo il modello
    if(check_cicle_bi_daily){
        // il modello è quello bigiornaliero
        // svuoto la stringa
        alarm_description = "";
        // formatto
        alarm_description = String("#M9 Mattino: " + bi_m_acc_time + "." + bi_m_acc_time_minute +
                                    " - " + bi_m_spegn_time + "." + bi_m_spegn_time_minute +
                                    " pomeriggio: " +  bi_p_acc_time + "." + bi_p_acc_time_minute
                                    + " - " + bi_p_spegn_time + "." + bi_p_spegn_time_minute
                                    );
    }
    else if(check_cicle_daily){
        alarm_description = String("#M9 Accensione: " + acc_time + "." + acc_time_minute
                                   + " spegnimento: " + spegn_time + "." + spegn_time_minute);
    }
    else{
        alarm_description = "#M9 Nessun allarme impostato. Utilizzare la finestra imposta azione.";
    }
}

#endif // STOVE_INFO
