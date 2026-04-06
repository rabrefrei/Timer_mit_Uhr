# Bedienungsanleitung – Timer mit Uhr

## Inhaltsverzeichnis

1. [Systemübersicht](#systemübersicht)
2. [Erstinbetriebnahme](#erstinbetriebnahme)
3. [Hauptmenü](#hauptmenü)
4. [Modus 1: Sport-Timer](#modus-1-sport-timer)
5. [Modus 2: Stoppuhr](#modus-2-stoppuhr)
6. [Modus 3: Countdown](#modus-3-countdown)
7. [Modus 4: Spielstand](#modus-4-spielstand)
8. [Slave-Display (LED-Großanzeige)](#slave-display-led-großanzeige)
9. [Uhrzeit und Datum einstellen](#uhrzeit-und-datum-einstellen)
10. [Energieverwaltung](#energieverwaltung)
11. [Technische Daten](#technische-daten)

---

## Systemübersicht

Der Timer mit Uhr besteht aus zwei Geräten, die drahtlos miteinander kommunizieren:

| Komponente | Gerät | Funktion |
|---|---|---|
| **Master** | M5Stack Core2 | Touchscreen-Steuereinheit |
| **Slave** | XIAO ESP32C3 + LEDs | LED-Großanzeige (86 LEDs) |

Die beiden Geräte verbinden sich automatisch über **ESP-NOW** – eine direkte WLAN-Verbindung ohne Router, mit einer Reichweite von ca. 40 m (in Gebäuden) bis 100 m (Freigelände). Es ist keine App oder Internetverbindung erforderlich.

**Neu gegenüber dem Universaltimer:** Die LED-Großanzeige zeigt die aktuelle Uhrzeit und das Datum an, wenn sich der Master im Hauptmenü befindet.

---

## Erstinbetriebnahme

1. **Großanzeige (Slave) einschalten**: Netzteil anschließen. Die LED-Anzeige bleibt zunächst dunkel, bis der Master eine Uhrzeit übermittelt hat.
2. **Master einschalten**: Das Hauptmenü erscheint automatisch. Die Großanzeige zeigt sofort die Uhrzeit an.
3. **Verbindungsaufbau**: Der Slave erkennt den Master automatisch (keine Konfiguration nötig).
4. **Uhrzeit einstellen**: Beim ersten Einsatz die Uhrzeit und das Datum am Master einmalig einstellen (siehe [Uhrzeit und Datum einstellen](#uhrzeit-und-datum-einstellen)).

> **Hinweis**: Slave und Master müssen einmalig mit der korrekten MAC-Adresse konfiguriert sein (Firmware-seitig hinterlegt). Bei Austausch eines Gerätes muss die Firmware entsprechend angepasst werden.

---

## Hauptmenü

Nach dem Einschalten erscheint das Hauptmenü mit vier Kacheln:

```
┌──────────────┬──────────────┐
│   SPORT      │  STOPPUHR    │
│  (blau)      │   (grün)     │
├──────────────┼──────────────┤
│  COUNTDOWN   │  SPIELSTAND  │
│  (orange)    │   (weiß)     │
└──────────────┴──────────────┘
     LANG DRUECKEN: UHR STELLEN
```

- Tippe auf eine Kachel, um den entsprechenden Modus zu öffnen.
- **Lang drücken** (2 Sek.) auf eine beliebige Stelle öffnet den Dialog zur Uhrzeit- und Datumseinstellung.
- Solange der Master im Hauptmenü steht, zeigt die **LED-Großanzeige die aktuelle Uhrzeit und das Datum** im Wechsel (alle 5 Sekunden).

---

## Modus 1: Sport-Timer

Der Sport-Timer ist für **Intervall-Training** (z. B. HIIT, CrossFit, Tabata) ausgelegt. Er zählt automatisch Arbeits- und Pausenphasen über mehrere Runden.

### Einrichten

Nach dem Tippen auf **SPORT** erscheint die Einstellungsansicht. Die drei Parameter werden nacheinander konfiguriert:

#### Schritt 1: Rundenanzahl

- Anzeige: **RUNDEN** (blau)
- Einstellbereich: **1–99 Runden**
- Steuerung: Tippe **−** oder **+** zum Anpassen
- Weiter: Tippe **MODE** (drei Punkte) oder **PLAY** um direkt zu starten

#### Schritt 2: Arbeitszeit

- Anzeige: **ARBEIT** (grün)
- Einstellbereich: **5–3600 Sekunden**
- Steuerung: **−** / **+** Buttons
- Weiter: **MODE** tippen

#### Schritt 3: Pausenzeit

- Anzeige: **PAUSE** (orange)
- Einstellbereich: **5–3600 Sekunden**
- Steuerung: **−** / **+** Buttons
- Starten: **PLAY** (Dreieck) tippen

### Ablauf nach dem Start

```
BEREIT (5 Sek. Countdown, gelb)
   ↓
ARBEIT (grün, zählt herunter)
   ↓
PAUSE (rot, zählt herunter)
   ↓
... Wiederholung für jede Runde ...
   ↓
FERTIG! (lila auf Slave, goldgelb auf Master)
```

Das Display zeigt oben links die aktuelle Runde (z. B. **RUNDE 2 / 5**) und in der Mitte die verbleibende Zeit.

### Bedienung während des Laufs

| Aktion | Geste |
|---|---|
| Pause | Tippe den mittig-unten Button |
| Fortsetzen | Tippe erneut |
| Zurück zum Setup | Langer Druck (2 Sek.) auf den Button |
| Zurück zum Menü | Tippe **< MENU** oben links |

### Signaltöne

| Ereignis | Ton |
|---|---|
| 3 Sek. vor Phasenwechsel | Mehrere kurze Warntöne (1500 Hz) |
| Arbeitsphase beginnt | Hoher Ton (800 Hz) |
| Pausenphase beginnt | Mittlerer Ton (1200 Hz) |
| Timer beendet | Langer Abschlusston |

### Beispiel: Tabata-Training

```
Runden:      8
Arbeitszeit: 20 Sekunden
Pausenzeit:  10 Sekunden
Gesamtzeit:  4 Minuten
```

---

## Modus 2: Stoppuhr

Die Stoppuhr misst Zeiten bis auf Hundertstel-Sekunden.

### Bedienung

1. Tippe im Hauptmenü auf **STOPPUHR**
2. Anzeige zeigt **00:00.00** (MM:SS.hh)
3. Tippe **PLAY** → Zeitmessung startet (grün)
4. Tippe **PAUSE** → Zeit stoppt (grau, "GESTOPPT")
5. Im gestoppten Zustand: Tippe **RESET** (links) → zurück auf 00:00.00

### Slave-Anzeige

Das LED-Display zeigt die Zeit im Format **MM:SS**. Wenn die Zeit gestoppt ist, blinkt der Doppelpunkt.

### Zurück zum Menü

Tippe **< MENU** oben links auf dem Touchscreen.

---

## Modus 3: Countdown

Der Countdown zählt von einer eingestellten Zeit auf null herunter.

### Einrichten

1. Tippe im Hauptmenü auf **COUNTDOWN**
2. Standardzeit: **01:00** (60 Sekunden)
3. Tippe **−** oder **+** zum Anpassen (Schritte: 10 Sekunden; Bereich: 10 s – 36.000 s / 10 h)

### Ablauf nach dem Start

```
BEREIT (5 Sek. Countdown, gelb)
   ↓
Countdown läuft (orange)
   ↓
ZEIT UM! (rot, bei Null)
```

### Bedienung während des Laufs

| Aktion | Geste |
|---|---|
| Pause | Mittig-unten Button tippen |
| Fortsetzen | Erneut tippen |
| Zurück zum Setup | Langer Druck auf Button |

---

## Modus 4: Spielstand

Der Spielstand-Modus zeigt Punkte für zwei Teams oder Spieler an – ohne Zeitkomponente.

### Bedienung

1. Tippe im Hauptmenü auf **SPIELSTAND**
2. Bildschirm zeigt zwei Scores nebeneinander:
   - **Links (Rot)**: Team 1 / Spieler 1
   - **Rechts (Grün)**: Team 2 / Spieler 2
3. Tippe die **Pfeil-nach-oben** Buttons um Punkte zu erhöhen
4. Tippe die **Pfeil-nach-unten** Buttons um Punkte zu verringern
5. Bereich: **0–99** Punkte pro Seite

### Slave-Anzeige

Das LED-Display zeigt den Spielstand im Format **RR:GG**, wobei die linken zwei Ziffern in Rot (Team 1) und die rechten zwei Ziffern in Grün (Team 2) leuchten. Der Doppelpunkt in der Mitte ist weiß.

---

## Slave-Display (LED-Großanzeige)

### Farben pro Betriebszustand

| Zustand | Farbe auf dem Slave |
|---|---|
| Uhrzeit- / Datumsanzeige (Hauptmenü) | Cyan, Doppelpunkt blinkt (Uhrzeit) / an (Datum) |
| Einstellmodus (Setup) | Rot, blinkend |
| Vorbereitung (5-Sek.) | Gelb |
| Arbeitsphase | Grün |
| Pausenphase | Rot |
| Timer-Pause (manuell) | Blau, Doppelpunkt blinkt |
| Fertig / Abgeschlossen | Lila |
| Spielstand (Team 1) | Rot |
| Spielstand (Team 2) | Grün |

### Uhrzeit- und Datumsanzeige

Steht der Master im Hauptmenü, wechselt die Großanzeige alle **5 Sekunden** zwischen:

- **HH:MM** – Aktuelle Uhrzeit, Doppelpunkt blinkt sekündlich
- **TT.MM** – Aktuelles Datum (Tag und Monat), Doppelpunkt permanent an

Verlässt der Master das Hauptmenü, wechselt die Anzeige sofort in den jeweiligen Timer-Modus.

### Kein Signal

Empfängt der Slave kein Signal vom Master (z. B. Master ausgeschaltet), bleibt die zuletzt empfangene Anzeige stehen.

---

## Uhrzeit und Datum einstellen

Da das Gerät keine Internetverbindung nutzt, werden Uhrzeit und Datum manuell eingestellt. Die eingestellten Werte werden in der **batteriepufferten Hardware-RTC** des Masters gespeichert und bleiben auch nach dem Ausschalten erhalten.

### Einstellungsdialog öffnen

Im Hauptmenü **lang drücken** (ca. 2 Sekunden) auf eine beliebige Stelle des Bildschirms.

### Schritt 1: Uhrzeit (1/2)

```
┌─────────────────────────────────┐
│    UHRZEIT EINSTELLEN      1/2  │
│  STUNDE          MINUTE         │
│    ▲                ▲           │
│   HH       :       MM           │
│    ▼                ▼           │
│         WEITER  >               │
└─────────────────────────────────┘
```

- Tippe **▲ / ▼** für Stunde (0–23) und Minute (0–59)
- Gedrückt halten für schnelles Scrollen
- Tippe **WEITER >** um zur Datumseinstellung zu gehen

### Schritt 2: Datum (2/2)

```
┌─────────────────────────────────┐
│    DATUM EINSTELLEN        2/2  │
│  TAG             MONAT          │
│    ▲                ▲           │
│   TT       .       MM           │
│    ▼                ▼           │
│        SPEICHERN                │
└─────────────────────────────────┘
```

- Tippe **▲ / ▼** für Tag (1–31) und Monat (1–12)
- Tippe **SPEICHERN** – die Werte werden in der RTC gesichert und sofort an die Großanzeige übertragen

### Abbrechen

**Lang drücken** auf einer beliebigen Stelle schließt den Dialog ohne zu speichern (auf beiden Screens).

> **Hinweis**: Das Jahr wird nicht eingestellt, da es für die Anzeige nicht benötigt wird.

---

## Energieverwaltung

### Master (M5Core2)

| Funktion | Einstellung |
|---|---|
| **Auto-Dimm** | Nach 60 Sek. Inaktivität → Helligkeit wird auf 5 % reduziert |
| **Auto-Off** | Nach 60 Min. im Hauptmenü → Gerät schaltet sich aus |
| **Akkuanzeige** | Oben rechts (Prozentwert) |
| **Ladestatus** | Oben rechts "LADEN" wenn per USB verbunden |

Ein Tippen auf den Touchscreen deaktiviert den Auto-Dimm-Modus sofort wieder.

### Slave

Der Slave hat keine eigene Energieverwaltung. Er läuft, solange er mit Strom versorgt wird. Die WS2812B-LEDs können bei voller Helligkeit bis zu **~4 A** verbrauchen – auf ausreichende Stromversorgung achten.

---

## Technische Daten

### Master

| Parameter | Wert |
|---|---|
| Gerät | M5Stack Core2 |
| Prozessor | ESP32 (Dual Core, 240 MHz) |
| Display | 2" IPS Touchscreen, 320×240 px |
| Drahtlos | ESP-NOW (2,4 GHz) |
| Echtzeituhr | Hardware-RTC (batteriepuffert) |
| Akku | 390 mAh (intern) |

### Slave

| Parameter | Wert |
|---|---|
| Gerät | Seeed Studio XIAO ESP32C3 |
| Prozessor | ESP32-C3 (Single Core, 160 MHz) |
| Drahtlos | ESP-NOW (2,4 GHz) |
| LEDs | 86× WS2812B RGB (GPIO 2) |
| Stromversorgung | 5 V via Netzteil – die USB-C Buchse ist nur zur Programmierung |

### Verbindung

| Parameter | Wert |
|---|---|
| Protokoll | ESP-NOW (direkte Peer-to-Peer-Verbindung) |
| Reichweite (innen) | ~40 m |
| Reichweite (außen) | ~100 m |
| Latenz | < 5 ms |
| Kein Router nötig | Ja |
