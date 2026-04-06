# Timer mit Uhr

Erweiterung des [Universaltimers](https://github.com/rabrefrei/Universaltimer) um eine Uhrzeitanzeige auf der LED-Großanzeige.

## Funktionen

- **Sport-Timer** – Intervalltraining mit konfigurierbaren Runden, Arbeits- und Pausenzeiten
- **Stoppuhr** – Zeitmessung bis auf Hundertstel-Sekunden
- **Countdown** – Rückwärtszähler von bis zu 10 Stunden
- **Spielstand** – Punkteanzeige für zwei Teams (0–99)
- **Uhrzeit & Datum** – Die LED-Großanzeige zeigt Uhrzeit (HH:MM) und Datum (TT.MM) im Wechsel, solange der Master im Hauptmenü steht

## Hardware

| Komponente | Gerät |
|---|---|
| Master | M5Stack Core2 |
| Slave | Seeed Studio XIAO ESP32C3 |
| Anzeige | 86× WS2812B RGB-LEDs (4 Ziffern + Doppelpunkt) |

## Kommunikation

Master und Slave kommunizieren direkt über **ESP-NOW** – kein Router, kein Internet erforderlich. Reichweite ca. 40 m (innen) / 100 m (außen).

## Projektstruktur

```
Timer_mit_Uhr/
├── Master/
│   ├── Master.ino          # M5Stack Core2 – Steuereinheit
│   └── TimerProtocol.h     # Gemeinsames Protokoll
├── Slave/
│   ├── Slave.ino           # XIAO ESP32C3 – LED-Großanzeige
│   └── TimerProtocol.h     # Gemeinsames Protokoll
├── TimerProtocol.h         # Referenzkopie
└── BEDIENUNGSANLEITUNG.md
```

## Abhängigkeiten

### Master
- [M5Unified](https://github.com/m5stack/M5Unified)
- ESP-NOW (Teil des ESP32 Arduino Core)

### Slave
- [FastLED](https://github.com/FastLED/FastLED)
- ESP-NOW (Teil des ESP32 Arduino Core)

## Flashen

```bash
# Master (M5Stack Core2)
arduino-cli compile --fqbn m5stack:esp32:m5stack_core2 Master
arduino-cli upload  --fqbn m5stack:esp32:m5stack_core2 --port <PORT> Master

# Slave (XIAO ESP32C3)
arduino-cli compile --fqbn esp32:esp32:XIAO_ESP32C3 Slave
arduino-cli upload  --fqbn esp32:esp32:XIAO_ESP32C3 --port <PORT> Slave
```

## Uhrzeit einstellen

Im Hauptmenü des Masters **lang drücken** → 2-stufiger Einstellungsdialog:
1. Uhrzeit (HH:MM)
2. Datum (TT.MM)

Die Zeit wird in der batteriepufferten Hardware-RTC des M5Stack gespeichert.

## Slave-Anzeige: Farbschema

| Zustand | Farbe |
|---|---|
| Uhrzeit / Datum (Hauptmenü) | Cyan |
| Einstellmodus | Rot, blinkend |
| Vorbereitung | Gelb |
| Arbeitsphase | Grün |
| Pausenphase | Rot |
| Pausiert | Blau |
| Fertig | Lila |
| Spielstand Team 1 / 2 | Rot / Grün |

## Lizenz

MIT
