/**
 * Projekt: Timer_mit_Uhr – Slave (XIAO ESP32C3)
 * Hardware: Seeed Studio XIAO ESP32C3
 * Display:  86-LED Großanzeige (4 Ziffern + Doppelpunkt)
 *
 * Uhrzeit (HH:MM, Cyan) wird nur angezeigt wenn der Master STATE_MENU sendet,
 * also im Hauptmenü steht. In allen anderen States normale Timer-Anzeige.
 */

#include <WiFi.h>
#include <esp_now.h>
#include <FastLED.h>
#include "TimerProtocol.h"

// --- Hardware-Konfiguration XIAO ESP32C3 ---
constexpr uint8_t  LED_PIN        = 2;
constexpr uint16_t NUM_LEDS       = 86;
constexpr uint8_t  BRIGHTNESS     = 255;
constexpr uint8_t  LEDS_PER_DIGIT = 21;

CRGB leds[NUM_LEDS];

// Mapping: Bit 0=b, 1=a, 2=f, 3=g, 4=c, 5=d, 6=e  (3 LEDs pro Segment)
const uint8_t DIGIT_MAP[] = {
    0b01110111, 0b00010001, 0b01101011, 0b00111011, 0b00011101,
    0b00111110, 0b01111110, 0b00010011, 0b01111111, 0b00111111
};

// --- Empfangspuffer ---
TimerData incomingBuffer;
volatile bool newData = false;

// --- Uhrzeit-Tracking (Minute läuft per millis() weiter) ---
bool     clockSynced  = false;
uint8_t  clockHour    = 0;
uint8_t  clockMinute  = 0;
uint32_t clockRefMs   = 0;

// --- Datum ---
bool    dateKnown  = false;
uint8_t clockDay   = 1;
uint8_t clockMonth = 1;

// ============================================================
// Hilfsfunktionen für das LED-Display
// ============================================================

void displayDigit(uint8_t pos, int8_t value, CRGB color) {
    if (pos > 3 || value < 0) return;
    uint16_t startIdx = pos * LEDS_PER_DIGIT;
    if (pos >= 2) startIdx += 2;
    uint8_t mask = DIGIT_MAP[value % 10];
    for (uint8_t seg = 0; seg < 7; seg++) {
        if ((mask >> seg) & 0x01) {
            for (uint8_t l = 0; l < 3; l++) {
                uint16_t idx = startIdx + seg * 3 + l;
                if (idx < NUM_LEDS) leds[idx] = color;
            }
        }
    }
}

void setColon(bool on, CRGB color) {
    leds[42] = on ? color : CRGB::Black;
    leds[43] = on ? color : CRGB::Black;
}

// ============================================================
// Uhrzeitanzeige HH:MM in Cyan
// ============================================================

void renderClock() {
    uint32_t elapsed      = millis() - clockRefMs;
    uint32_t totalMinutes = (uint32_t)clockHour * 60 + clockMinute + (elapsed / 60000UL);
    uint8_t  h = (totalMinutes / 60) % 24;
    uint8_t  m = totalMinutes % 60;

    FastLED.clear();
    CRGB c = CRGB(0, 180, 200);
    displayDigit(3, h / 10, c);
    displayDigit(2, h % 10, c);
    displayDigit(1, m / 10, c);
    displayDigit(0, m % 10, c);
    setColon((millis() / 1000) % 2 == 0, c);
    FastLED.show();
}

// ============================================================
// Datumsanzeige TT.MM (Punkt immer an, kein Blinken)
// ============================================================

void renderDate() {
    FastLED.clear();
    CRGB c = CRGB(0, 180, 200);

    displayDigit(3, clockDay   / 10, c);
    displayDigit(2, clockDay   % 10, c);
    displayDigit(1, clockMonth / 10, c);
    displayDigit(0, clockMonth % 10, c);
    setColon(true, c); // Punkt als Trenner, nicht blinkend
    FastLED.show();
}

// ============================================================
// Timer-Anzeige
// ============================================================

void renderDisplay(const TimerData& d) {
    FastLED.clear();

    if (d.state == STATE_SCORE) {
        uint8_t r = d.seconds / 100;
        uint8_t g = d.seconds % 100;
        displayDigit(3, r / 10, CRGB::Red);
        displayDigit(2, r % 10, CRGB::Red);
        displayDigit(1, g / 10, CRGB::Green);
        displayDigit(0, g % 10, CRGB::Green);
        setColon(true, CRGB::White);
    } else {
        CRGB color = CRGB::White;
        switch (d.state) {
            case STATE_SETUP:    color = CRGB::Red;    break;
            case STATE_PREPARE:  color = CRGB::Yellow; break;
            case STATE_WORK:     color = CRGB::Green;  break;
            case STATE_REST:     color = CRGB::Red;    break;
            case STATE_FINISHED: color = CRGB::Purple; break;
            case STATE_PAUSED:   color = CRGB::Blue;   break;
        }
        if (d.state == STATE_SETUP) {
            if ((millis() / 500) % 2 == 0) {
                displayDigit(3, 0, color); displayDigit(2, 0, color);
                displayDigit(1, 0, color); displayDigit(0, 0, color);
                setColon(true, color);
            }
        } else {
            uint8_t mm = d.seconds / 60;
            uint8_t ss = d.seconds % 60;
            displayDigit(3, mm / 10, color);
            displayDigit(2, mm % 10, color);
            displayDigit(1, ss / 10, color);
            displayDigit(0, ss % 10, color);
            bool blink = (d.state == STATE_PAUSED) || (d.seconds % 2 == 0);
            setColon(blink, color);
        }
    }
    FastLED.show();
}

// ============================================================
// ESP-NOW Callback
// ============================================================

void onDataRecv(const esp_now_recv_info* recv_info, const uint8_t* data, int len) {
    if (len == sizeof(TimerData)) {
        memcpy(&incomingBuffer, data, sizeof(TimerData));
        newData = true;
        // Uhrzeit-Referenz immer mitführen
        if (incomingBuffer.hour < 24 && incomingBuffer.minute < 60) {
            clockHour   = incomingBuffer.hour;
            clockMinute = incomingBuffer.minute;
            clockRefMs  = millis();
            clockSynced = true;
        }
        if (incomingBuffer.day >= 1 && incomingBuffer.day <= 31 &&
            incomingBuffer.month >= 1 && incomingBuffer.month <= 12) {
            clockDay   = incomingBuffer.day;
            clockMonth = incomingBuffer.month;
            dateKnown  = true;
        }
    }
}

// ============================================================
// Setup & Loop
// ============================================================

void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear(true);

    WiFi.mode(WIFI_STA);
    WiFi.setTxPower(WIFI_POWER_19_5dBm);

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Init fehlgeschlagen");
        return;
    }
    esp_now_register_recv_cb(onDataRecv);
    Serial.println("Slave bereit.");
}

void loop() {
    static uint32_t lastRender = 0;

    // Hilfsfunktion: Uhr oder Datum je nach 5-Sekunden-Takt zeichnen
    auto renderMenuDisplay = [&]() {
        static uint32_t toggleMs  = 0;
        static bool     showDate  = false;

        if (millis() - toggleMs >= 5000) {
            // Nur umschalten wenn Datum bekannt, sonst immer Uhrzeit
            if (dateKnown) showDate = !showDate;
            toggleMs = millis();
        }
        if (showDate && dateKnown) renderDate();
        else                       renderClock();
    };

    if (!newData) {
        if (incomingBuffer.state == STATE_MENU && clockSynced) {
            if (millis() - lastRender >= 500) {
                renderMenuDisplay();
                lastRender = millis();
            }
        }
        if (incomingBuffer.state == STATE_SETUP) {
            if (millis() - lastRender >= 100) {
                renderDisplay(incomingBuffer);
                lastRender = millis();
            }
        }
        return;
    }

    newData = false;

    if (incomingBuffer.state == STATE_MENU) {
        if (clockSynced) renderMenuDisplay();
        else             FastLED.clear(true);
    } else {
        renderDisplay(incomingBuffer);
    }
    lastRender = millis();
}
