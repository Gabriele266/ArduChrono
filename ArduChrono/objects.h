#ifndef OBJECTS_H_INCLUDED
#define OBJECTS_H_INCLUDED


// Librerie della standard library
#include <Wire.h>                       // Comunicazione 12_c
#include <LiquidCrystal_I2C.h>          // Controllo dello schermo I2_C

// Librerie addizionali
#include <SimpleDHT.h>                  // Controllo del sensore DHT22
#include <WIFIMODULE.h>                 // Controllo del modulo wifi
#include <TEMPERATURE.h>                // Controllo della temperatura
#include <RELAY.h>
#include <DATA.h>                       // Controllo della data
#include <BUTTON.h>                     // Controllo dei pulsanti
#include <DallasTemperature.h>          // Controllo dei sensori dallas
#include <RTClib.h>                     // Controllo del rtc
#include <OneWire.h>                    // Controllo a onewire
#include "costants.h"


DATA mydata;                                                  // Oggetto data che contiene la data e l'ora corrente
static WIFIMODULE mywifi;                                     // Rappresenta il modulo wifi
TEMPERATURE esternal_temperature;                             // Oggetto temperature che contiene la temperatura esterna
TEMPERATURE stove_temperature;                                // Oggetto temperature che contiene la temperatura stufa
TEMPERATURE DHT22TEMP;                                        // Oggetto temperature che contiene la temperatura interna
RELAY STOVE(2);                                               // Oggetto relay che controlla la stufa
RELAY VENT(23);                                               // Oggetto relay che controlla la ventola
RELAY PWR(25);                                                // Oggetto relay che controlla la potnza

static BUTTON BACK_SWITCH(6);                                 // Oggetto button che rappresenta il pulsante per la retro illuminazione
static LiquidCrystal_I2C lcd(0x27, 16, 2);                    // Oggetto che controlla lo schermo
RTC_DS1307 rtc;                                               // Oggetto che controlla l'rtc
SimpleDHT22 dht22;                                            // Oggetto che controlla il DHT22
OneWire ESTERNAL_SENSOR_WIRE(ESTERNAL_SENSOR_PIN);            // Oggetto che controlla il cavo esterno
OneWire STOVE_SENSOR_WIRE(STOVE_SENSOR_PIN);                  // Oggetto che controlla il cavo esterno
DallasTemperature ESTERNAL_SENSOR(&ESTERNAL_SENSOR_WIRE);     // Sensore esterno
DallasTemperature STOVE_SENSOR(&STOVE_SENSOR_WIRE);           // Sensore interno




#endif // OBJECTS_H_INCLUDED
