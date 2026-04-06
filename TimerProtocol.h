/**
 * @file TimerProtocol.h
 * @brief Gemeinsame Protokoll-Definitionen für Master und Slave
 *        (Timer_mit_Uhr – erweitert um Echtzeit-Übertragung)
 *
 * WICHTIG: Diese Datei muss in Master/ und Slave/ identisch sein!
 *
 * Änderung gegenüber Universaltimer:
 *   TimerData enthält jetzt zusätzlich hour und minute.
 *   Der Slave zeigt die Uhrzeit (HH:MM) an, wenn der Master nicht sendet.
 */
#pragma once
#include <stdint.h>

// --- Protokoll-States (Feld 'state' in TimerData) ---
constexpr uint8_t STATE_SETUP    = 0;  // Einstellung (vor dem Start)
constexpr uint8_t STATE_PREPARE  = 1;  // Vorbereitungs-Countdown
constexpr uint8_t STATE_WORK     = 2;  // Arbeitsphase / Stoppuhr läuft
constexpr uint8_t STATE_REST     = 3;  // Pausenphase
constexpr uint8_t STATE_FINISHED = 4;  // Timer abgelaufen
constexpr uint8_t STATE_PAUSED   = 5;  // Pausiert
constexpr uint8_t STATE_SCORE    = 6;  // Spielstand-Modus
constexpr uint8_t STATE_MENU     = 7;  // Master im Hauptmenü → Slave zeigt Uhrzeit

// --- Setup-Submodes (codiert im 'round'-Feld wenn state == STATE_SETUP) ---
constexpr uint8_t SETUP_SUBMODE_ROUNDS = 0;
constexpr uint8_t SETUP_SUBMODE_WORK   = 1;
constexpr uint8_t SETUP_SUBMODE_PAUSE  = 2;

// --- Gemeinsame Datenstruktur ---
// Neu: hour (0-23) und minute (0-59) werden vom Master mitgesendet.
// Der Slave nutzt diese Werte zur Uhrzeitanzeige wenn kein Master-Signal vorhanden.
struct __attribute__((packed)) TimerData {
    uint8_t  state;    // STATE_*-Konstante
    uint16_t seconds;  // Zeitwert, Score (Format XXYY) oder Rundenanzahl
    uint8_t  round;    // Laufend: aktuelle Runde. STATE_SETUP: SETUP_SUBMODE_*
    uint8_t  hour;     // Aktuelle Stunde (0-23) laut Master-RTC
    uint8_t  minute;   // Aktuelle Minute (0-59) laut Master-RTC
    uint8_t  day;      // Aktueller Tag (1-31) laut Master-RTC
    uint8_t  month;    // Aktueller Monat (1-12) laut Master-RTC
};
