#ifndef COSTANTS_H_INCLUDED
#define COSTANTS_H_INCLUDED

#include <Arduino.h>

// Oggetti derivati dalle mie classi
#define ESTERNAL_SENSOR_PIN 4               // Pin del sensore esterno
#define STOVE_SENSOR_PIN 5                  // Pin del sensore di temperatura della stufa
#define DHT22PIN 3                          // Pin del sensore DHT22
#define ON 1
#define OFF 0

const String VERSION = "1.3.0";

static String alarm_description = "Nessun allarme impostato";              // Descrizione allarme

String acc_time;                            // Orario di accensione convertito in stringa
String acc_time_minute;

String spegn_time;                          // Orario di spegnimento in stringa
String spegn_time_minute;

String bi_m_acc_time;                       // Orario di accensione del mattino convertito in stringa per il bigiornaliero
String bi_m_acc_time_minute;

String bi_m_spegn_time;                     // Orario di spegnimento del mattino convertito in stringa per il bigiornaliero
String bi_m_spegn_time_minute;

String bi_p_acc_time;                       // Orario di accensione del pomeriggio convertito in stringa per il bigiornaliero
String bi_p_acc_time_minute;

String bi_p_spegn_time;                     // Orario di spegnimento del pomeriggio convertito in stringa per il bigiornaliero
String bi_p_spegn_time_minute;

bool enable_minutes = true;
bool module_connected = true;               // Valore che indica se il modulo wifi si è connesso
bool check_cicle_daily = false;             // Indica se verrà controllato il ciclo giornaliero
bool check_cicle_bi_daily = false;          // Indica se verrà controllato il ciclo bi-giornaliero
bool interactive_mode = false;              // Attiva o disattiva la modalità interattiva della interfaccia seriale
bool daily_selected = false;                // Indica se è stato selezionato il modello giornaliero
bool bi_daily_selected = false;             // Indica se è stato selezionato il modello bi-giornaliero

float esternal_max_temp , esternal_min_temp;// Massima esterna e minima esterna
float stove_max_temp , stove_min_temp;      // Massima e minima stufa
float home_max_temp , home_min_temp;        // Massima e minima casa
int vent = 0;
int pwr = 0;
int stove = OFF;


// Registro dei comandi inviati dal modulo wifi
// I comandi sono formati dall' ID dell' oggetto grafico, (scelto da me) (vedere funzione setup) e lo stato. A seconda della tipologia di controllo lo stato può essere
// 1 o 0 per i controlli switch
// P per i controlli button
// testo per i controlli textbox
static const String  stoveSwitch_on  = "#W11";        // Comando seriale per il modulo WiFi che mette lo switch della stufa su on
static const String  stoveSwitch_off = "#W10";        // Comando seriale per il modulo WiFi che mette lo switch della stufa su off
static const String  buttonPower_press = "#BVP";
static const String  buttonVent_press  = "#BPP";
static const String  dailySwitch_on = "#W21";
static const String  dailySwitch_off = "#W20";
static const String  biDailySwitch_on = "#W31";
static const String  biDailySwitch_off = "#W30";
static const String  settingsButton_press = "#BIP";
static const String  mainButton_press = "#BMP";
static const String  online = "$ONLINE";
static const String  cloud_connecting_error = "$CLOUD-CONN-ERR";
static const String  ap_not_found_error = "$ERR-CONN-AP-NOT-FOUND";
static const String  connected = "$CONNECTED";
static const String  startCicle_on = "#WC1";
static const String  startCicle_off = "#WC0";
static const String  temperature_not_plausible = "errore: temperatura esterna non plusibile.";
// Icona mostrata sul display quando la stufa è accesa
const byte stove_on[] = {
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110
};
// Icona mostrata sul display quando la stufa è spenta
const byte stove_off[] = {
  //simbolo per indicare che la stufa è spenta
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};
// Icona mostrata sul display quando il modulo è connesso all' access point
const byte connected_ok[] = {
  //quando il modulo si è connesso mostro questa icona sulò display
  B01000,
  B11100,
  B01001,
  B00011,
  B00111,
  B01111,
  B11111
};
// Icona mostrata sul display quando il modulo non è connesso all' access point
const byte connected_no[] = {
  //quando il modulo non si è connesso(ha restituito errore) mostro questa icona sul display
  B00000,
  B00000,
  B00001,
  B00011,
  B00111,
  B01111,
  B11111
};

#endif // COSTANTS_H_INCLUDED
